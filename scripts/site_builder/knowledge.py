from __future__ import annotations

from html import escape, unescape
from pathlib import Path
import re
import sys

from .config import CATEGORY_ORDER, LEVEL_ORDER, ROOT
from .models import KnowledgePage

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


def make_knowledge_title_map(knowledge_pages: list["KnowledgePage"]) -> dict[str, "KnowledgePage"]:
    """知識記事の正式な title からページを引く辞書を作る。

    本文内の自動リンクは、意図しない別名展開を避けるため title 完全一致だけを対象にする。
    通常の導線に出さない未分類ページはリンク候補から外す。
    """
    result: dict[str, KnowledgePage] = {}
    for page in visible_knowledge_pages(knowledge_pages):
        title = page.title.strip()
        if title:
            result.setdefault(title, page)
    return result


def link_knowledge_references_in_html(
    body_html: str,
    *,
    current_source_path: Path | None,
    title_map: dict[str, "KnowledgePage"],
    href_prefix: str,
) -> str:
    """問題解説・知識記事本文の限定された参照表現だけを内部リンクへ変換する。

    対象は次の2種類だけ。
    - `## 関連知識` 節内の、知識記事 title と完全一致する `###` 見出し
    - 本文中で `「XXX」の記事参照。` または `「XXX」「YYY」...の記事参照。` の形になっていて、
      各記事名が title と一致する箇所

    記事参照では鍵括弧より前の文言を問わない。
    aliases / absorbs では解決せず、既存リンクやそれ以外の文章も変更しない。
    """

    def target_for(raw_name: str) -> KnowledgePage | None:
        name = unescape(raw_name).strip()
        target = title_map.get(name)
        if target is None:
            return None
        if current_source_path is not None and target.source_path == current_source_path:
            return None
        return target

    section_pattern = re.compile(
        r'(?P<head><h2\b[^>]*>\s*関連知識\s*</h2>)(?P<body>.*?)(?=(?:<h2\b)|\Z)',
        flags=re.S,
    )
    heading_pattern = re.compile(
        r'<h3(?P<attrs>[^>]*)>(?P<name>[^<>]+)</h3>',
        flags=re.S,
    )

    def replace_section(match: re.Match[str]) -> str:
        def replace_heading(heading_match: re.Match[str]) -> str:
            name_html = heading_match.group("name")
            target = target_for(name_html)
            if target is None:
                return heading_match.group(0)
            href = escape(f"{href_prefix}{Path(target.url).name}", quote=True)
            return (
                f'<h3{heading_match.group("attrs")}>'
                f'<a href="{href}">{name_html}</a>'
                f'</h3>'
            )

        linked_body = heading_pattern.sub(replace_heading, match.group("body"))
        return match.group("head") + linked_body

    body_html = section_pattern.sub(replace_section, body_html)

    # 鍵括弧で囲まれた記事名が1つ以上連続し、その直後が `の記事参照。` なら対象にする。
    # 鍵括弧より前の文言は判定しない。既存リンクやHTMLタグの内部を誤って拾わないよう、
    # 記事名部分にはHTMLタグを許さない。
    reference_pattern = re.compile(
        r'(?P<names>(?:「[^<>「」]+」)+)の記事参照。'
    )
    quoted_name_pattern = re.compile(r'「(?P<name>[^<>「」]+)」')

    def replace_reference(match: re.Match[str]) -> str:
        quoted_names = list(quoted_name_pattern.finditer(match.group("names")))
        if not quoted_names:
            return match.group(0)

        linked_names: list[str] = []
        for quoted_match in quoted_names:
            name_html = quoted_match.group("name")
            target = target_for(name_html)
            if target is None:
                # 従来どおり、定型文中に解決できない記事名が1つでもある場合は
                # 文全体を変更しない。部分的な自動リンクを避ける。
                return match.group(0)
            href = escape(f"{href_prefix}{Path(target.url).name}", quote=True)
            linked_names.append(f'「<a href="{href}">{name_html}</a>」')

        return (
            f'{"".join(linked_names)}'
            f'の記事参照。'
        )

    # 既存リンクやコードの中には新しいリンクを作らない。
    # Markdownから生成されたHTMLをタグ単位に分け、a/code/pre/script/style の外側にある
    # テキストだけを置換する。
    html_parts = re.split(r'(<[^>]+>)', body_html)
    blocked_tags = {"a", "code", "pre", "script", "style"}
    blocked_depth = 0

    for i, part in enumerate(html_parts):
        if not part:
            continue
        if part.startswith("<"):
            closing = re.match(r'</\s*([A-Za-z0-9]+)\b', part)
            if closing and closing.group(1).lower() in blocked_tags:
                blocked_depth = max(0, blocked_depth - 1)
                continue

            opening = re.match(r'<\s*([A-Za-z0-9]+)\b', part)
            if (
                opening
                and opening.group(1).lower() in blocked_tags
                and not part.rstrip().endswith("/>")
            ):
                blocked_depth += 1
            continue

        if blocked_depth == 0:
            html_parts[i] = reference_pattern.sub(replace_reference, part)

    return "".join(html_parts)


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
