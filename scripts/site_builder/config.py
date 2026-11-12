from __future__ import annotations

from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

SITE_TITLE = "競技プログラミング 知識集積所"

EXTERNAL_LINKS = [
    ("AtCoder", "https://atcoder.jp/home"),
    ("AtCoder Problems", "https://kenkoooo.com/atcoder/"),
    ("AtCoder NoviSteps", "https://atcoder-novisteps.vercel.app/"),
    ("Aizu Online Judge", "https://judge.u-aizu.ac.jp/onlinejudge/"),
    ("Project Euler", "https://projecteuler.net/"),
    ("だいくすとら氏によるグラフ描画ツール", "https://graph-editor.daikusutora3.workers.dev/"),
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

DEFAULT_EXPLANATIONS = ROOT / "explanations"

DEFAULT_KNOWLEDGE = ROOT / "knowledge"

DEFAULT_OUT = ROOT / "docs"

DEFAULT_CONFIG = ROOT / "site_config.toml"

IMAGE_EXTENSIONS = {".png", ".jpg", ".jpeg", ".gif", ".svg", ".webp", ".avif"}

TABLE_CELL_BACKGROUNDS = {
    "red": "#fee2e2",
    "coral": "#ffe4e1",
    "orange": "#ffedd5",
    "amber": "#fef3c7",
    "yellow": "#fef9c3",
    "olive": "#eef0c8",
    "lime": "#ecfccb",
    "green": "#dcfce7",
    "emerald": "#d1fae5",
    "teal": "#ccfbf1",
    "cyan": "#cffafe",
    "blue": "#dbeafe",
    "navy": "#dbe4f0",
    "indigo": "#e0e7ff",
    "violet": "#ede9fe",
    "purple": "#f3e8ff",
    "magenta": "#fae8ff",
    "rose": "#ffe4e6",
    "pink": "#fce7f3",
    "brown": "#f5e6d3",
    "gray": "#e2e8f0",
    "black": "#cbd5e1",
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
