from __future__ import annotations

from collections import defaultdict
from html import escape
from pathlib import Path
from urllib.parse import quote
import re

from .config import EXTERNAL_LINKS, SITE_TITLE
from .knowledge import (
    category_sort_key, level_sort_key, visible_knowledge_pages,
    link_knowledge_references_in_html,
)
from .markdown import add_heading_anchors_and_build_toc
from .models import ExplanationPage, KnowledgePage
from .navigation import (
    category_item_sort_key, category_page_href, contest_sort_key, contest_url_from_pages,
    educational_contest_group, educational_group_sort_key, is_external_href, knowledge_sort_key,
    level_item_sort_key, level_page_href, load_contest_views, page_sort_key, render_problem_card_link,
    slug_id, sort_pages_for_view, tag_explanation_sort_key, tag_href,
)

SIDEBAR_KNOWLEDGE_PAGES: list[KnowledgePage] = []

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


def render_explanation_page(
    page: ExplanationPage,
    knowledge_title_map: dict[str, KnowledgePage],
) -> str:
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

    linked_body_html = link_knowledge_references_in_html(
        page.body_html,
        current_source_path=page.source_path,
        title_map=knowledge_title_map,
        href_prefix="../knowledge/",
    )
    linked_body_html, toc_html = add_heading_anchors_and_build_toc(linked_body_html)

    body = f"""
{site_header_compact(prefix="../")}
<main class="page-layout">
  <article class="markdown-body">
    <div class="explanation-heading">
      <h1>{escape(page.full_title)}</h1>
      {f'<p class="explanation-title-ja">{escape(page.problem_title_ja)}</p>' if page.problem_title_ja else ''}
    </div>
    {auto_links}
    {toc_html}
    {linked_body_html}
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
    return render_index_layout(
        f"タグ: {tag}",
        inner,
        prefix="../",
        css_href="../style.css",
        include_mathjax=any(k.summary_needs_mathjax for k in knowledge_pages),
    )


def render_knowledge_page(
    page: KnowledgePage,
    related_explanations: list[ExplanationPage],
    related_knowledge: list[KnowledgePage],
    knowledge_title_map: dict[str, KnowledgePage],
) -> str:
    related_explanation_items = "\n        ".join(
        f'<li><a href="../{escape(p.url)}">{escape(p.full_title)}</a></li>'
        for p in sorted(related_explanations, key=page_sort_key)
    ) or '<li class="muted">なし</li>'

    related_knowledge_items = "".join(
        f'<li><a href="../{escape(k.url)}">{escape(k.title)}</a></li>'
        for k in sorted(related_knowledge, key=knowledge_sort_key)
    ) or '<li class="muted">なし</li>'

    linked_body_html = link_knowledge_references_in_html(
        page.body_html,
        current_source_path=page.source_path,
        title_map=knowledge_title_map,
        href_prefix="",
    )
    linked_body_html, toc_html = add_heading_anchors_and_build_toc(linked_body_html)

    body = f"""
{site_header_compact(prefix="../")}
<main class="page-layout">
  <article class="markdown-body">
    <h1>{escape(page.title)}</h1>
    {toc_html}
    {linked_body_html}
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
    return render_index_layout(
        title,
        inner,
        current=title,
        prefix="../",
        css_href="../style.css",
        include_mathjax=any(p.summary_needs_mathjax for p in knowledge_pages),
    )


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
    return render_index_layout(
        title,
        inner,
        current="",
        prefix="../../",
        css_href="../../style.css",
        include_mathjax=any(p.summary_needs_mathjax for p in pages),
    )


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
