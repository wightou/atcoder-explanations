from __future__ import annotations

from collections import defaultdict
from html import escape, unescape
from pathlib import Path
import re
import sys

try:
    import markdown as markdown_lib  # type: ignore
except Exception:
    markdown_lib = None

from .config import IMAGE_EXTENSIONS, TABLE_CELL_BACKGROUNDS
from .models import PageAsset
from .navigation import ascii_slug, short_hash

def remove_duplicate_top_h1(body_md: str) -> str:
    lines = body_md.splitlines()
    i = 0
    while i < len(lines) and not lines[i].strip():
        i += 1
    if i < len(lines) and re.match(r"^#\s+", lines[i]):
        del lines[i]
        while i < len(lines) and not lines[i].strip():
            del lines[i]
    return "\n".join(lines)


def is_local_image_url(url: str) -> bool:
    url = url.strip()
    if not url or url.startswith("#") or url.startswith("/"):
        return False
    if re.match(r"^[A-Za-z][A-Za-z0-9+.-]*:", url):
        return False
    path_part = re.split(r"[?#]", url, maxsplit=1)[0]
    return Path(path_part).suffix.lower() in IMAGE_EXTENSIONS


def split_markdown_link_destination(raw: str) -> tuple[str, str]:
    """Markdown link destinationからURL本体と後続のtitle指定を分ける。簡易パーサ。"""
    raw = raw.strip()
    if raw.startswith("<"):
        end = raw.find(">")
        if end != -1:
            return raw[1:end], raw[end + 1 :]
    m = re.match(r"([^\s]+)(.*)\Z", raw, flags=re.S)
    if not m:
        return raw, ""
    return m.group(1), m.group(2)


def rewrite_markdown_image_links(
    body_md: str,
    *,
    source_path: Path,
    source_root: Path,
    output_subdir: str,
) -> tuple[str, list[PageAsset]]:
    """同じ入力ディレクトリに置いた画像を、生成HTMLから参照できるパスへ変換する。

    例: explanations/abc463/e.md から ![図](e1.png) と書いた場合、
    docs/explanations/abc463/e1.png にコピーし、HTML上の参照は abc463/e1.png にする。
    """
    assets: list[PageAsset] = []
    source_root_resolved = source_root.resolve()

    def replace(match: re.Match[str]) -> str:
        alt = match.group(1)
        dest_raw = match.group(2)
        url, suffix = split_markdown_link_destination(dest_raw)
        if not is_local_image_url(url):
            return match.group(0)

        path_part, *rest = re.split(r"([?#].*)", url, maxsplit=1)
        url_suffix = "".join(rest)
        image_source = (source_path.parent / path_part).resolve()
        try:
            image_rel_to_root = image_source.relative_to(source_root_resolved)
        except ValueError:
            print(f"WARNING: 画像パスが入力ディレクトリ外を指しています: {source_path}: {url}", file=sys.stderr)
            return match.group(0)

        output_path = Path(output_subdir) / image_rel_to_root
        assets.append(PageAsset(image_source, output_path))
        rewritten_url = image_rel_to_root.as_posix() + url_suffix
        return f"![{alt}]({rewritten_url}{suffix})"

    return re.sub(r"!\[([^\]]*)\]\(([^)\n]+)\)", replace, body_md), assets


def render_inline(s: str) -> str:
    s = escape(s)
    s = re.sub(r"`([^`]+)`", r"<code>\1</code>", s)
    s = re.sub(r"!\[([^\]]*)\]\(([^)]+)\)", r'<img src="\2" alt="\1">', s)
    s = re.sub(r"\[([^\]]+)\]\(([^)]+)\)", r'<a href="\2">\1</a>', s)
    return s


TABLE_ROW_HEADER_DIRECTIVE_RE = re.compile(
    r"^\s*<!--\s*table-row-header\s*:\s*(true|false)\s*-->\s*$",
    flags=re.I,
)


TABLE_ROW_HEADER_ANY_DIRECTIVE_RE = re.compile(
    r"^\s*<!--\s*table-row-header\s*:\s*([^>]*)-->\s*$",
    flags=re.I,
)


def prepare_markdown_table_row_headers(
    md: str,
    *,
    source_path: Path | None = None,
) -> tuple[str, list[bool]]:
    """Markdown表ごとの左列見出し指定を読み取り、指定コメントを本文から除く。

    各Markdown表の直前の非空行に
    `<!-- table-row-header: true -->` または `false` を要求する。
    指定漏れは警告し、後方互換のため `false` として扱う。
    コードブロック内は対象外。
    """
    lines = md.splitlines()
    out_lines = list(lines)
    flags: list[bool] = []
    consumed_directives: set[int] = set()
    directive_lines: set[int] = set()
    in_code = False

    def location(line_index: int) -> str:
        if source_path is None:
            return f"本文{line_index + 1}行目"
        return f"{source_path}（本文{line_index + 1}行目）"

    # まずコードブロック外の指定コメントを記録し、不正値も警告する。
    code_state = False
    for idx, line in enumerate(lines):
        if line.lstrip().startswith("```"):
            code_state = not code_state
            continue
        if code_state:
            continue
        any_match = TABLE_ROW_HEADER_ANY_DIRECTIVE_RE.match(line)
        if any_match is None:
            continue
        directive_lines.add(idx)
        if TABLE_ROW_HEADER_DIRECTIVE_RE.match(line) is None:
            value = any_match.group(1).strip()
            print(
                f"WARNING: table-row-header は true / false のいずれかを指定してください: {location(idx)}: {value!r}",
                file=sys.stderr,
            )

    i = 0
    while i < len(lines):
        line = lines[i]
        if line.lstrip().startswith("```"):
            in_code = not in_code
            i += 1
            continue
        if in_code:
            i += 1
            continue

        if (
            "|" in line
            and i + 1 < len(lines)
            and "|" in lines[i + 1]
            and is_markdown_table_separator(lines[i + 1])
        ):
            prev = i - 1
            while prev >= 0 and not lines[prev].strip():
                prev -= 1

            flag = False
            if prev >= 0:
                match = TABLE_ROW_HEADER_DIRECTIVE_RE.match(lines[prev])
                if match is not None:
                    flag = match.group(1).lower() == "true"
                    consumed_directives.add(prev)
                    out_lines[prev] = ""
                elif TABLE_ROW_HEADER_ANY_DIRECTIVE_RE.match(lines[prev]) is None:
                    print(
                        "WARNING: Markdown表の直前に "
                        "<!-- table-row-header: true --> または "
                        f"<!-- table-row-header: false --> を指定してください: {location(i)}",
                        file=sys.stderr,
                    )
            else:
                print(
                    "WARNING: Markdown表の直前に "
                    "<!-- table-row-header: true --> または "
                    f"<!-- table-row-header: false --> を指定してください: {location(i)}",
                    file=sys.stderr,
                )
            flags.append(flag)

            i += 2
            while i < len(lines) and lines[i].strip() and "|" in lines[i]:
                i += 1
            continue

        i += 1

    # 表に結び付かなかった正しい指定も警告し、HTMLへ露出しないよう除去する。
    for idx in sorted(directive_lines):
        if idx in consumed_directives:
            continue
        if TABLE_ROW_HEADER_DIRECTIVE_RE.match(lines[idx]) is not None:
            print(
                f"WARNING: table-row-header 指定の直後にMarkdown表がありません: {location(idx)}",
                file=sys.stderr,
            )
        out_lines[idx] = ""

    return "\n".join(out_lines), flags


def split_table_background_marker(cell: str) -> tuple[str | None, str]:
    """表セル先頭の `{red}` 形式を取り出す。未対応色は通常文字列として残す。"""
    match = re.match(r"^\{([a-z]+)\}\s*", cell, flags=re.I)
    if match is None:
        return None, cell
    color = match.group(1).lower()
    if color not in TABLE_CELL_BACKGROUNDS:
        return None, cell
    if color == "red" and re.search(r"\{/red\}", cell[match.end():], flags=re.I):
        return None, cell
    return color, cell[match.end():]


def append_html_class(attrs: str, class_name: str) -> str:
    """HTML開始タグの属性文字列へ class を安全に追記する。"""
    class_match = re.search(r'\bclass=(["\'])(.*?)\1', attrs, flags=re.I | re.S)
    if class_match is None:
        return attrs + f' class="{class_name}"'
    current = class_match.group(2).split()
    if class_name in current:
        return attrs
    updated = " ".join([*current, class_name])
    return attrs[:class_match.start(2)] + updated + attrs[class_match.end(2):]


def append_html_style(attrs: str, declaration: str) -> str:
    """HTML開始タグの属性文字列へ style 宣言を安全に追記する。"""
    style_match = re.search(r'\bstyle=(["\'])(.*?)\1', attrs, flags=re.I | re.S)
    if style_match is None:
        return attrs + f' style="{declaration}"'
    current = style_match.group(2).strip()
    if current and not current.endswith(";"):
        current += ";"
    updated = f"{current} {declaration}".strip()
    return attrs[:style_match.start(2)] + updated + attrs[style_match.end(2):]


def apply_table_backgrounds_to_html(html: str) -> str:
    """Python-Markdown が生成した th / td の先頭色指定を背景色へ変換する。"""
    color_pattern = "|".join(map(re.escape, TABLE_CELL_BACKGROUNDS))
    pattern = re.compile(
        rf"<(?P<tag>th|td)(?P<attrs>[^>]*)>(?P<space>\s*)\{{(?P<color>{color_pattern})\}}\s*",
        flags=re.I,
    )

    def replace(match: re.Match[str]) -> str:
        tag = match.group("tag")
        color = match.group("color").lower()
        attrs = append_html_class(match.group("attrs"), f"table-bg-{color}")
        attrs = append_html_style(attrs, f"background-color: {TABLE_CELL_BACKGROUNDS[color]};")
        return f'<{tag}{attrs}>{match.group("space")}'

    return pattern.sub(replace, html)


def apply_inline_text_colors_to_html(html: str) -> str:
    """本文の `{red}...{/red}` を赤文字へ変換する。code / pre 内は対象外。"""
    parts = re.split(r"(<[^>]+>)", html)
    blocked_tags = {"code", "pre", "script", "style"}
    blocked_depth = 0
    markers: list[tuple[int, int, int, bool]] = []
    marker_pattern = re.compile(r"\{(?P<close>/)?red\}", flags=re.I)

    for part_index, part in enumerate(parts):
        if not part:
            continue
        if part.startswith("<"):
            closing = re.match(r"</\s*([A-Za-z0-9]+)\b", part)
            if closing and closing.group(1).lower() in blocked_tags:
                blocked_depth = max(0, blocked_depth - 1)
                continue

            opening = re.match(r"<\s*([A-Za-z0-9]+)\b", part)
            if (
                opening
                and opening.group(1).lower() in blocked_tags
                and not part.rstrip().endswith("/>")
            ):
                blocked_depth += 1
            continue

        if blocked_depth != 0:
            continue

        for match in marker_pattern.finditer(part):
            markers.append((
                part_index,
                match.start(),
                match.end(),
                bool(match.group("close")),
            ))

    stack: list[int] = []
    matched: dict[int, str] = {}
    for marker_index, (_, _, _, is_closing) in enumerate(markers):
        if not is_closing:
            stack.append(marker_index)
            continue
        if not stack:
            continue
        opening_index = stack.pop()
        matched[opening_index] = '<span class="text-red">'
        matched[marker_index] = "</span>"

    replacements: dict[int, list[tuple[int, int, str]]] = {}
    for marker_index, replacement in matched.items():
        part_index, start, end, _ = markers[marker_index]
        replacements.setdefault(part_index, []).append((start, end, replacement))

    for part_index, items in replacements.items():
        part = parts[part_index]
        for start, end, replacement in sorted(items, reverse=True):
            part = part[:start] + replacement + part[end:]
        parts[part_index] = part

    return "".join(parts)


def parse_markdown_table_alignment(separator_cell: str) -> str | None:
    """Markdown表の区切りセルから配置指定を取り出す。"""
    marker = separator_cell.strip()
    if not re.fullmatch(r":?-{3,}:?", marker):
        return None
    if marker.startswith(":") and marker.endswith(":"):
        return "center"
    if marker.endswith(":"):
        return "right"
    if marker.startswith(":"):
        return "left"
    return None


def table_cell_html(tag: str, cell: str, alignment: str | None = None) -> str:
    color, content = split_table_background_marker(cell)
    attrs = ""
    if color:
        attrs = append_html_class(attrs, f"table-bg-{color}")
        attrs = append_html_style(attrs, f"background-color: {TABLE_CELL_BACKGROUNDS[color]};")
    if alignment in {"left", "center", "right"}:
        attrs = append_html_class(attrs, f"table-align-{alignment}")
        attrs = append_html_style(attrs, f"text-align: {alignment};")
    return f"<{tag}{attrs}>" + render_inline(content) + f"</{tag}>"


def is_markdown_table_separator(line: str) -> bool:
    parts = [part.strip() for part in line.strip().strip("|").split("|")]
    if not parts:
        return False
    for part in parts:
        if not re.fullmatch(r":?-{3,}:?", part):
            return False
    return True


def split_markdown_table_row(line: str) -> list[str]:
    # 簡易フォールバック用。エスケープされたパイプまでは扱わない。
    return [cell.strip() for cell in line.strip().strip("|").split("|")]


def render_markdown_table(table_lines: list[str]) -> str:
    if len(table_lines) < 2:
        return ""
    header = split_markdown_table_row(table_lines[0])
    separator = split_markdown_table_row(table_lines[1])
    alignments = [
        parse_markdown_table_alignment(separator[i]) if i < len(separator) else None
        for i in range(len(header))
    ]
    body_rows = [split_markdown_table_row(line) for line in table_lines[2:]]

    out = ['<table>', "<thead><tr>"]
    for i, cell in enumerate(header):
        out.append(table_cell_html("th", cell, alignments[i]))
    out.append("</tr></thead>")

    if body_rows:
        out.append("<tbody>")
        for row in body_rows:
            out.append("<tr>")
            for i in range(len(header)):
                cell = row[i] if i < len(row) else ""
                out.append(table_cell_html("td", cell, alignments[i]))
            out.append("</tr>")
        out.append("</tbody>")

    out.append("</table>")
    return "\n".join(out)


def normalize_unordered_list_indentation(md: str) -> str:
    """箇条書きのインデント幅を階層として解釈し、4スペース単位へ正規化する。

    `  - child` と `    - child` のどちらも、直前の浅い項目より1段深い
    箇条書きとして扱う。コードブロック内は変更しない。
    """
    lines = md.splitlines()
    out: list[str] = []
    indent_stack: list[int] = []
    in_code = False

    for line in lines:
        if line.startswith("```"):
            in_code = not in_code
            indent_stack = []
            out.append(line)
            continue

        if in_code:
            out.append(line)
            continue

        match = re.match(r"^(?P<indent>[ \t]*)-(?P<space>\s+)(?P<body>.*)$", line)
        if match is None:
            if line.strip():
                indent_stack = []
            out.append(line)
            continue

        indent = len(match.group("indent").expandtabs(4))
        if not indent_stack:
            indent_stack = [indent]
        elif indent > indent_stack[-1]:
            indent_stack.append(indent)
        elif indent < indent_stack[-1]:
            while len(indent_stack) > 1 and indent < indent_stack[-1]:
                indent_stack.pop()
            if indent != indent_stack[-1]:
                if indent > indent_stack[-1]:
                    indent_stack.append(indent)
                else:
                    indent_stack = [indent]

        depth = len(indent_stack) - 1
        out.append(" " * (4 * depth) + "- " + match.group("body"))

    return "\n".join(out)


def fallback_markdown_to_html(md: str) -> str:
    lines = normalize_unordered_list_indentation(md).splitlines()
    out: list[str] = []
    para: list[str] = []
    list_depth = -1
    in_code = False
    code_lines: list[str] = []
    i = 0

    def flush_para() -> None:
        nonlocal para
        if para:
            out.append("<p>" + render_inline("\n".join(para)).replace("\n", "<br>\n") + "</p>")
            para = []

    def close_ul() -> None:
        nonlocal list_depth
        if list_depth < 0:
            return
        out.append("</li>")
        while list_depth > 0:
            out.append("</ul>")
            out.append("</li>")
            list_depth -= 1
        out.append("</ul>")
        list_depth = -1

    while i < len(lines):
        line = lines[i]

        if line.startswith("```"):
            if in_code:
                out.append("<pre><code>" + escape("\n".join(code_lines)) + "</code></pre>")
                code_lines = []
                in_code = False
            else:
                flush_para()
                close_ul()
                in_code = True
            i += 1
            continue

        if in_code:
            code_lines.append(line)
            i += 1
            continue

        if not line.strip():
            flush_para()
            close_ul()
            i += 1
            continue

        if (
            "|" in line
            and i + 1 < len(lines)
            and "|" in lines[i + 1]
            and is_markdown_table_separator(lines[i + 1])
        ):
            flush_para()
            close_ul()
            table_lines = [line, lines[i + 1]]
            i += 2
            while i < len(lines) and lines[i].strip() and "|" in lines[i]:
                table_lines.append(lines[i])
                i += 1
            out.append(render_markdown_table(table_lines))
            continue

        h = re.match(r"^(#{1,6})\s+(.*)$", line)
        if h:
            flush_para()
            close_ul()
            level = len(h.group(1))
            out.append(f"<h{level}>{render_inline(h.group(2))}</h{level}>")
            i += 1
            continue

        list_item = re.match(r"^(?P<indent> *)(?:-)\s+(?P<body>.*)$", line)
        if list_item:
            flush_para()
            depth = len(list_item.group("indent")) // 4
            body = render_inline(list_item.group("body").strip())

            if list_depth < 0:
                out.append("<ul>")
                list_depth = 0
                out.append("<li>" + body)
            elif depth > list_depth:
                while list_depth < depth:
                    out.append("<ul>")
                    list_depth += 1
                out.append("<li>" + body)
            elif depth == list_depth:
                out.append("</li>")
                out.append("<li>" + body)
            else:
                out.append("</li>")
                while list_depth > depth:
                    out.append("</ul>")
                    out.append("</li>")
                    list_depth -= 1
                out.append("<li>" + body)

            i += 1
            continue

        para.append(line)
        i += 1

    flush_para()
    close_ul()
    if in_code:
        out.append("<pre><code>" + escape("\n".join(code_lines)) + "</code></pre>")
    return "\n".join(out)


def apply_table_alignments_to_html(html: str) -> str:
    """Markdown実装が出力した表セルの配置指定をCSSより強いインライン指定へ統一する。"""
    tag_pattern = re.compile(r"<(?P<tag>th|td)(?P<attrs>[^>]*)>", flags=re.I)
    align_attr_pattern = re.compile(
        r"\balign\s*=\s*(?:\"(?P<double>left|center|right)\"|'(?P<single>left|center|right)'|(?P<bare>left|center|right))",
        flags=re.I,
    )
    style_pattern = re.compile(r"\btext-align\s*:\s*(left|center|right)\b", flags=re.I)

    def replace(match: re.Match[str]) -> str:
        attrs = match.group("attrs")
        style_match = style_pattern.search(attrs)
        alignment: str | None = None
        if style_match is not None:
            alignment = style_match.group(1).lower()
        else:
            align_match = align_attr_pattern.search(attrs)
            if align_match is not None:
                alignment = next(
                    value.lower()
                    for value in (align_match.group("double"), align_match.group("single"), align_match.group("bare"))
                    if value is not None
                )
        if alignment is None:
            return match.group(0)

        attrs = append_html_class(attrs, f"table-align-{alignment}")
        if style_match is None:
            attrs = append_html_style(attrs, f"text-align: {alignment};")
        return f'<{match.group("tag")}{attrs}>'

    return tag_pattern.sub(replace, html)


def apply_table_row_headers_to_html(html: str, row_header_flags: list[bool]) -> str:
    """Markdown表の指定に従い、tbody各行の先頭tdを行見出しthへ変換する。"""
    table_pattern = re.compile(r"<table\b[^>]*>.*?</table>", flags=re.I | re.S)
    matches = list(table_pattern.finditer(html))
    if len(matches) != len(row_header_flags):
        # 通常は一致する。raw HTML の table などが混在した場合は誤変換を避ける。
        print(
            "WARNING: Markdown表の検出数と生成HTMLの table 数が一致しないため、"
            "左列見出し指定を適用できません。",
            file=sys.stderr,
        )
        return html

    def convert_table(table_html: str) -> str:
        tbody_pattern = re.compile(r"(<tbody\b[^>]*>)(.*?)(</tbody>)", flags=re.I | re.S)

        def convert_tbody(match: re.Match[str]) -> str:
            body = match.group(2)
            row_pattern = re.compile(r"(<tr\b[^>]*>)(.*?)(</tr>)", flags=re.I | re.S)

            def convert_row(row_match: re.Match[str]) -> str:
                row_body = row_match.group(2)
                cell_pattern = re.compile(
                    r"(?P<prefix>^\s*)<td(?P<attrs>[^>]*)>(?P<body>.*?)</td>",
                    flags=re.I | re.S,
                )
                cell_match = cell_pattern.search(row_body)
                if cell_match is None:
                    return row_match.group(0)
                attrs = cell_match.group("attrs")
                if re.search(r"\bscope\s*=", attrs, flags=re.I) is None:
                    attrs += ' scope="row"'
                replacement = (
                    cell_match.group("prefix")
                    + f"<th{attrs}>"
                    + cell_match.group("body")
                    + "</th>"
                )
                row_body = row_body[:cell_match.start()] + replacement + row_body[cell_match.end():]
                return row_match.group(1) + row_body + row_match.group(3)

            return match.group(1) + row_pattern.sub(convert_row, body) + match.group(3)

        return tbody_pattern.sub(convert_tbody, table_html)

    out: list[str] = []
    last = 0
    for match, use_row_header in zip(matches, row_header_flags):
        out.append(html[last:match.start()])
        table_html = match.group(0)
        out.append(convert_table(table_html) if use_row_header else table_html)
        last = match.end()
    out.append(html[last:])
    return "".join(out)


def wrap_markdown_tables_html(html: str) -> str:
    """Markdown本文中の表を横スクロール用の要素で囲む。"""
    pattern = re.compile(r"(<table\b[^>]*>.*?</table>)", flags=re.I | re.S)
    return pattern.sub(r'<div class="markdown-table-scroll">\1</div>', html)


def markdown_to_html(body_md: str, *, source_path: Path | None = None) -> str:
    body_md, row_header_flags = prepare_markdown_table_row_headers(body_md, source_path=source_path)
    body_md = remove_duplicate_top_h1(body_md)
    body_md = normalize_unordered_list_indentation(body_md)
    if markdown_lib is not None:
        html = markdown_lib.markdown(
            body_md,
            extensions=["extra", "toc", "fenced_code", "tables", "sane_lists", "nl2br"],
            output_format="html5",
        )
    else:
        html = fallback_markdown_to_html(body_md)
    html = apply_inline_text_colors_to_html(html)
    html = apply_table_backgrounds_to_html(html)
    html = apply_table_alignments_to_html(html)
    html = apply_table_row_headers_to_html(html, row_header_flags)
    return wrap_markdown_tables_html(html)


def add_heading_anchors_and_build_toc(html: str) -> tuple[str, str]:
    """本文中の h2〜h6 に固有IDを付け、全見出しから目次HTMLを作る。

    ページタイトルは本文HTMLの外側で生成しているため対象外。
    見出しレベルが飛んだ場合も、直前のより浅い見出しを親として扱う。
    """
    heading_pattern = re.compile(
        r"<h(?P<level>[2-6])(?P<attrs>[^>]*)>(?P<body>.*?)</h(?P=level)>",
        flags=re.I | re.S,
    )
    id_pattern = re.compile(
        r"\s+id\s*=\s*(?:\"(?P<double>[^\"]*)\"|'(?P<single>[^']*)'|(?P<bare>[^\s>]+))",
        flags=re.I,
    )
    tag_pattern = re.compile(r"<[^>]+>")

    headings: list[dict[str, object]] = []
    id_counts: dict[str, int] = defaultdict(int)
    used_ids: set[str] = set()

    def make_anchor_id(text: str) -> str:
        plain = re.sub(r"\s+", " ", unescape(text)).strip()
        base = ascii_slug(plain)
        if base:
            base = f"section-{base}"
        else:
            base = f"section-{short_hash(plain or 'heading', 10)}"
        while True:
            id_counts[base] += 1
            candidate = base if id_counts[base] == 1 else f"{base}-{id_counts[base]}"
            if candidate not in used_ids:
                return candidate

    def replace_heading(match: re.Match[str]) -> str:
        level = int(match.group("level"))
        attrs = match.group("attrs")
        body = match.group("body")
        plain_text = unescape(tag_pattern.sub("", body)).strip()

        existing_id_match = id_pattern.search(attrs)
        existing_id = ""
        if existing_id_match is not None:
            existing_id = next(
                (value for value in (
                    existing_id_match.group("double"),
                    existing_id_match.group("single"),
                    existing_id_match.group("bare"),
                ) if value is not None),
                "",
            )
            existing_id = unescape(existing_id).strip()

        if existing_id and existing_id not in used_ids:
            anchor_id = existing_id
            rendered_attrs = attrs
        else:
            anchor_id = make_anchor_id(plain_text)
            rendered_attrs = id_pattern.sub("", attrs)
            rendered_attrs += f' id="{escape(anchor_id, quote=True)}"'

        used_ids.add(anchor_id)
        headings.append({"level": level, "text": plain_text, "id": anchor_id, "children": []})
        return f'<h{level}{rendered_attrs}>{body}</h{level}>'

    html_with_anchors = heading_pattern.sub(replace_heading, html)
    if not headings:
        return html_with_anchors, ""

    roots: list[dict[str, object]] = []
    stack: list[dict[str, object]] = []
    for heading in headings:
        level = int(heading["level"])
        while stack and int(stack[-1]["level"]) >= level:
            stack.pop()
        if stack:
            children = stack[-1]["children"]
            assert isinstance(children, list)
            children.append(heading)
        else:
            roots.append(heading)
        stack.append(heading)

    def render_nodes(nodes: list[dict[str, object]]) -> str:
        items: list[str] = []
        for node in nodes:
            children = node["children"]
            assert isinstance(children, list)
            node_text = re.sub(r"\s+", "", str(node["text"]))
            suppress_children = node_text == "関連知識"
            child_html = render_nodes(children) if children and not suppress_children else ""
            items.append(
                f'<li><a href="#{escape(str(node["id"]), quote=True)}">{escape(str(node["text"]))}</a>'
                f'{child_html}</li>'
            )
        return "<ul>" + "".join(items) + "</ul>"

    toc_html = (
        '<nav class="page-toc" aria-label="目次">'
        '<div class="page-toc-title">目次</div>'
        f'{render_nodes(roots)}'
        '</nav>'
    )
    return html_with_anchors, toc_html


def markdown_to_plain_text(body_md: str) -> str:
    text = re.sub(r"```.*?```", " ", body_md, flags=re.S)
    text = re.sub(r"`([^`]*)`", r"\1", text)
    text = re.sub(r"!\[([^\]]*)\]\([^)]+\)", r"\1", text)
    text = re.sub(r"\[([^\]]+)\]\([^)]+\)", r"\1", text)
    text = re.sub(r"^[#>*\-\s]+", "", text, flags=re.M)
    text = re.sub(r"\s+", " ", text)
    return text.strip()
