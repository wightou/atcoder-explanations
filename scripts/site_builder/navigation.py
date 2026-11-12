from __future__ import annotations

from html import escape
from pathlib import Path
from urllib.parse import quote, urlparse
import hashlib
import re

try:
    import tomllib
except Exception:
    tomllib = None

from .config import (
    DEFAULT_CONFIG, DEFAULT_CONTEST_VIEWS, EDUCATIONAL_CONTEST_ALIASES,
    EDUCATIONAL_CONTEST_GROUPS,
)
from .knowledge import category_sort_key, level_sort_key
from .models import ExplanationPage, KnowledgePage

TAG_SLUG_MAP: dict[str, str] = {}

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
