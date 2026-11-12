from __future__ import annotations

from collections import defaultdict
from pathlib import Path

from . import navigation
from . import render as render_module
from .knowledge import (
    category_sort_key, level_sort_key, visible_knowledge_pages,
    warn_missing_knowledge_pages_for_tags, make_knowledge_key_map, make_knowledge_title_map,
    resolve_related_knowledge_pages, warn_missing_related_knowledge_pages,
)
from .loader import clean_output, copy_page_assets, load_explanations, load_knowledge, warn_unexpected_explanation_files
from .models import ExplanationPage, KnowledgePage
from .navigation import category_page_filename, classify_page, level_page_filename, load_contest_views, make_tag_slug_map, tag_filename
from .render import (
    render_explanation_page, render_explanations_index, render_grouped_table_page, render_knowledge_filtered_page,
    render_knowledge_grouped_page, render_knowledge_index, render_knowledge_page, render_list_page, render_portal,
    render_table_page, render_tag_page,
)
from .styles import write_css

def build(explanations_dir: Path, knowledge_dir: Path, out_dir: Path) -> None:
    warn_unexpected_explanation_files(explanations_dir)
    clean_output(out_dir)

    for sub in ["explanations", "tags", "contests", "knowledge", "knowledge/categories", "knowledge/levels"]:
        (out_dir / sub).mkdir(parents=True, exist_ok=True)

    explanations = load_explanations(explanations_dir)
    knowledge_pages = load_knowledge(knowledge_dir)
    copy_page_assets(explanations, out_dir)
    copy_page_assets(knowledge_pages, out_dir)

    render_module.SIDEBAR_KNOWLEDGE_PAGES = knowledge_pages

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
        # aliases / absorbs 間や title との重複は除外し、同じ知識記事を二重登録しない。
        for key in k.tag_keys:
            if key == k.title:
                continue
            if key in explanation_tag_names:
                knowledge_tag_map[key].append(k)

    all_tag_names = sorted(explanation_tag_names | set(knowledge_tag_map), key=str)
    navigation.TAG_SLUG_MAP = make_tag_slug_map(all_tag_names, visible_knowledge)

    knowledge_key_map = make_knowledge_key_map(knowledge_pages)
    knowledge_title_map = make_knowledge_title_map(knowledge_pages)

    for p in explanations:
        (out_dir / p.url).write_text(
            render_explanation_page(p, knowledge_title_map),
            encoding="utf-8",
        )

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
            render_knowledge_page(
                k,
                list(related_pages.values()),
                related_knowledge_pages,
                knowledge_title_map,
            ),
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
