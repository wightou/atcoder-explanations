#!/usr/bin/env python3
from __future__ import annotations

from collections import defaultdict
from dataclasses import dataclass
from html import escape
from pathlib import Path
from urllib.parse import quote, urlparse
import argparse
import hashlib
import re
import shutil
import sys

try:
    import tomllib
except Exception:
    tomllib = None

try:
    import yaml  # type: ignore
except Exception:
    yaml = None

try:
    import markdown as markdown_lib  # type: ignore
except Exception:
    markdown_lib = None


SITE_TITLE = "競技プログラミング 知識集積所"
EXTERNAL_LINKS = [
    ("AtCoder", "https://atcoder.jp/home"),
    ("AtCoder Problems", "https://kenkoooo.com/atcoder/"),
    ("AtCoder NoviSteps", "https://atcoder-novisteps.vercel.app/"),
    ("Aizu Online Judge", "https://judge.u-aizu.ac.jp/onlinejudge/"),
    ("Project Euler", "https://projecteuler.net/"),
]

DEFAULT_CONTEST_VIEWS = [
    {
        "id": "ABC",
        "title": "ABC",
        "path": "abc.html",
        "layout": "table",
        "order": "latest_first",
        "columns": list("ABCDEFG"),
    },
    {
        "id": "ARC",
        "title": "ARC",
        "path": "arc.html",
        "layout": "table",
        "order": "latest_first",
        "columns": list("ABCDEF"),
    },
    {
        "id": "AGC",
        "title": "AGC",
        "path": "agc.html",
        "layout": "table",
        "order": "latest_first",
        "columns": list("ABCDEF"),
    },
    {
        "id": "AHC",
        "title": "AHC",
        "path": "ahc.html",
        "layout": "list",
        "order": "latest_first",
        "columns": [],
    },
    {
        "id": "典型90問",
        "title": "典型90問",
        "path": "typical90.html",
        "layout": "list",
        "order": "oldest_first",
        "columns": [],
    },
    {
        "id": "教育系コンテスト",
        "title": "教育系コンテスト",
        "path": "educational.html",
        "layout": "grouped_list",
        "order": "oldest_first",
        "columns": [],
        "groups": ["EDPC", "TDPC", "NDPC", "FPS24"],
    },
    {
        "id": "その他AtCoder",
        "title": "その他AtCoder",
        "path": "other-atcoder.html",
        "layout": "list",
        "order": "latest_first",
        "columns": [],
    },
    {
        "id": "Aizu Online Judge",
        "title": "Aizu Online Judge",
        "path": "aoj.html",
        "layout": "list",
        "order": "oldest_first",
        "columns": [],
    },
    {
        "id": "Project Euler（100番まで）",
        "title": "Project Euler",
        "description": "（権利の都合上100番まで）",
        "path": "project-euler-100.html",
        "layout": "list",
        "order": "oldest_first",
        "columns": [],
    },
]


EDUCATIONAL_CONTEST_GROUPS = ["EDPC", "TDPC", "NDPC", "FPS24"]
EDUCATIONAL_CONTEST_ALIASES = {
    "dp": "EDPC",
    "educational_dp": "EDPC",
    "educationaldpcontest": "EDPC",
    "dpcontest": "EDPC",
    "edpc": "EDPC",
    "tdpc": "TDPC",
    "ndpc": "NDPC",
    "fps24": "FPS24",
}

EXTERNAL_LINK_GROUPS = [
    (
        "リンク（競プロ関係）",
        [
            ("AtCoder", "https://atcoder.jp/home", "競プロサイト"),
            ("AtCoder Problems", "https://kenkoooo.com/atcoder/", "AtCoderの非公式過去問集"),
            ("Aizu Online Judge", "https://judge.u-aizu.ac.jp/onlinejudge/", "会津大学のジャッジ"),
            ("Project Euler", "https://projecteuler.net/", "数学系競プロ（英語）"),
        ],
    ),
    (
        "",
        [
            ("", "https://w.atwiki.jp/sport_programming/pages/11.html", "自動更新"),
            ("", "https://w.atwiki.jp/sport_programming/pages/81.html", "手動更新"),
            ("@wiki", "https://atwiki.jp/", ""),
            ("", "https://w.atwiki.jp/guide/", ""),
        ],
    ),
]

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_EXPLANATIONS = ROOT / "explanations"
DEFAULT_KNOWLEDGE = ROOT / "knowledge"
DEFAULT_OUT = ROOT / "docs"
DEFAULT_CONFIG = ROOT / "site_config.toml"
IMAGE_EXTENSIONS = {".png", ".jpg", ".jpeg", ".gif", ".svg", ".webp", ".avif"}
SIDEBAR_KNOWLEDGE_PAGES: list["KnowledgePage"] = []
TAG_SLUG_MAP: dict[str, str] = {}

TABLE_TEXT_COLORS = {
    "red": "#b42318",
    "coral": "#b53a32",
    "orange": "#9a3412",
    "amber": "#7a5200",
    "yellow": "#713f12",
    "olive": "#5f5b00",
    "lime": "#4d7c0f",
    "green": "#15803d",
    "emerald": "#047857",
    "teal": "#0f766e",
    "cyan": "#0e7490",
    "blue": "#1d4ed8",
    "navy": "#1e3a5f",
    "indigo": "#4338ca",
    "violet": "#6d28d9",
    "purple": "#7e22ce",
    "magenta": "#a21caf",
    "rose": "#be123c",
    "pink": "#be185d",
    "brown": "#6f4e37",
    "gray": "#475569",
    "black": "#111827",
}

CATEGORY_ORDER = [
    "基本",
    "変数とデータ構造",
    "データ探索系",
    "高速化系",
    "文字列系",
    "グラフ理論系",
    "幾何学系",
    "その他数学系",
    "その他",
    "典型問題集",
    "未分類",
]

LEVEL_ORDER = [
    "難易度問わず",
    "A問題相当",
    "B問題相当",
    "C問題相当",
    "D問題相当",
    "E問題相当",
    "F問題相当",
    "G問題以上相当",
    "AHC用",
    "未分類",
]

def ordered_name_key(name: str, order: list[str]) -> tuple[int, str]:
    try:
        return (order.index(name), "")
    except ValueError:
        return (len(order), name)

def category_sort_key(name: str) -> tuple[int, str]:
    return ordered_name_key(name, CATEGORY_ORDER)

def level_sort_key(name: str) -> tuple[int, str]:
    return ordered_name_key(name, LEVEL_ORDER)


def is_visible_knowledge(page: "KnowledgePage") -> bool:
    """通常導線に表示する知識ページかどうか。未分類はファイル生成だけ許容し、一覧・検索から外す。"""
    return page.category != "未分類" and page.level != "未分類"


def visible_knowledge_pages(pages: list["KnowledgePage"]) -> list["KnowledgePage"]:
    return [p for p in pages if is_visible_knowledge(p)]




def warn_missing_knowledge_pages_for_tags(explanation_tag_names: set[str], knowledge_pages: list["KnowledgePage"]) -> list[str]:
    """問題解説で使われているタグに対応する知識記事がない場合、実行結果に警告を出す。

    title / aliases / absorbs のいずれかに一致する可視の知識記事があれば対応済みとみなす。
    docs/tags/ のタグページ自体は通常どおり生成する。
    """
    known_keys: set[str] = set()
    for page in visible_knowledge_pages(knowledge_pages):
        known_keys.update(page.tag_keys)

    missing = sorted(explanation_tag_names - known_keys, key=str)
    if missing:
        print("WARNING: 対応する知識記事が見つからないタグがあります。", file=sys.stderr)
        for tag in missing:
            print(f"  - {tag}", file=sys.stderr)
    return missing


def make_knowledge_key_map(knowledge_pages: list["KnowledgePage"]) -> dict[str, "KnowledgePage"]:
    """知識名・別名・吸収名から知識ページを引く辞書を作る。

    related は title / aliases / absorbs のいずれでも解決できるようにする。
    通常の導線に出さない未分類ページは、関連知識リンクの候補から外す。
    """
    result: dict[str, KnowledgePage] = {}
    for page in visible_knowledge_pages(knowledge_pages):
        for key in page.tag_keys:
            result.setdefault(key, page)
    return result


def resolve_related_knowledge_pages(
    page: "KnowledgePage",
    key_map: dict[str, "KnowledgePage"],
) -> tuple[list["KnowledgePage"], list[str]]:
    resolved: list[KnowledgePage] = []
    missing: list[str] = []
    seen_paths: set[Path] = {page.source_path}

    for raw in page.related:
        key = str(raw).strip()
        if not key:
            continue
        related_page = key_map.get(key)
        if related_page is None:
            missing.append(key)
            continue
        if related_page.source_path in seen_paths:
            continue
        seen_paths.add(related_page.source_path)
        resolved.append(related_page)

    return resolved, missing


def warn_missing_related_knowledge_pages(missing_by_page: dict[Path, list[str]]) -> None:
    if not missing_by_page:
        return
    print("WARNING: related に対応する知識記事が見つからない項目があります。", file=sys.stderr)
    for path in sorted(missing_by_page, key=lambda p: str(p)):
        names = ", ".join(missing_by_page[path])
        print(f"  - {path.relative_to(ROOT)}: {names}", file=sys.stderr)


@dataclass(frozen=True)
class PageAsset:
    source_path: Path
    output_path: Path


@dataclass(frozen=True)
class ExplanationPage:
    source_path: Path
    meta: dict
    body_md: str
    body_html: str
    body_text: str
    url: str
    assets: list[PageAsset]

    @property
    def contest(self) -> str:
        return str(self.meta["contest"])

    @property
    def problem(self) -> str:
        return str(self.meta["problem"])

    @property
    def problem_title(self) -> str:
        return str(self.meta["problem_title"])

    @property
    def problem_title_ja(self) -> str:
        return str(self.meta.get("problem_title_ja", "")).strip()

    @property
    def problem_code(self) -> str:
        return f"{self.contest} {self.problem}"

    @property
    def full_title(self) -> str:
        return f"{self.problem_code} - {self.problem_title}"

    @property
    def full_title_with_ja(self) -> str:
        if self.problem_title_ja:
            return f"{self.full_title}（{self.problem_title_ja}）"
        return self.full_title

    @property
    def tags(self) -> list[str]:
        tags = self.meta.get("tags", [])
        if tags is None:
            return []
        if not isinstance(tags, list):
            raise ValueError(f"{self.source_path}: tags は YAML 配列にしてください")
        return [str(x) for x in tags]

    @property
    def needs_mathjax(self) -> bool:
        return markdown_needs_mathjax(self.body_md)


@dataclass(frozen=True)
class KnowledgePage:
    source_path: Path
    meta: dict
    body_md: str
    body_html: str
    body_text: str
    url: str
    assets: list[PageAsset]

    @property
    def title(self) -> str:
        return str(self.meta["title"])

    @property
    def category(self) -> str:
        return str(self.meta.get("category", "未分類"))

    @property
    def level(self) -> str:
        return str(self.meta.get("level", "未設定"))

    @property
    def summary(self) -> str:
        return str(self.meta.get("summary", "")).strip()

    @property
    def needs_mathjax(self) -> bool:
        return markdown_needs_mathjax(self.body_md)

    def _int_meta(self, key: str, default: int = 9999) -> int:
        raw = self.meta.get(key, default)
        try:
            return int(raw)
        except Exception as exc:
            raise ValueError(f"{self.source_path}: {key} は整数にしてください") from exc

    @property
    def category_order(self) -> int:
        return self._int_meta("category_order")

    @property
    def level_order(self) -> int:
        return self._int_meta("level_order")

    @property
    def aliases(self) -> list[str]:
        aliases = self.meta.get("aliases", [])
        if aliases is None:
            return []
        if not isinstance(aliases, list):
            raise ValueError(f"{self.source_path}: aliases は YAML 配列にしてください")
        return [str(x) for x in aliases]

    @property
    def absorbs(self) -> list[str]:
        absorbs = self.meta.get("absorbs", [])
        if absorbs is None:
            return []
        if not isinstance(absorbs, list):
            raise ValueError(f"{self.source_path}: absorbs は YAML 配列にしてください")
        return [str(x) for x in absorbs]

    @property
    def related(self) -> list[str]:
        related = self.meta.get("related", [])
        if related is None:
            return []
        if not isinstance(related, list):
            raise ValueError(f"{self.source_path}: related は YAML 配列にしてください")
        return [str(x) for x in related]

    @property
    def tag_keys(self) -> list[str]:
        seen: set[str] = set()
        keys: list[str] = []
        for value in [self.title, *self.aliases, *self.absorbs]:
            key = str(value).strip()
            if key and key not in seen:
                seen.add(key)
                keys.append(key)
        return keys


def parse_simple_yaml(src: str) -> dict:
    if yaml is not None:
        data = yaml.safe_load(src) or {}
        if not isinstance(data, dict):
            raise ValueError("front matter は辞書にしてください")
        return data

    data: dict[str, object] = {}
    current_key: str | None = None
    current_list_item: dict[str, object] | None = None

    for raw in src.splitlines():
        line = raw.rstrip()
        if not line.strip() or line.lstrip().startswith("#"):
            continue

        if line.startswith("  - "):
            if current_key is None:
                raise ValueError("YAML配列の親キーがありません")
            data.setdefault(current_key, [])
            if not isinstance(data[current_key], list):
                raise ValueError(f"{current_key} は配列として読めません")

            item = line[4:].strip()
            m = re.match(r"^([A-Za-z0-9_]+):(?:\s*(.*))?$", item)
            if m:
                current_list_item = {m.group(1): unquote_scalar(m.group(2) or "")}
                data[current_key].append(current_list_item)
            else:
                current_list_item = None
                data[current_key].append(unquote_scalar(item))
            continue

        if line.startswith("    "):
            if current_list_item is None:
                raise ValueError(f"簡易YAMLで読めない行です: {line}")
            m = re.match(r"^\s+([A-Za-z0-9_]+):(?:\s*(.*))?$", line)
            if not m:
                raise ValueError(f"簡易YAMLで読めない行です: {line}")
            current_list_item[m.group(1)] = unquote_scalar(m.group(2) or "")
            continue

        m = re.match(r"^([A-Za-z0-9_]+):(?:\s*(.*))?$", line)
        if not m:
            raise ValueError(f"簡易YAMLで読めない行です: {line}")

        key, value = m.group(1), m.group(2)
        current_key = key
        current_list_item = None
        data[key] = [] if value is None or value == "" else unquote_scalar(value)

    return data


def unquote_scalar(s: str) -> str:
    s = s.strip()
    if len(s) >= 2 and ((s[0] == s[-1] == '"') or (s[0] == s[-1] == "'")):
        return s[1:-1]
    if s == "null":
        return ""
    return s


def parse_front_matter(text: str, path: Path) -> tuple[dict, str]:
    m = re.match(r"\A---\s*\n(.*?)\n---\s*\n?(.*)\Z", text, flags=re.S)
    if not m:
        raise ValueError(f"{path}: front matter がありません")
    return parse_simple_yaml(m.group(1)), m.group(2)


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


def split_table_text_color_marker(cell: str) -> tuple[str | None, str]:
    """表セル先頭の `{red}` 形式を取り出す。未対応色は通常文字列として残す。"""
    match = re.match(r"^\{([a-z]+)\}\s*", cell, flags=re.I)
    if match is None:
        return None, cell
    color = match.group(1).lower()
    if color not in TABLE_TEXT_COLORS:
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


def apply_table_text_colors_to_html(html: str) -> str:
    """Python-Markdown が生成した th / td の先頭色指定を CSS class に変換する。"""
    color_pattern = "|".join(map(re.escape, TABLE_TEXT_COLORS))
    pattern = re.compile(
        rf"<(?P<tag>th|td)(?P<attrs>[^>]*)>(?P<space>\s*)\{{(?P<color>{color_pattern})\}}\s*",
        flags=re.I,
    )

    def replace(match: re.Match[str]) -> str:
        tag = match.group("tag")
        attrs = append_html_class(match.group("attrs"), f'table-text-{match.group("color").lower()}')
        return f'<{tag}{attrs}>{match.group("space")}'

    return pattern.sub(replace, html)


def table_cell_html(tag: str, cell: str) -> str:
    color, content = split_table_text_color_marker(cell)
    class_attr = f' class="table-text-{color}"' if color else ""
    return f"<{tag}{class_attr}>" + render_inline(content) + f"</{tag}>"


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
    body_rows = [split_markdown_table_row(line) for line in table_lines[2:]]

    out = ['<div class="markdown-table-scroll"><table>', "<thead><tr>"]
    for cell in header:
        out.append(table_cell_html("th", cell))
    out.append("</tr></thead>")

    if body_rows:
        out.append("<tbody>")
        for row in body_rows:
            out.append("<tr>")
            for i in range(len(header)):
                cell = row[i] if i < len(row) else ""
                out.append(table_cell_html("td", cell))
            out.append("</tr>")
        out.append("</tbody>")

    out.append("</table></div>")
    return "\n".join(out)


def fallback_markdown_to_html(md: str) -> str:
    lines = md.splitlines()
    out: list[str] = []
    para: list[str] = []
    in_ul = False
    in_code = False
    code_lines: list[str] = []
    i = 0

    def flush_para() -> None:
        nonlocal para
        if para:
            out.append("<p>" + render_inline("\n".join(para)).replace("\n", "<br>\n") + "</p>")
            para = []

    def close_ul() -> None:
        nonlocal in_ul
        if in_ul:
            out.append("</ul>")
            in_ul = False

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

        if line.startswith("- "):
            flush_para()
            if not in_ul:
                out.append("<ul>")
                in_ul = True
            out.append("<li>" + render_inline(line[2:].strip()) + "</li>")
            i += 1
            continue

        para.append(line)
        i += 1

    flush_para()
    close_ul()
    if in_code:
        out.append("<pre><code>" + escape("\n".join(code_lines)) + "</code></pre>")
    return "\n".join(out)


def markdown_needs_mathjax(body_md: str) -> bool:
    """Markdown本文にMathJaxが必要そうな数式区切りがあるか判定する。

    コードブロック・インラインコード内の `$` は数式扱いしない。
    判定は読み込み要否を決めるための軽量なものに留める。
    """
    text = re.sub(r"```.*?```", " ", body_md, flags=re.S)
    text = re.sub(r"`[^`]*`", " ", text)
    return bool(re.search(r"\$\$|(?<!\\)\$(?!\s)|\\\(|\\\[", text))


def markdown_to_html(body_md: str) -> str:
    body_md = remove_duplicate_top_h1(body_md)
    if markdown_lib is not None:
        html = markdown_lib.markdown(
            body_md,
            extensions=["extra", "toc", "fenced_code", "tables", "sane_lists", "nl2br"],
            output_format="html5",
        )
    else:
        html = fallback_markdown_to_html(body_md)
    return apply_table_text_colors_to_html(html)


def markdown_to_plain_text(body_md: str) -> str:
    text = re.sub(r"```.*?```", " ", body_md, flags=re.S)
    text = re.sub(r"`([^`]*)`", r"\1", text)
    text = re.sub(r"!\[([^\]]*)\]\([^)]+\)", r"\1", text)
    text = re.sub(r"\[([^\]]+)\]\([^)]+\)", r"\1", text)
    text = re.sub(r"^[#>*\-\s]+", "", text, flags=re.M)
    text = re.sub(r"\s+", " ", text)
    return text.strip()


def require_keys(meta: dict, keys: list[str], path: Path) -> None:
    missing = [key for key in keys if key not in meta]
    if missing:
        raise ValueError(f"{path}: 必須項目がありません: {', '.join(missing)}")


def safe_filename(name: str) -> str:
    name = str(name).strip()
    name = re.sub(r"[\\/:*?\"<>|#%{}^~\[\]`;\s]+", "_", name)
    return name or "untitled"


def slug_id(value: str) -> str:
    return quote(safe_filename(value))


def ascii_slug(value: str) -> str:
    """URL・ファイル名用のASCII slugを作る。日本語だけの文字列は空になる。"""
    text = str(value).strip().lower()
    text = text.replace("++", "pp")
    text = text.replace("#", "sharp")
    text = re.sub(r"[^a-z0-9]+", "-", text)
    text = re.sub(r"-+", "-", text).strip("-")
    return text


def short_hash(value: str, n: int = 8) -> str:
    return hashlib.sha1(str(value).encode("utf-8")).hexdigest()[:n]


def fallback_tag_slug(tag: str) -> str:
    slug = ascii_slug(tag)
    if slug:
        return slug
    return f"tag-{short_hash(tag)}"


def make_tag_slug_map(tags: list[str], knowledge_pages: list["KnowledgePage"]) -> dict[str, str]:
    """タグ名から docs/tags/<slug>.html の <slug> を決める。

    表示名は日本語のまま、ファイル名だけASCIIにする。
    知識記事の title と一致するタグは、必ず知識記事ファイル名の stem を優先する。
    これにより、たとえば title: ビームサーチ / file: beam-search.md のページは、
    alias の beam search と衝突しても docs/tags/beam-search.html を確保する。
    """
    title_slug: dict[str, str] = {}
    for k in knowledge_pages:
        title_slug[k.title] = safe_filename(k.source_path.stem)

    result: dict[str, str] = {}
    used: dict[str, str] = {}

    # まず知識記事 title を割り当てる。title の slug を最優先で確保する。
    for tag in sorted(set(tags) & set(title_slug), key=str):
        base = title_slug[tag]
        slug = base
        if slug in used and used[slug] != tag:
            slug = f"{base}-{short_hash(tag, 6)}"
        used[slug] = tag
        result[tag] = slug

    # 次に、それ以外のタグを割り当てる。
    for tag in sorted(set(tags) - set(result), key=str):
        base = fallback_tag_slug(tag)
        slug = base
        if slug in used and used[slug] != tag:
            slug = f"{base}-{short_hash(tag, 6)}"
        used[slug] = tag
        result[tag] = slug
    return result


def tag_filename(tag: str) -> str:
    slug = TAG_SLUG_MAP.get(tag) or fallback_tag_slug(tag)
    return f"{slug}.html"


def tag_href(tag: str, *, from_subdir: bool = False) -> str:
    prefix = "../" if from_subdir else ""
    return prefix + "tags/" + quote(tag_filename(tag))


CATEGORY_PAGE_SLUGS = {
    "基本": "basic",
    "変数とデータ構造": "variables-and-data-structures",
    "データ探索系": "data-search",
    "典型問題集": "typical-problems",
    "高速化系": "optimization",
    "文字列系": "strings",
    "グラフ理論系": "graph-theory",
    "幾何学系": "geometry",
    "その他数学系": "other-math",
    "その他": "other",
    "未分類": "unclassified",
}

LEVEL_PAGE_SLUGS = {
    "難易度問わず": "all-levels",
    "A問題相当": "a",
    "B問題相当": "b",
    "C問題相当": "c",
    "D問題相当": "d",
    "E問題相当": "e",
    "F問題相当": "f",
    "G問題以上相当": "g-and-above",
    "AHC用": "ahc",
    "未分類": "unclassified",
}

def category_page_filename(name: str) -> str:
    return f"{CATEGORY_PAGE_SLUGS.get(name) or fallback_tag_slug(name)}.html"

def level_page_filename(name: str) -> str:
    return f"{LEVEL_PAGE_SLUGS.get(name) or fallback_tag_slug(name)}.html"

def category_page_href(name: str, *, prefix: str = "") -> str:
    return f"{prefix}knowledge/categories/{quote(category_page_filename(name))}"

def level_page_href(name: str, *, prefix: str = "") -> str:
    return f"{prefix}knowledge/levels/{quote(level_page_filename(name))}"


def explanation_filename(contest: str, problem: str) -> str:
    return f"{contest.lower()}-{problem.lower()}.html"


def knowledge_filename(path: Path) -> str:
    return f"{safe_filename(path.stem)}.html"


def normalized_contest_key(contest: str) -> str:
    return str(contest).lower().replace(" ", "").replace("-", "_")


def educational_contest_group(contest: str) -> str | None:
    return EDUCATIONAL_CONTEST_ALIASES.get(normalized_contest_key(contest))


def educational_group_sort_key(name: str) -> tuple[int, str]:
    try:
        return (EDUCATIONAL_CONTEST_GROUPS.index(name), "")
    except ValueError:
        return (len(EDUCATIONAL_CONTEST_GROUPS), name)


def contest_url_from_pages(pages: list["ExplanationPage"]) -> str:
    for page in pages:
        url = str(page.meta.get("problem_url", "")).strip()
        m = re.match(r"(https://atcoder\.jp/contests/[^/]+)", url)
        if m:
            return m.group(1)
    return ""



def render_problem_card_link(
    page: "ExplanationPage",
    *,
    code_label: str,
    href: str,
    css_class: str = "problem-cell",
) -> str:
    """問題一覧用の3点配置カードを生成する。

    左上に問題コード、中央に英語タイトル、右下に任意の日本語タイトルを表示する。
    """
    title_ja_html = (
        f'<span class="problem-title-ja">{escape(page.problem_title_ja)}</span>'
        if page.problem_title_ja else ""
    )
    return (
        f'<a class="{css_class}" href="{escape(href)}" title="{escape(page.full_title_with_ja)}">'
        f'<span class="problem-code">{escape(code_label)}</span>'
        f'<span class="problem-title">{escape(page.problem_title)}</span>'
        f'{title_ja_html}'
        '</a>'
    )


def contest_sort_key(contest: str):
    m = re.fullmatch(r"([A-Za-z]+)(\d+)", contest)
    if m:
        return (m.group(1).upper(), int(m.group(2)))
    c = contest.lower()
    if c in {"typical90", "typical"}:
        return ("TYPICAL90", 90)
    educational_group = educational_contest_group(contest)
    if educational_group:
        try:
            return ("EDU", EDUCATIONAL_CONTEST_GROUPS.index(educational_group))
        except ValueError:
            return ("EDU", len(EDUCATIONAL_CONTEST_GROUPS))
    return (contest.upper(), 10**18)


def problem_sort_key(problem: str):
    s = str(problem).upper()
    if re.fullmatch(r"\d+", s):
        return (0, int(s))
    if len(s) == 1 and "A" <= s <= "Z":
        return (1, ord(s) - ord("A"))
    return (2, s)


def page_sort_key(page: ExplanationPage):
    return (contest_sort_key(page.contest), problem_sort_key(page.problem), page.problem_title)


def tag_explanation_sort_key(page: ExplanationPage):
    """タグページ用の並び順。ABC→ARC→AGC→その他、各系列内ではA問題から順に並べる。"""
    contest_class = classify_page(page)
    class_order = {
        "ABC": 0,
        "ARC": 1,
        "AGC": 2,
        "AHC": 3,
        "典型90問": 4,
        "教育系コンテスト": 5,
        "その他AtCoder": 6,
        "Aizu Online Judge": 7,
        "Project Euler（100番まで）": 8,
        "その他": 9,
    }.get(contest_class, 99)
    if class_order < 3:
        return (class_order, problem_sort_key(page.problem), contest_sort_key(page.contest), page.problem_title)
    return (class_order, problem_sort_key(page.problem), contest_sort_key(page.contest), page.problem_title)


def category_item_sort_key(page: KnowledgePage):
    return (page.category_order, page.title)


def level_item_sort_key(page: KnowledgePage):
    return (page.level_order, page.title)


def knowledge_sort_key(page: KnowledgePage):
    return (level_sort_key(page.level), category_sort_key(page.category), page.level_order, page.category_order, page.title)



def normalize_contest_view(raw: dict) -> dict:
    spec = dict(raw)
    if "id" not in spec:
        raise ValueError("contest_views には id が必要です")
    spec.setdefault("title", str(spec["id"]))
    spec.setdefault("description", "")
    spec.setdefault("path", f"{str(spec['id']).lower()}.html")
    spec.setdefault("layout", "list")
    spec.setdefault("order", "latest_first")
    spec.setdefault("columns", [])

    if spec["layout"] not in {"table", "list", "grouped_table", "grouped_list"}:
        raise ValueError(f"{spec['id']}: layout は table / list / grouped_table / grouped_list のいずれかにしてください")
    if spec["order"] not in {"latest_first", "oldest_first"}:
        raise ValueError(f"{spec['id']}: order は latest_first または oldest_first にしてください")

    columns = spec.get("columns", [])
    if isinstance(columns, str):
        columns = list(columns)
    spec["columns"] = [str(x).upper() for x in columns]

    groups = spec.get("groups", [])
    if isinstance(groups, str):
        groups = [groups]
    spec["groups"] = [str(x).upper() for x in groups]
    return spec


def load_contest_views(config_path: Path = DEFAULT_CONFIG) -> list[dict]:
    if not config_path.exists():
        return [normalize_contest_view(spec) for spec in DEFAULT_CONTEST_VIEWS]

    if tomllib is None:
        raise RuntimeError("site_config.toml を読むには Python 3.11 以降が必要です")

    with config_path.open("rb") as f:
        data = tomllib.load(f)

    raw_views = data.get("contest_views", [])
    if not isinstance(raw_views, list):
        raise ValueError("site_config.toml: [[contest_views]] を配列として指定してください")

    if not raw_views:
        return [normalize_contest_view(spec) for spec in DEFAULT_CONTEST_VIEWS]

    return [normalize_contest_view(spec) for spec in raw_views]


def sort_pages_for_view(pages: list[ExplanationPage], order: str) -> list[ExplanationPage]:
    return sorted(pages, key=page_sort_key, reverse=(order == "latest_first"))


def classify_page(page: ExplanationPage) -> str:
    c = normalized_contest_key(page.contest)
    if re.fullmatch(r"abc\d+", c):
        return "ABC"
    if re.fullmatch(r"arc\d+", c):
        return "ARC"
    if re.fullmatch(r"agc\d+", c):
        return "AGC"
    if re.fullmatch(r"ahc\d+", c):
        return "AHC"
    if c in {"typical90", "typical", "競プロ典型90問"}:
        return "典型90問"
    if educational_contest_group(page.contest):
        return "教育系コンテスト"
    if c in {"aoj", "aizuonlinejudge", "aizu"} or c.startswith("aoj"):
        return "Aizu Online Judge"
    if c in {"projecteuler", "euler", "pe"} or c.startswith("projecteuler"):
        return "Project Euler（100番まで）"
    if "atcoder" in c or re.fullmatch(r"[a-z]+\d+", c):
        return "その他AtCoder"
    return "その他"


def is_external_href(href: str) -> bool:
    """サイト外リンクかどうかを判定する。

    生成サイト内リンクは相対パスで出すため、http(s) の絶対URLは外部リンクとして扱う。
    ページ内アンカーや mailto / tel / javascript は対象外にする。
    """
    href = str(href).strip()
    if not href or href.startswith("#"):
        return False
    lowered = href.lower()
    if lowered.startswith(("mailto:", "tel:", "javascript:")):
        return False
    if href.startswith("//"):
        return True
    parsed = urlparse(href)
    return parsed.scheme in {"http", "https"}


def add_external_link_attrs(html: str) -> str:
    """HTML内の外部リンクだけを別タブで開くようにする。"""

    def replace(match: re.Match[str]) -> str:
        before = match.group(1)
        quote_char = match.group(2)
        href = match.group(3)
        after = match.group(4)
        attrs = before + after
        if not is_external_href(href):
            return match.group(0)

        additions = ""
        if not re.search(r"\btarget\s*=", attrs, flags=re.I):
            additions += ' target="_blank"'
        if not re.search(r"\brel\s*=", attrs, flags=re.I):
            additions += ' rel="noopener noreferrer"'
        return f'<a{before}href={quote_char}{href}{quote_char}{after}{additions}>'

    return re.sub(r'<a\b([^>]*?)href=(["\'])(.*?)\2([^>]*)>', replace, html)


def mathjax_head_html() -> str:
    return """<script>
window.MathJax = {
  tex: {
    inlineMath: [['$', '$'], ['\\\\(', '\\\\)']],
    displayMath: [['$$', '$$'], ['\\\\[', '\\\\]']],
    processEscapes: true
  },
  svg: { fontCache: 'global' }
};
</script>
<script defer src="https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-svg.js"></script>"""


def problem_title_fit_head_html() -> str:
    return """<script>
function fitProblemTitleJa() {
  const items = document.querySelectorAll('.problem-title-ja');
  for (const el of items) {
    el.style.fontSize = '';
    const computed = window.getComputedStyle(el);
    const base = Number.parseFloat(computed.fontSize);
    if (!Number.isFinite(base) || base <= 0 || el.clientWidth <= 0) {
      continue;
    }
    const minSize = Math.max(9, base * 0.86);
    let size = base;
    while (el.scrollWidth > el.clientWidth && size > minSize) {
      size = Math.max(minSize, size - 0.5);
      el.style.fontSize = size + 'px';
    }
  }
}
window.addEventListener('DOMContentLoaded', fitProblemTitleJa);
window.addEventListener('load', fitProblemTitleJa);
window.addEventListener('resize', fitProblemTitleJa);
</script>"""


def html_document(
    title: str,
    body: str,
    *,
    css_href: str = "style.css",
    include_mathjax: bool = False,
    include_problem_title_fit: bool = False,
) -> str:
    body = add_external_link_attrs(body)
    head_parts = []
    if include_mathjax:
        head_parts.append(mathjax_head_html())
    if include_problem_title_fit:
        head_parts.append(problem_title_fit_head_html())
    head_extra = "\n".join(head_parts)
    return f"""<!doctype html>
<html lang="ja">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>{escape(title)}</title>
<link rel="stylesheet" href="{css_href}">
{head_extra}
</head>
<body>
{body}
</body>
</html>
"""


def site_header_compact(*, prefix: str = "") -> str:
    return f"""
<header class="site-header">
  <div class="site-header-inner">
    <a class="site-title" href="{prefix}index.html">{SITE_TITLE}</a>
  </div>
</header>
"""


def render_sidebar_nav(*, prefix: str = "../") -> str:
    explanation_items = [
        (str(spec["title"]), f"{prefix}contests/{spec['path']}")
        for spec in load_contest_views()
    ]

    sidebar_knowledge = visible_knowledge_pages(SIDEBAR_KNOWLEDGE_PAGES)
    category_names = sorted({p.category for p in sidebar_knowledge}, key=category_sort_key)
    level_names = sorted({p.level for p in sidebar_knowledge}, key=level_sort_key)

    category_items = [
        (name, category_page_href(name, prefix=prefix))
        for name in category_names
    ]
    level_items = [
        (name, level_page_href(name, prefix=prefix))
        for name in level_names
    ]

    def nested(items: list[tuple[str, str]]) -> str:
        if not items:
            return '<li class="muted">項目なし</li>'
        return "".join(
            f'<li><a href="{href}">{escape(label)}</a></li>'
            for label, href in items
        )

    return f"""
<li><a href="{prefix}index.html">トップ</a></li>
<li class="side-nav-group">
  <a href="{prefix}contests/index.html">問題解説</a>
  <ul class="side-sub-links">
    {nested(explanation_items)}
  </ul>
</li>
<li class="side-nav-group">
  <a href="{prefix}knowledge/by-category.html">知識集（分野別）</a>
  <ul class="side-sub-links">
    {nested(category_items)}
  </ul>
</li>
<li class="side-nav-group">
  <a href="{prefix}knowledge/by-level.html">知識集（難易度別）</a>
  <ul class="side-sub-links">
    {nested(level_items)}
  </ul>
</li>
"""


def render_external_links() -> str:
    return "".join(
        f'<li class="external-link-item"><a href="{href}">{escape(label)}</a></li>'
        for label, href in EXTERNAL_LINKS
    )


def common_sidebar(*, prefix: str = "") -> str:
    return f"""
<aside class="sidebar">
  <section class="side-card">
    <h2>リンク</h2>
    <ul class="side-links">
      {render_sidebar_nav(prefix=prefix)}
    </ul>
  </section>

  <section class="side-card">
    <h2>外部リンク</h2>
    <ul class="side-links">
      {render_external_links()}
    </ul>
  </section>
</aside>
"""


def render_tags(tags: list[str], *, from_subdir: bool = False) -> str:
    if not tags:
        return '<span class="muted">なし</span>'
    return "\n".join(
        f'<a class="tag" href="{tag_href(tag, from_subdir=from_subdir)}">{escape(tag)}</a>'
        for tag in tags
    )



def render_index_layout(
    title: str,
    inner_html: str,
    *,
    current: str = "",
    prefix: str = "",
    css_href: str = "style.css",
    include_mathjax: bool = False,
    include_problem_title_fit: bool = False,
) -> str:
    body = f"""
{site_header_compact(prefix=prefix)}
<main class="page-layout">
  <article class="main-panel">
    {inner_html}
  </article>
  {common_sidebar(prefix=prefix)}
</main>
"""
    return html_document(
        title,
        body,
        css_href=css_href,
        include_mathjax=include_mathjax,
        include_problem_title_fit=include_problem_title_fit,
    )


def render_portal(pages_by_class: dict[str, list[ExplanationPage]], knowledge_pages: list[KnowledgePage]) -> str:
    contest_items = [
        (str(spec["title"]), f"contests/{spec['path']}", len(pages_by_class.get(str(spec["id"]), [])))
        for spec in load_contest_views()
    ]

    visible_knowledge = visible_knowledge_pages(knowledge_pages)
    category_count: dict[str, int] = defaultdict(int)
    level_count: dict[str, int] = defaultdict(int)
    for k in visible_knowledge:
        category_count[k.category] += 1
        level_count[k.level] += 1

    category_items = [
        (name, category_page_href(name), count)
        for name, count in sorted(category_count.items(), key=lambda item: category_sort_key(item[0]))
    ]
    level_items = [
        (name, level_page_href(name), count)
        for name, count in sorted(level_count.items(), key=lambda item: level_sort_key(item[0]))
    ]

    def cards(items: list[tuple[str, str, int]]) -> str:
        if not items:
            return '<p class="muted">まだ項目がありません。</p>'
        return "".join(
            f"""
<a class="portal-card" href="{href}">
  <span class="portal-title">{escape(label)}</span>
  <span class="portal-count">{count}件</span>
</a>
"""
            for label, href, count in items
        )

    inner = f"""
<h1>{SITE_TITLE}</h1>

<section class="portal-section">
  <h2>問題解説</h2>
  <div class="portal-grid">
    {cards(contest_items)}
  </div>
</section>

<section class="portal-section">
  <h2>知識集（分野別）</h2>
  <div class="portal-grid">
    {cards(category_items)}
  </div>
</section>

<section class="portal-section">
  <h2>知識集（難易度別）</h2>
  <div class="portal-grid">
    {cards(level_items)}
  </div>
</section>
"""
    return render_index_layout(SITE_TITLE, inner, prefix="")


def render_explanations_index(pages_by_class: dict[str, list[ExplanationPage]]) -> str:
    items = [
        (str(spec["title"]), str(spec["path"]), len(pages_by_class.get(str(spec["id"]), [])))
        for spec in load_contest_views()
    ]
    card_html = "".join(
        f"""
<a class="portal-card" href="{href}">
  <span class="portal-title">{escape(label)}</span>
  <span class="portal-count">{count}件</span>
</a>
"""
        for label, href, count in items
    )
    inner = f"""
<h1>問題解説</h1>
<div class="portal-grid">
  {card_html}
</div>
"""
    return render_index_layout("問題解説", inner, current="問題解説", prefix="../", css_href="../style.css")


def render_table_page(title: str, pages: list[ExplanationPage], columns: list[str], *, order: str, description: str = "") -> str:
    rows_by_contest: dict[str, dict[str, ExplanationPage]] = defaultdict(dict)
    for p in pages:
        rows_by_contest[p.contest.upper()][p.problem.upper()] = p

    rows = []
    for contest in sorted(rows_by_contest.keys(), key=contest_sort_key, reverse=(order == "latest_first")):
        cells = []
        for problem in columns:
            p = rows_by_contest[contest].get(problem)
            if p is None:
                cells.append('<td class="empty">-</td>')
            else:
                problem_code = f"{p.contest.upper()} {p.problem.upper()}"
                cells.append(
                    '<td>'
                    + render_problem_card_link(p, code_label=problem_code, href=f"../{p.url}")
                    + '</td>'
                )
        rows.append(
            f'<tr><th><a href="https://atcoder.jp/contests/{contest.lower()}">{escape(contest)}</a></th>'
            + "".join(cells)
            + "</tr>"
        )

    empty_row = f'<tr><td class="empty" colspan="{len(columns)+1}">まだ解説がありません。</td></tr>'
    description_html = f'<p class="lead">{escape(description)}</p>' if description else ""
    inner = f"""
<h1>{escape(title)}</h1>
{description_html}
<div class="table-scroll">
  <table class="contest-table">
    <thead>
      <tr>
        <th>Contest</th>
        {''.join(f'<th>{c}</th>' for c in columns)}
      </tr>
    </thead>
    <tbody>
      {''.join(rows) if rows else empty_row}
    </tbody>
  </table>
</div>
"""
    return render_index_layout(
        title,
        inner,
        current="問題解説",
        prefix="../",
        css_href="../style.css",
        include_problem_title_fit=any(p.problem_title_ja for p in pages),
    )


def render_list_page(title: str, pages: list[ExplanationPage], *, order: str, description: str = "") -> str:
    items = []
    for p in sort_pages_for_view(pages, order):
        items.append(
            f'<li><a href="../{escape(p.url)}">{escape(p.full_title)}</a> '
            f'<span class="inline-tags">{render_tags(p.tags, from_subdir=True)}</span></li>'
        )

    description_html = f'<p class="lead">{escape(description)}</p>' if description else ""
    inner = f"""
<h1>{escape(title)}</h1>
{description_html}
<ul class="plain-list">
  {''.join(items) if items else '<li class="muted">まだ解説がありません。</li>'}
</ul>
"""
    return render_index_layout(title, inner, current="問題解説", prefix="../", css_href="../style.css")



def render_grouped_table_page(
    title: str,
    pages: list[ExplanationPage],
    *,
    order: str,
    groups: list[str] | None = None,
    description: str = "",
) -> str:
    """教育系コンテストなどを、グループごとのカード一覧として表示する。

    各カードは通常コンテスト表の問題セルと同じく、左上に問題コード、
    中央に英語タイトル、右下に任意の日本語タイトルを表示する。
    タグは出さない。layout 名は後方互換のため grouped_table も受け付ける。
    """
    pages_by_group: dict[str, list[ExplanationPage]] = defaultdict(list)
    for page in pages:
        group = educational_contest_group(page.contest) or page.contest.upper()
        pages_by_group[group].append(page)

    group_order = groups or []

    def group_key(name: str) -> tuple[int, str]:
        if name in group_order:
            return (group_order.index(name), "")
        return educational_group_sort_key(name)

    chunks: list[str] = []
    for group in sorted(pages_by_group.keys(), key=group_key):
        group_pages = sort_pages_for_view(pages_by_group[group], order)
        contest_url = contest_url_from_pages(group_pages)
        heading = (
            f'<h2><a href="{escape(contest_url)}">{escape(group)}</a></h2>'
            if contest_url else f'<h2>{escape(group)}</h2>'
        )
        items = []
        for page in group_pages:
            code_label = f"{group} {page.problem.upper()}"
            items.append(
                render_problem_card_link(
                    page,
                    code_label=code_label,
                    href=f"../{page.url}",
                    css_class="problem-cell educational-problem-card",
                )
            )
        chunks.append(f"""
<section class="contest-subsection">
  {heading}
  <div class="education-card-grid">
    {''.join(items) if items else '<p class="muted">まだ解説がありません。</p>'}
  </div>
</section>
""")

    description_html = f'<p class="lead">{escape(description)}</p>' if description else ""
    inner = f"""
<h1>{escape(title)}</h1>
{description_html}
<div class="group-list">
  {''.join(chunks) if chunks else '<p class="muted">まだ解説がありません。</p>'}
</div>
"""
    return render_index_layout(
        title,
        inner,
        current="問題解説",
        prefix="../",
        css_href="../style.css",
        include_problem_title_fit=any(p.problem_title_ja for p in pages),
    )


def get_alternative_submission_links(meta: dict) -> list[tuple[str, str]]:
    links: list[tuple[str, str]] = []

    single = str(meta.get("alternative_submission_url", "")).strip()
    if single:
        links.append(("別解", single))

    raw = meta.get("alternative_submission_urls", [])
    if raw is None:
        return links

    if isinstance(raw, str):
        if raw.strip():
            links.append(("別解", raw.strip()))
        return links

    if not isinstance(raw, list):
        return links

    for i, item in enumerate(raw, start=1):
        default_label = "別解" if len(raw) == 1 else f"別解{i}"
        if isinstance(item, dict):
            label = str(item.get("label", default_label)).strip() or default_label
            url = str(item.get("url", "")).strip()
        else:
            label = default_label
            url = str(item).strip()
        if url:
            links.append((label, url))

    return links


def render_explanation_page(page: ExplanationPage) -> str:
    tag_note = page.meta.get("tag_note")
    tag_note_html = f'<p class="tag-note">※ {escape(str(tag_note))}</p>' if tag_note else ""

    problem_url = str(page.meta.get("problem_url", "")).strip()
    submission_url = str(page.meta.get("submission_url", "")).strip()
    auto_link_items = []
    if problem_url:
        auto_link_items.append(f'<a href="{escape(problem_url)}">問題ページ</a>')
    if submission_url:
        auto_link_items.append(f'<a href="{escape(submission_url)}">解答例</a>')
    for label, url in get_alternative_submission_links(page.meta):
        auto_link_items.append(f'<a href="{escape(url)}">{escape(label)}</a>')
    auto_links = (
        '<div class="auto-links">' + ''.join(auto_link_items) + '</div>'
        if auto_link_items else ""
    )

    body = f"""
{site_header_compact(prefix="../")}
<main class="page-layout">
  <article class="markdown-body">
    <h1>{escape(page.full_title)}</h1>
    {auto_links}
    {page.body_html}
  </article>

  <aside class="sidebar">
    <section class="side-card">
      <h2>タグ</h2>
      <div class="tags">
        {render_tags(page.tags, from_subdir=True)}
      </div>
      {tag_note_html}
    </section>

    <section class="side-card">
      <h2>リンク</h2>
      <ul class="side-links">
        {render_sidebar_nav(prefix="../")}
      </ul>
    </section>

    <section class="side-card">
      <h2>外部リンク</h2>
      <ul class="side-links">
        {render_external_links()}
      </ul>
    </section>
  </aside>
</main>
"""
    return html_document(page.full_title, body, css_href="../style.css", include_mathjax=page.needs_mathjax)


def render_tag_page(tag: str, pages: list[ExplanationPage], knowledge_pages: list[KnowledgePage]) -> str:
    knowledge_items = "".join(
        f'<li><a href="../{escape(k.url)}">{escape(k.title)}</a>'
        f'{f"<div class=\"knowledge-summary\">{escape(k.summary)}</div>" if k.summary else ""}</li>'
        for k in sorted(knowledge_pages, key=knowledge_sort_key)
    )
    if not knowledge_items:
        knowledge_items = '<li class="muted">対応する知識解説はまだありません。</li>'

    explanation_items = "".join(
        f'<li><a href="../{escape(p.url)}">{escape(p.full_title)}</a></li>'
        for p in sorted(pages, key=tag_explanation_sort_key)
    )
    if not explanation_items:
        explanation_items = '<li class="muted">このタグを使う問題解説はまだありません。</li>'

    inner = f"""
<h1>タグ: {escape(tag)}</h1>

<section class="tag-section">
  <h2>知識解説</h2>
  <ul class="plain-list">
    {knowledge_items}
  </ul>
</section>

<section class="tag-section">
  <h2>問題解説</h2>
  <p class="lead">ABCのA問題から順、ARCのA問題から順、AGCのA問題から順、その他の順で表示します。</p>
  <ul class="plain-list">
    {explanation_items}
  </ul>
</section>
"""
    return render_index_layout(f"タグ: {tag}", inner, prefix="../", css_href="../style.css")


def render_knowledge_page(
    page: KnowledgePage,
    related_explanations: list[ExplanationPage],
    related_knowledge: list[KnowledgePage],
) -> str:
    related_explanation_items = "".join(
        f'<li><a href="../{escape(p.url)}">{escape(p.full_title)}</a></li>'
        for p in sorted(related_explanations, key=page_sort_key)
    ) or '<li class="muted">なし</li>'

    related_knowledge_items = "".join(
        f'<li><a href="../{escape(k.url)}">{escape(k.title)}</a></li>'
        for k in sorted(related_knowledge, key=knowledge_sort_key)
    ) or '<li class="muted">なし</li>'

    body = f"""
{site_header_compact(prefix="../")}
<main class="page-layout">
  <article class="markdown-body">
    <h1>{escape(page.title)}</h1>
    {page.body_html}
  </article>

  <aside class="sidebar">
    <section class="side-card">
      <h2>知識情報</h2>
      <p><strong>分野:</strong> {escape(page.category)}</p>
      <p><strong>難易度:</strong> {escape(page.level)}</p>
    </section>

    <section class="side-card">
      <h2>関連知識</h2>
      <ul class="side-links">
        {related_knowledge_items}
      </ul>
    </section>

    <section class="side-card">
      <h2>この知識を使う解説</h2>
      <ul class="side-links">
        {related_explanation_items}
      </ul>
    </section>

    <section class="side-card">
      <h2>リンク</h2>
      <ul class="side-links">
        {render_sidebar_nav(prefix="../")}
      </ul>
    </section>

    <section class="side-card">
      <h2>外部リンク</h2>
      <ul class="side-links">
        {render_external_links()}
      </ul>
    </section>
  </aside>
</main>
"""
    return html_document(page.title, body, css_href="../style.css", include_mathjax=page.needs_mathjax)

def render_knowledge_item(page: KnowledgePage, *, link_prefix: str = "") -> str:
    summary_html = f'<div class="knowledge-summary">{escape(page.summary)}</div>' if page.summary else ""
    href = f"{link_prefix}{quote(Path(page.url).name)}"
    return f"""
<li class="knowledge-entry">
  <a class="knowledge-title" href="{href}">{escape(page.title)}</a>
  {summary_html}
</li>
"""


def render_knowledge_grouped_page(
    title: str,
    knowledge_pages: list[KnowledgePage],
    *,
    group_by: str,
) -> str:
    knowledge_pages = visible_knowledge_pages(knowledge_pages)
    primary_groups: dict[str, dict[str, list[KnowledgePage]]] = defaultdict(lambda: defaultdict(list))

    for p in knowledge_pages:
        if group_by == "category":
            primary = p.category
            secondary = p.level
        elif group_by == "level":
            primary = p.level
            secondary = p.category
        else:
            raise ValueError(f"unknown group_by: {group_by}")
        primary_groups[primary][secondary].append(p)

    primary_key = category_sort_key if group_by == "category" else level_sort_key
    secondary_key = level_sort_key if group_by == "category" else category_sort_key

    chunks = []
    for primary in sorted(primary_groups.keys(), key=primary_key):
        subchunks = []
        for secondary in sorted(primary_groups[primary].keys(), key=secondary_key):
            item_key = category_item_sort_key if group_by == "category" else level_item_sort_key
            items = "".join(
                render_knowledge_item(p)
                for p in sorted(primary_groups[primary][secondary], key=item_key)
            )
            subchunks.append(f"""
<section class="sub-index-group">
  <h3>{escape(secondary)}</h3>
  <ul class="knowledge-list">{items}</ul>
</section>
""")

        chunks.append(f"""
<section class="index-group" id="{slug_id(primary)}">
  <h2>{escape(primary)}</h2>
  <div class="sub-group-list">
    {''.join(subchunks)}
  </div>
</section>
""")

    note = "分野ごとにまとめ、その中で難易度別に分類しています。" if group_by == "category" else "難易度ごとにまとめ、その中で分野別に分類しています。"

    inner = f"""
<h1>{escape(title)}</h1>
<p class="lead">{escape(note)}</p>
<div class="group-list">
  {''.join(chunks) if chunks else '<p class="muted">まだ知識ページがありません。</p>'}
</div>
"""
    return render_index_layout(title, inner, current=title, prefix="../", css_href="../style.css")


def render_knowledge_filtered_page(
    title: str,
    knowledge_pages: list[KnowledgePage],
    *,
    group_by: str,
    value: str,
) -> str:
    visible = visible_knowledge_pages(knowledge_pages)
    if group_by == "category":
        pages = [p for p in visible if p.category == value]
        secondary_label = "難易度"
        secondary_getter = lambda p: p.level
        secondary_key = level_sort_key
        item_key = category_item_sort_key
        all_href = "../by-category.html"
        note = f"分野「{value}」の知識だけを表示しています。"
    elif group_by == "level":
        pages = [p for p in visible if p.level == value]
        secondary_label = "分野"
        secondary_getter = lambda p: p.category
        secondary_key = category_sort_key
        item_key = level_item_sort_key
        all_href = "../by-level.html"
        note = f"難易度「{value}」の知識だけを表示しています。"
    else:
        raise ValueError(f"unknown group_by: {group_by}")

    groups: dict[str, list[KnowledgePage]] = defaultdict(list)
    for p in pages:
        groups[secondary_getter(p)].append(p)

    chunks = []
    for secondary in sorted(groups.keys(), key=secondary_key):
        items = "".join(
            render_knowledge_item(p, link_prefix="../")
            for p in sorted(groups[secondary], key=item_key)
        )
        chunks.append(f"""
<section class="sub-index-group">
  <h2>{escape(secondary)}</h2>
  <ul class="knowledge-list">{items}</ul>
</section>
""")

    inner = f"""
<h1>{escape(title)}</h1>
<p class="lead">{escape(note)}</p>
<p class="config-note"><a href="{all_href}">一覧ページへ戻る</a></p>
<div class="group-list">
  {''.join(chunks) if chunks else '<p class="muted">まだ知識ページがありません。</p>'}
</div>
"""
    return render_index_layout(title, inner, current="", prefix="../../", css_href="../../style.css")


def render_knowledge_index(knowledge_pages: list[KnowledgePage]) -> str:
    visible_knowledge = visible_knowledge_pages(knowledge_pages)
    inner = f"""
<h1>知識集</h1>
<div class="portal-grid">
  <a class="portal-card" href="by-category.html">
    <span class="portal-title">知識集（分野別）</span>
    <span class="portal-count">{len(set(p.category for p in visible_knowledge))}項目</span>
  </a>
  <a class="portal-card" href="by-level.html">
    <span class="portal-title">知識集（難易度別）</span>
    <span class="portal-count">{len(set(p.level for p in visible_knowledge))}項目</span>
  </a>
</div>
"""
    return render_index_layout("知識集", inner, prefix="../", css_href="../style.css")



def validate_project_euler_page(page: ExplanationPage) -> None:
    if classify_page(page) != "Project Euler（100番まで）":
        return
    m = re.search(r"\d+", str(page.problem))
    if not m:
        raise ValueError(f"{page.source_path}: Project Euler は problem に 1 以上 100 以下の番号を含めてください")
    number = int(m.group(0))
    if not (1 <= number <= 100):
        raise ValueError(f"{page.source_path}: Project Euler は100番までのみ掲載対象です")


def load_explanations(src_dir: Path) -> list[ExplanationPage]:
    pages = []
    for path in sorted(src_dir.glob("**/*.md")):
        meta, body_md = parse_front_matter(path.read_text(encoding="utf-8"), path)
        require_keys(meta, ["contest", "problem", "problem_title", "problem_url", "submission_url", "tags"], path)
        body_md_for_html, assets = rewrite_markdown_image_links(
            body_md,
            source_path=path,
            source_root=src_dir,
            output_subdir="explanations",
        )
        body_html = markdown_to_html(body_md_for_html)
        body_text = markdown_to_plain_text(body_md)
        url = f"explanations/{explanation_filename(str(meta['contest']), str(meta['problem']))}"
        page = ExplanationPage(path, meta, body_md, body_html, body_text, url, assets)
        validate_project_euler_page(page)
        pages.append(page)
    return pages


def load_knowledge(src_dir: Path) -> list[KnowledgePage]:
    pages = []
    if not src_dir.exists():
        return pages
    for path in sorted(src_dir.glob("**/*.md")):
        meta, body_md = parse_front_matter(path.read_text(encoding="utf-8"), path)
        require_keys(meta, ["title"], path)
        body_md_for_html, assets = rewrite_markdown_image_links(
            body_md,
            source_path=path,
            source_root=src_dir,
            output_subdir="knowledge",
        )
        body_html = markdown_to_html(body_md_for_html)
        body_text = markdown_to_plain_text(body_md)
        url = f"knowledge/{knowledge_filename(path)}"
        pages.append(KnowledgePage(path, meta, body_md, body_html, body_text, url, assets))
    return pages


def clean_output(out_dir: Path) -> None:
    out_dir.mkdir(parents=True, exist_ok=True)
    for child in out_dir.iterdir():
        if child.is_dir():
            shutil.rmtree(child)
        else:
            child.unlink()


def copy_page_assets(pages: list[ExplanationPage] | list[KnowledgePage], out_dir: Path) -> None:
    copied: set[tuple[Path, Path]] = set()
    for page in pages:
        for asset in page.assets:
            if not asset.source_path.exists():
                print(f"WARNING: 画像ファイルが見つかりません: {page.source_path}: {asset.source_path}", file=sys.stderr)
                continue
            dst = out_dir / asset.output_path
            key = (asset.source_path, dst)
            if key in copied:
                continue
            dst.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(asset.source_path, dst)
            copied.add(key)



def write_css(out_dir: Path) -> None:
    table_color_variables = "\n".join(
        f"  --table-text-{name}: {value};" for name, value in TABLE_TEXT_COLORS.items()
    )
    table_color_rules = "\n".join(
        (
            f".markdown-body .table-text-{name},\n"
            f".markdown-body .table-text-{name} a {{ color: var(--table-text-{name}); }}"
        )
        for name in TABLE_TEXT_COLORS
    )
    css = """
:root {
  --bg: #f5f8fc;
  --panel: #ffffff;
  --fg: #172033;
  --muted: #64748b;
  --subtle: #8290a3;
  --faint: #b8d0ee;
  --border: #cbd8e8;
  --header: #16233a;
  --header-subtle: #223553;
  --accent: #2f80ed;
  --accent-2: #14b8a6;
  --tag-bg: #e9f3ff;
  --tag-border: #bfdbfe;
__TABLE_TEXT_COLOR_VARIABLES__
}

* { box-sizing: border-box; }

body {
  margin: 0;
  background:
    radial-gradient(circle at 20% 0%, rgba(47,128,237,.10), transparent 32%),
    linear-gradient(180deg, #eef5ff 0%, var(--bg) 280px);
  color: var(--fg);
  font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", "Noto Sans JP", Helvetica, Arial, sans-serif;
  line-height: 1.75;
}

a {
  color: var(--accent);
  text-decoration: none;
}

a:hover {
  text-decoration: underline;
}

.site-header {
  background: linear-gradient(135deg, var(--header), var(--header-subtle));
  color: #f8fafc;
  border-bottom: 1px solid rgba(255,255,255,.12);
  box-shadow: 0 10px 28px rgba(22,35,58,.16);
}

.site-header-inner {
  max-width: 1180px;
  margin: 0 auto;
  padding: 16px 24px;
  display: flex;
  gap: 18px;
  align-items: center;
  justify-content: space-between;
  flex-wrap: wrap;
}

.site-title {
  font-weight: 900;
  color: #f8fafc;
  letter-spacing: .02em;
}

.site-title:hover {
  text-decoration: none;
}

.site-nav {
  display: flex;
  gap: 8px;
  flex-wrap: wrap;
  font-size: 0.93rem;
}

.site-nav a {
  color: #f8fafc;
  padding: 4px 10px;
  border-radius: 999px;
  background: rgba(255,255,255,.08);
}

.site-nav a:hover {
  background: rgba(255,255,255,.14);
  text-decoration: none;
}

.site-nav a.active {
  background: rgba(47,128,237,.32);
  border: 1px solid rgba(255,255,255,.22);
}

.container {
  max-width: 1180px;
  margin: 0 auto;
  padding: 30px 24px 36px;
}

.page-layout {
  max-width: 1180px;
  margin: 0 auto;
  padding: 30px 24px 36px;
  display: grid;
  grid-template-columns: minmax(0, 1fr) 280px;
  gap: 28px;
}

.main-panel,
.markdown-body {
  min-width: 0;
}

.main-panel,
.markdown-body {
  background: var(--panel);
  border: 1px solid var(--border);
  border-radius: 18px;
  padding: 24px 26px;
  box-shadow: 0 18px 45px rgba(22,35,58,.08);
}

.main-panel h1,
.markdown-body h1 {
  margin: 0 0 22px;
  padding-bottom: 14px;
  border-bottom: 1px solid var(--border);
  position: relative;
  font-size: 2rem;
  line-height: 1.3;
}

.main-panel h1::after,
.markdown-body h1::after {
  content: "";
  position: absolute;
  left: 0;
  bottom: -2px;
  width: 88px;
  height: 4px;
  border-radius: 999px;
  background: linear-gradient(90deg, var(--accent), var(--accent-2));
}

.markdown-body h2 {
  font-size: 1.42rem;
  padding-bottom: .28em;
  border-bottom: 1px solid var(--border);
  margin-top: 32px;
}

.markdown-body h3 {
  margin-top: 24px;
}

.markdown-body code {
  background: rgba(47,128,237,.10);
  padding: .2em .4em;
  border-radius: 6px;
  font-family: ui-monospace, SFMono-Regular, Consolas, "Liberation Mono", Menlo, monospace;
  font-size: 85%;
}

.markdown-body pre {
  background: #f8fbff;
  border: 1px solid var(--border);
  border-radius: 12px;
  overflow-x: auto;
  padding: 16px;
}

.markdown-body pre code {
  background: transparent;
  padding: 0;
}

.markdown-body img {
  max-width: 100%;
  height: auto;
  display: block;
  margin: 16px 0;
  border: 1px solid var(--border);
  border-radius: 12px;
  background: #fff;
}

.markdown-body table {
  width: max-content;
  min-width: 100%;
  border-collapse: separate;
  border-spacing: 0;
  margin: 16px 0;
  border: 1px solid var(--border);
  border-radius: 12px;
  overflow: hidden;
  background: #fff;
}

.markdown-body th,
.markdown-body td {
  border-right: 1px solid var(--border);
  border-bottom: 1px solid var(--border);
  padding: 8px 10px;
  vertical-align: top;
  white-space: nowrap;
}

.markdown-body tr > *:last-child {
  border-right: 0;
}

.markdown-body tbody tr:last-child > *,
.markdown-body thead tr:last-child > * {
  border-bottom: 0;
}

.markdown-body th {
  background: #eef5ff;
  color: #1f3b65;
  font-weight: 700;
  text-align: center;
}

/* 表セル先頭の `{red}` などを文字色へ変換する。 */
__TABLE_TEXT_COLOR_RULES__

.markdown-table-scroll {
  overflow-x: auto;
  margin: 16px 0;
}

.markdown-table-scroll table {
  margin: 0;
}

.math-display, mjx-container[display="true"] {
  overflow-x: auto;
  overflow-y: hidden;
  padding: 0.35em 0;
}


.auto-links {
  display: flex;
  flex-wrap: wrap;
  gap: 10px;
  margin: -8px 0 22px;
}

.auto-links a {
  display: inline-block;
  border: 1px solid var(--border);
  border-radius: 999px;
  background: #f8fbff;
  padding: 4px 12px;
  font-weight: 700;
  box-shadow: 0 4px 12px rgba(22,35,58,.05);
}

.auto-links a:hover {
  text-decoration: none;
  border-color: rgba(47,128,237,.65);
  background: var(--tag-bg);
}

.lead {
  color: var(--muted);
  margin-top: -8px;
}

.portal-section {
  margin-top: 28px;
}

.portal-section h2 {
  font-size: 1.18rem;
  margin: 0 0 12px;
  padding-left: 10px;
  border-left: 4px solid var(--accent);
}

.portal-grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(210px, 1fr));
  gap: 14px;
  margin-top: 16px;
}

.portal-card {
  display: block;
  border: 1px solid var(--border);
  border-radius: 14px;
  padding: 15px 16px;
  color: var(--fg);
  background: #fff;
  box-shadow: 0 8px 22px rgba(22,35,58,.07);
  transition: transform .12s ease, border-color .12s ease, box-shadow .12s ease;
}

.portal-card:hover {
  transform: translateY(-2px);
  text-decoration: none;
  border-color: rgba(47,128,237,.65);
  box-shadow: 0 12px 28px rgba(47,128,237,.13);
}

.portal-title {
  display: block;
  font-size: 1.08rem;
  font-weight: 800;
}

.portal-count {
  display: block;
  color: var(--muted);
  margin-top: 4px;
  font-size: .92rem;
}

.sidebar {
  display: flex;
  flex-direction: column;
  gap: 16px;
  margin-top: 82px;
}

.side-card {
  border: 1px solid var(--border);
  border-radius: 14px;
  background: #fff;
  padding: 15px 16px;
  box-shadow: 0 8px 22px rgba(22,35,58,.07);
}

.side-card h2 {
  font-size: 1rem;
  margin: 0 0 10px;
  color: #1d4ed8;
}

.side-card p {
  margin: 0;
}

.tags {
  display: flex;
  flex-wrap: wrap;
  gap: 6px;
}

.tag {
  display: inline-block;
  border: 1px solid var(--tag-border);
  border-radius: 999px;
  background: var(--tag-bg);
  padding: 2px 10px;
  color: var(--fg);
  font-size: 0.88rem;
}

.tag:hover {
  text-decoration: none;
  border-color: rgba(47,128,237,.75);
}

.tag-note {
  color: var(--muted);
  font-size: 0.9rem;
  margin: 10px 0 0;
}

.side-links {
  padding-left: 1.2em;
  margin: 0;
}

.side-links li {
  margin: 4px 0;
}

.side-nav-group > a,
.side-nav-group > span {
  font-weight: 700;
}

.side-sub-links {
  margin: 4px 0 8px;
  padding-left: 1.1em;
}

.side-sub-links li {
  margin: 2px 0;
}

.external-link-item {
  margin: 6px 0;
}

.table-scroll {
  overflow-x: auto;
}

.contest-table {
  width: 100%;
  min-width: 760px;
  border-collapse: separate;
  border-spacing: 0;
  table-layout: fixed;
  overflow: hidden;
  border: 1px solid var(--border);
  border-radius: 14px;
  background: #fff;
}

.contest-table th,
.contest-table td {
  border-right: 1px solid var(--border);
  border-bottom: 1px solid var(--border);
  padding: 8px;
  vertical-align: top;
}

.contest-table tr > *:last-child {
  border-right: 0;
}

.contest-table tbody tr:last-child > * {
  border-bottom: 0;
}

.contest-table thead th {
  background: #eef5ff;
  text-align: center;
  color: #1f3b65;
}

.contest-table tbody th {
  width: 92px;
  background: #f8fbff;
  white-space: nowrap;
}

.contest-table td {
  min-width: 108px;
  height: 58px;
}

.contest-table td.empty {
  color: var(--muted);
  text-align: center;
  vertical-align: middle;
}

.problem-cell {
  display: grid;
  min-width: 0;
  height: 100%;
  min-height: 42px;
  grid-template-rows: auto auto auto;
  align-content: space-between;
  gap: 2px;
  color: var(--fg);
}

.problem-cell:hover {
  color: var(--accent);
}

.problem-code {
  display: block;
  min-width: 0;
  align-self: start;
  justify-self: start;
  font-size: 0.68rem;
  font-weight: 700;
  line-height: 1.15;
  letter-spacing: 0.02em;
  color: var(--faint);
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
}

.problem-title {
  display: block;
  min-width: 0;
  align-self: center;
  justify-self: center;
  max-width: 100%;
  font-size: 0.82rem;
  font-weight: 400;
  line-height: 1.2;
  text-align: center;
  overflow: hidden;
  white-space: nowrap;
  text-overflow: ellipsis;
}

.problem-title-ja {
  display: block;
  min-width: 0;
  width: 100%;
  align-self: end;
  justify-self: stretch;
  max-width: 100%;
  color: var(--faint);
  font-size: 0.70rem;
  font-weight: 600;
  line-height: 1.15;
  text-align: right;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
  padding-inline: 3px;
  box-sizing: border-box;
}


.education-card-grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(180px, 1fr));
  gap: 8px;
}

.educational-problem-card {
  min-height: 72px;
  padding: 8px;
  border: 1px solid var(--border);
  border-radius: 12px;
  background: #fff;
  box-shadow: 0 6px 16px rgba(22,35,58,.04);
}

.educational-problem-card:hover {
  border-color: #bcd7ff;
  box-shadow: 0 8px 20px rgba(37,99,235,.08);
}

.plain-list {
  padding-left: 1.25em;
}

.plain-list li {
  margin: 6px 0;
}

.tag-section {
  margin-top: 26px;
}

.tag-section h2 {
  font-size: 1.24rem;
  margin-bottom: 10px;
}

.inline-tags {
  margin-left: 8px;
}

.inline-tags .tag {
  font-size: 0.8rem;
  padding: 1px 8px;
}

.group-list {
  display: grid;
  gap: 16px;
}

.index-group {
  border: 1px solid var(--border);
  border-radius: 14px;
  padding: 14px 16px;
  background: #fff;
  box-shadow: 0 8px 22px rgba(22,35,58,.05);
}

.index-group h2 {
  margin-top: 0;
  border-left: 4px solid var(--accent);
  padding-left: 10px;
}

.sub-group-list {
  display: grid;
  gap: 12px;
}

.sub-index-group {
  border-top: 1px solid var(--border);
  padding-top: 10px;
}

.sub-index-group:first-child {
  border-top: 0;
  padding-top: 0;
}

.sub-index-group h3 {
  margin: 0 0 8px;
  font-size: 1.05rem;
}

.knowledge-list {
  list-style: none;
  padding-left: 0;
  margin: 0;
}

.knowledge-entry {
  margin: 8px 0;
}

.knowledge-title {
  font-weight: 700;
}

.knowledge-summary {
  color: var(--muted);
  font-size: 0.92rem;
  margin-top: 2px;
}

.config-note {
  color: var(--muted);
  font-size: 0.92rem;
}

.muted {
  color: var(--muted);
}

@media (max-width: 860px) {
  body {
    background: var(--bg);
  }

  .site-header-inner {
    align-items: flex-start;
  }

  .page-layout {
    grid-template-columns: 1fr;
    padding: 18px 14px 28px;
  }

  .container {
    padding: 18px 14px 28px;
  }

  .main-panel,
  .markdown-body {
    padding: 18px 16px;
    border-radius: 14px;
  }

  .main-panel h1,
  .markdown-body h1 {
    font-size: 1.65rem;
  }

  .sidebar {
    margin-top: 0;
    order: initial;
  }

  .contest-table {
    min-width: 720px;
    table-layout: fixed;
  }
}
"""
    css = css.replace("__TABLE_TEXT_COLOR_VARIABLES__", table_color_variables)
    css = css.replace("__TABLE_TEXT_COLOR_RULES__", table_color_rules)
    (out_dir / "style.css").write_text(css.lstrip(), encoding="utf-8")



def build(explanations_dir: Path, knowledge_dir: Path, out_dir: Path) -> None:
    clean_output(out_dir)

    for sub in ["explanations", "tags", "contests", "knowledge", "knowledge/categories", "knowledge/levels"]:
        (out_dir / sub).mkdir(parents=True, exist_ok=True)

    explanations = load_explanations(explanations_dir)
    knowledge_pages = load_knowledge(knowledge_dir)
    copy_page_assets(explanations, out_dir)
    copy_page_assets(knowledge_pages, out_dir)

    global SIDEBAR_KNOWLEDGE_PAGES, TAG_SLUG_MAP
    SIDEBAR_KNOWLEDGE_PAGES = knowledge_pages

    tag_map: dict[str, list[ExplanationPage]] = defaultdict(list)
    knowledge_tag_map: dict[str, list[KnowledgePage]] = defaultdict(list)
    pages_by_class: dict[str, list[ExplanationPage]] = defaultdict(list)

    for p in explanations:
        pages_by_class[classify_page(p)].append(p)
        for tag in p.tags:
            tag_map[tag].append(p)

    visible_knowledge = visible_knowledge_pages(knowledge_pages)
    explanation_tag_names = set(tag_map)
    warn_missing_knowledge_pages_for_tags(explanation_tag_names, knowledge_pages)
    for k in visible_knowledge:
        # 知識記事自身の title は常にタグページを作る。
        knowledge_tag_map[k.title].append(k)
        # aliases / absorbs は、問題解説側で実際にタグとして使われている場合だけ
        # タグページを作る。検索用 alias だけで空のタグページが増えるのを防ぐ。
        for key in [*k.aliases, *k.absorbs]:
            if key in explanation_tag_names:
                knowledge_tag_map[key].append(k)

    all_tag_names = sorted(explanation_tag_names | set(knowledge_tag_map), key=str)
    TAG_SLUG_MAP = make_tag_slug_map(all_tag_names, visible_knowledge)

    for p in explanations:
        (out_dir / p.url).write_text(render_explanation_page(p), encoding="utf-8")

    knowledge_key_map = make_knowledge_key_map(knowledge_pages)
    missing_related_by_page: dict[Path, list[str]] = {}

    for k in knowledge_pages:
        related_pages: dict[Path, ExplanationPage] = {}
        for key in k.tag_keys:
            for related in tag_map.get(key, []):
                related_pages[related.source_path] = related
        related_knowledge_pages, missing_related = resolve_related_knowledge_pages(k, knowledge_key_map)
        if missing_related:
            missing_related_by_page[k.source_path] = missing_related
        (out_dir / k.url).write_text(
            render_knowledge_page(k, list(related_pages.values()), related_knowledge_pages),
            encoding="utf-8",
        )

    warn_missing_related_knowledge_pages(missing_related_by_page)

    (out_dir / "index.html").write_text(render_portal(pages_by_class, knowledge_pages), encoding="utf-8")
    (out_dir / "contests" / "index.html").write_text(render_explanations_index(pages_by_class), encoding="utf-8")

    for spec in load_contest_views():
        class_id = str(spec["id"])
        title = str(spec["title"])
        path = str(spec["path"])
        description = str(spec.get("description", ""))
        pages = pages_by_class.get(class_id, [])
        layout = str(spec["layout"])
        order = str(spec["order"])
        if layout == "table":
            html = render_table_page(title, pages, list(spec["columns"]), order=order, description=description)
        elif layout in {"grouped_table", "grouped_list"}:
            html = render_grouped_table_page(
                title,
                pages,
                order=order,
                groups=list(spec.get("groups", [])),
                description=description,
            )
        else:
            html = render_list_page(title, pages, order=order, description=description)
        (out_dir / "contests" / path).write_text(html, encoding="utf-8")

    for tag in all_tag_names:
        (out_dir / "tags" / tag_filename(tag)).write_text(
            render_tag_page(tag, tag_map.get(tag, []), knowledge_tag_map.get(tag, [])),
            encoding="utf-8",
        )

    (out_dir / "knowledge" / "index.html").write_text(render_knowledge_index(knowledge_pages), encoding="utf-8")
    (out_dir / "knowledge" / "by-category.html").write_text(
        render_knowledge_grouped_page("知識集（分野別）", knowledge_pages, group_by="category"),
        encoding="utf-8",
    )
    (out_dir / "knowledge" / "by-level.html").write_text(
        render_knowledge_grouped_page("知識集（難易度別）", knowledge_pages, group_by="level"),
        encoding="utf-8",
    )

    visible_knowledge = visible_knowledge_pages(knowledge_pages)
    for category in sorted({p.category for p in visible_knowledge}, key=category_sort_key):
        (out_dir / "knowledge" / "categories" / category_page_filename(category)).write_text(
            render_knowledge_filtered_page(f"知識集（{category}）", knowledge_pages, group_by="category", value=category),
            encoding="utf-8",
        )
    for level in sorted({p.level for p in visible_knowledge}, key=level_sort_key):
        (out_dir / "knowledge" / "levels" / level_page_filename(level)).write_text(
            render_knowledge_filtered_page(f"知識集（{level}）", knowledge_pages, group_by="level", value=level),
            encoding="utf-8",
        )

    (out_dir / ".nojekyll").write_text("", encoding="utf-8")
    write_css(out_dir)

    print(f"generated explanations: {len(explanations)}")
    print(f"generated knowledge pages: {len(knowledge_pages)}")
    print(f"output: {out_dir}")


def main() -> None:
    parser = argparse.ArgumentParser(description="競プロ解説 Markdown から静的HTMLサイトを生成します。")
    parser.add_argument("--explanations", type=Path, default=DEFAULT_EXPLANATIONS, help="入力解説Markdownディレクトリ")
    parser.add_argument("--knowledge", type=Path, default=DEFAULT_KNOWLEDGE, help="入力知識Markdownディレクトリ")
    parser.add_argument("--out", type=Path, default=DEFAULT_OUT, help="出力ディレクトリ")
    args = parser.parse_args()
    build(args.explanations, args.knowledge, args.out)


if __name__ == "__main__":
    main()
