#!/usr/bin/env python3
from __future__ import annotations

from dataclasses import dataclass, field
from pathlib import Path
import argparse

ROOT = Path(__file__).resolve().parents[1]
KNOWLEDGE_DIR = ROOT / "knowledge"

CATEGORIES = [
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

LEVELS = [
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

@dataclass(frozen=True)
class KnowledgeStub:
    slug: str
    title: str
    level: str
    category: str
    summary: str = "今後記述予定。"
    category_order: int = 9999
    level_order: int = 9999
    aliases: list[str] = field(default_factory=list)
    absorbs: list[str] = field(default_factory=list)
    related: list[str] = field(default_factory=list)


def k(slug: str, title: str, level: str, category: str, *, aliases=None, absorbs=None, related=None, summary: str = "今後記述予定。", category_order: int = 9999, level_order: int = 9999) -> KnowledgeStub:
    if category not in CATEGORIES:
        raise ValueError(f"unknown category: {category} ({title})")
    if level not in LEVELS:
        raise ValueError(f"unknown level: {level} ({title})")
    return KnowledgeStub(
        slug=slug,
        title=title,
        level=level,
        category=category,
        summary=summary,
        category_order=int(category_order),
        level_order=int(level_order),
        aliases=list(aliases or []),
        absorbs=list(absorbs or []),
        related=list(related or []),
    )

STUBS: list[KnowledgeStub] = [
    # 難易度問わず
    k("input-output", "入出力", "難易度問わず", "基本", aliases=["cin", "cout", "標準入力", "標準出力"], absorbs=["cin", "cout"]),
    k("cpp-code-style", "C++のコードの書き方", "難易度問わず", "基本", aliases=["C++コード", "C++の書き方", "コードの書き方", "C++ coding style"], related=["入出力", "エラー", "テスト"]),
    k("error", "エラー", "難易度問わず", "基本"),
    k("test", "テスト", "難易度問わず", "基本"),
    k("mathematical-expression", "数学的表現", "難易度問わず", "基本"),
    k("constructive-problem", "構築問題", "難易度問わず", "基本"),
    k("ad-hoc-thinking", "考察問題", "難易度問わず", "基本"),
    k("simulation", "シミュレーション", "難易度問わず", "基本", absorbs=["イベントソート"]),
    k("corner-case", "コーナーケース", "難易度問わず", "基本"),
    k("divide-and-conquer", "分割統治法", "難易度問わず", "高速化系"),
    k("number-theory", "整数論", "難易度問わず", "その他数学系"),
    k("trigonometric-ratio", "三角比", "難易度問わず", "幾何学系"),
    k("geometric-vector", "ベクトル（幾何学）", "難易度問わず", "幾何学系"),
    k("distance", "距離", "難易度問わず", "幾何学系", absorbs=["マンハッタン距離", "ユークリッド距離", "チェビシェフ距離", "座標回転"]),
    k("permutation-and-combination", "順列組み合わせ", "難易度問わず", "その他数学系", absorbs=["数え上げ（複雑なもの）"]),
    k("probability", "確率", "難易度問わず", "その他数学系", absorbs=["期待値の線形性"]),

    # A問題相当
    k("int", "int型", "A問題相当", "変数とデータ構造"),
    k("double", "double型", "A問題相当", "変数とデータ構造"),
    k("char", "char型", "A問題相当", "変数とデータ構造"),
    k("string", "string型", "A問題相当", "変数とデータ構造", absorbs=["文字列処理"]),
    k("bool", "bool型", "A問題相当", "変数とデータ構造"),
    k("if-statement", "if分岐", "A問題相当", "基本", absorbs=["比較演算", "論理演算", "else if"]),
    k("while-loop", "whileループ", "A問題相当", "基本"),
    k("for-loop", "forループ", "A問題相当", "基本"),
    k("vector", "vector", "A問題相当", "変数とデータ構造", aliases=["vector（一次元）"], absorbs=["vector（一次元）", "reverse関数"]),
    k("sort", "ソート", "A問題相当", "基本", aliases=["sort"], absorbs=["sort関数", "自力ソート", "バブルソート", "選択ソート", "挿入ソート", "マージソート", "ヒープソート", "カウンティングソート", "バケツソート"]),
    k("arithmetic-operations", "四則演算", "A問題相当", "その他数学系", absorbs=["剰余"]),
    k("maximum-minimum", "最大値・最小値", "A問題相当", "その他数学系", absorbs=["max関数", "min関数"]),
    k("odd-even", "偶奇判定", "A問題相当", "その他数学系"),
    k("multiple-check", "倍数判定", "A問題相当", "その他数学系"),
    k("counting", "数え上げ", "A問題相当", "その他数学系", absorbs=["数え上げ（基本）"]),

    # B問題相当
    k("long-long", "long long型", "B問題相当", "変数とデータ構造"),
    k("pair", "pair型", "B問題相当", "変数とデータ構造"),
    k("tuple", "tuple型", "B問題相当", "変数とデータ構造"),
    k("stack", "stack", "B問題相当", "変数とデータ構造"),
    k("queue", "queue", "B問題相当", "変数とデータ構造"),
    k("deque", "deque", "B問題相当", "変数とデータ構造"),
    k("priority-queue", "priority_queue", "B問題相当", "変数とデータ構造"),
    k("set", "set", "B問題相当", "変数とデータ構造"),
    k("map", "map", "B問題相当", "変数とデータ構造"),
    k("list", "list", "B問題相当", "変数とデータ構造"),
    k("absolute-value", "絶対値", "B問題相当", "その他数学系", aliases=["abs"], absorbs=["abs関数"]),
    k("multidimensional-vector", "多次元vector", "B問題相当", "変数とデータ構造", absorbs=["二次元以上のvector"]),
    k("nested-loop", "多重ループ", "B問題相当", "基本", absorbs=["二重以上のループ"]),
    k("range-for", "範囲for文", "B問題相当", "基本"),
    k("base-n", "N進法", "B問題相当", "その他数学系"),
    k("bit-operation", "ビット演算", "B問題相当", "その他数学系", absorbs=["ビットシフト"]),
    k("brute-force", "全探索", "B問題相当", "データ探索系"),
    k("grid-operation", "グリッド操作", "B問題相当", "基本", absorbs=["グリッド回転"]),
    k("sentinel", "番兵法", "B問題相当", "基本"),

    # C問題相当
    k("reserve", "reserve関数", "C問題相当", "変数とデータ構造"),
    k("complexity-analysis", "計算量の見積もり", "C問題相当", "高速化系"),
    k("difference-update", "差分更新", "C問題相当", "高速化系"),
    k("preprocessing", "前処理", "C問題相当", "高速化系"),
    k("memoization", "メモ化", "C問題相当", "高速化系"),
    k("recursion", "再帰", "C問題相当", "基本"),
    k("memoized-recursion", "メモ化再帰", "C問題相当", "高速化系"),
    k("cumulative-sum", "累積和", "C問題相当", "高速化系"),
    k("difference-array", "階差数列", "C問題相当", "高速化系", aliases=["imos法", "いもす法"], absorbs=["一次元imos法"]),
    k("inclusion-exclusion", "包除原理", "C問題相当", "その他数学系"),
    k("complement", "補集合", "C問題相当", "その他数学系"),
    k("primality-test", "素数判定", "C問題相当", "その他数学系"),
    k("divisor-enumeration", "約数列挙", "C問題相当", "その他数学系"),
    k("gcd", "最大公約数", "C問題相当", "その他数学系"),
    k("lcm", "最小公倍数", "C問題相当", "その他数学系"),
    k("factorial", "階乗", "C問題相当", "その他数学系", aliases=["factorial"], related=["順列組み合わせ"]),
    k("binomial-coefficient", "二項係数", "C問題相当", "その他数学系", aliases=["combination", "nCr", "nCk"], related=["階乗", "順列組み合わせ", "剰余類環", "繰り返し二乗法"]),
    k("extended-euclidean", "拡張ユークリッドの互除法", "D問題相当", "その他数学系"),
    k("binary-exponentiation", "繰り返し二乗法", "C問題相当", "その他数学系"),
    k("binary-search", "二分探索", "C問題相当", "データ探索系", aliases=["binary search", "lower_bound", "upper_bound", "答えで二分探索"], absorbs=["lower_bound関数", "upper_bound関数", "解の二分探索"], related=["計算量の見積もり"]),
    k("inverse-mapping", "逆写像", "C問題相当", "データ探索系", aliases=["inverse mapping", "inverse map", "逆引き"], absorbs=["逆置換"], related=["前処理", "vector", "map"]),
    k("two-pointer-method", "ツーポインタ法", "C問題相当", "データ探索系", aliases=["ツーポインタ", "2ポインタ", "two pointers", "two-pointer method", "two pointer method"], related=["計算量の見積もり", "ソート", "尺取法", "sliding window法"]),
    k("shakutori-method", "尺取法", "C問題相当", "データ探索系", aliases=["しゃくとり法", "尺取り法"], related=["ツーポインタ法", "sliding window法"]),
    k("sliding-window", "sliding window法", "C問題相当", "データ探索系", aliases=["sliding window", "sliding-window法", "スライディングウィンドウ", "スライディングウィンドウ法"], related=["ツーポインタ法", "尺取法"]),
    k("bit-bruteforce", "bit全探索", "C問題相当", "データ探索系"),
    k("permutation-search", "順列全探索", "C問題相当", "データ探索系", aliases=["next_permutation探索"]),
    k("modular-arithmetic", "剰余類環", "C問題相当", "その他数学系", absorbs=["modint"], related=["繰り返し二乗法"]),
    k("run-length-encoding", "ランレングス圧縮", "C問題相当", "高速化系", aliases=["RLE"]),
    k("greedy", "貪欲法", "C問題相当", "データ探索系", aliases=["greedy"], absorbs=["ソートを用いる貪欲法"]),
    k("periodicity", "周期性", "C問題相当", "その他数学系"),
    k("parity", "パリティ", "C問題相当", "その他数学系"),
    k("adjacency-list", "隣接リスト", "C問題相当", "グラフ理論系"),
    k("adjacency-matrix", "隣接行列", "C問題相当", "グラフ理論系"),
    k("bfs", "幅優先探索", "C問題相当", "グラフ理論系", aliases=["BFS"]),
    k("dfs", "深さ優先探索", "C問題相当", "グラフ理論系", aliases=["DFS"], absorbs=["バックトラック"]),
    k("super-vertex", "超頂点", "D問題相当", "グラフ理論系", aliases=["仮想頂点", "ダミー頂点", "super vertex", "virtual vertex", "auxiliary vertex"], absorbs=["超始点", "超終点", "super source", "super sink"], related=["グラフ理論の基礎", "隣接リスト", "幅優先探索", "Dijkstra法", "最短経路問題"]),

    # D問題相当
    k("dynamic-programming", "動的計画法", "D問題相当", "高速化系", aliases=["DP"]),
    k("backtrace", "バックトレース", "D問題相当", "データ探索系", aliases=["経路復元", "解の復元"], related=["動的計画法", "最短経路問題"]),
    k("knapsack-problem", "ナップサック問題", "D問題相当", "典型問題集", aliases=["knapsack", "0/1ナップサック"], related=["動的計画法"]),
    k("interval-scheduling-problem", "区間スケジューリング問題", "C問題相当", "典型問題集", aliases=["interval scheduling"], related=["貪欲法", "ソート"]),
    k("shortest-path-problem", "最短経路問題", "D問題相当", "典型問題集", aliases=["迷路の最短経路", "shortest path"], related=["幅優先探索", "Dijkstra法", "Warshall-Floyd法"]),
    k("connected-components-count", "連結成分数", "C問題相当", "典型問題集", aliases=["連結成分の個数"], related=["幅優先探索", "深さ優先探索", "隣接リスト"]),
    k("tree-dp", "木DP", "D問題相当", "高速化系", related=["動的計画法", "木"]),
    k("bit-dp", "bitDP", "D問題相当", "高速化系", related=["動的計画法", "bit全探索"]),
    k("interval-dp", "区間DP", "D問題相当", "高速化系", related=["動的計画法"]),
    k("digit-dp", "桁DP", "E問題相当", "高速化系", related=["動的計画法"]),
    k("dag-dp", "DAG上のDP", "D問題相当", "高速化系", aliases=["DAG DP", "DAG上DP"], absorbs=["トポロジカルソート"], related=["動的計画法", "グラフ理論の基礎"]),
    k("graph-theory-basics", "グラフ理論の基礎", "D問題相当", "グラフ理論系", aliases=["グラフ理論の基礎（知識としてのみ）"]),
    k("union-find", "UnionFind木", "D問題相当", "変数とデータ構造", aliases=["DSU", "Disjoint Set Union"]),
    k("rooted-tree", "根付き木", "D問題相当", "グラフ理論系"),
    k("tree", "木", "D問題相当", "グラフ理論系", absorbs=["木の探索", "木の直径"]),
    k("binary-lifting", "頂点倍加", "D問題相当", "グラフ理論系", aliases=["ダブリング on tree"]),
    k("bipartite-graph", "二部グラフ", "D問題相当", "グラフ理論系"),
    k("zero-one-bfs", "01最良優先探索", "D問題相当", "グラフ理論系", aliases=["0-1 BFS"]),
    k("dijkstra", "Dijkstra法", "D問題相当", "グラフ理論系", aliases=["ダイクストラ法"]),
    k("a-star", "A*アルゴリズム", "D問題相当", "グラフ理論系", aliases=["A*", "A-star"]),
    k("two-dimensional-cumulative-sum", "二次元累積和", "D問題相当", "高速化系", related=["累積和"]),
    k("two-dimensional-imos", "二次元imos法", "D問題相当", "高速化系", related=["二次元累積和"]),
    k("prime-factorization", "素因数分解", "D問題相当", "その他数学系"),
    k("sieve-of-eratosthenes", "エラトステネスの篩", "D問題相当", "その他数学系"),
    k("segmented-sieve", "区間篩", "D問題相当", "その他数学系", related=["エラトステネスの篩"]),
    k("minimum-spanning-tree", "最小全域木", "D問題相当", "グラフ理論系", aliases=["Kruskal法", "Prim法"], absorbs=["Kruskal法", "Prim法"]),

    # E問題相当
    k("hash", "ハッシュ", "E問題相当", "文字列系"),
    k("rolling-hash", "ローリングハッシュ", "E問題相当", "文字列系", related=["ハッシュ"]),
    k("rerooting-dp", "全方位木DP", "E問題相当", "高速化系", related=["木DP"]),
    k("ternary-search", "三分探索", "E問題相当", "データ探索系"),
    k("fenwick-tree", "Fenwick木", "E問題相当", "変数とデータ構造", aliases=["BIT", "Binary Indexed Tree"]),
    k("segment-tree", "segment木", "E問題相当", "変数とデータ構造", aliases=["セグメント木"]),
    k("doubling", "ダブリング", "E問題相当", "高速化系", absorbs=["行列累乗"]),
    k("matrix-computation", "行列計算", "E問題相当", "その他数学系"),
    k("tropical-semiring", "トロピカル半環", "E問題相当", "その他数学系"),
    k("palindrome-check", "回文判定", "E問題相当", "文字列系"),
    k("z-algorithm", "Zアルゴリズム", "E問題相当", "文字列系"),
    k("lis", "最長増加部分列", "E問題相当", "典型問題集", aliases=["LIS"], related=["動的計画法", "二分探索"]),
    k("lcs", "最長共通部分列", "E問題相当", "典型問題集", aliases=["LCS"], related=["動的計画法"]),
    k("bellman-ford", "Bellman-Ford法", "E問題相当", "グラフ理論系"),
    k("warshall-floyd", "Warshall-Floyd法", "E問題相当", "グラフ理論系", aliases=["Floyd-Warshall"]),
    k("argument-sort", "偏角ソート", "E問題相当", "幾何学系"),

    # F問題相当
    k("lazy-segment-tree", "lazy segment木", "F問題相当", "変数とデータ構造", aliases=["遅延セグメント木"]),
    k("lca", "最近共通祖先", "F問題相当", "典型問題集", aliases=["LCA"], related=["木", "頂点倍加"]),
    k("coordinate-compression", "座標圧縮", "F問題相当", "高速化系"),
    k("trie", "Trie木", "F問題相当", "文字列系"),
    k("aho-corasick", "Aho-Corasick法", "F問題相当", "文字列系", related=["Trie木"]),
    k("nim", "Nim", "F問題相当", "その他数学系", aliases=["Grundy数", "ゲーム理論"], absorbs=["二人零和有限確定完全情報ゲーム", "二人零和有限確定完全情報ゲーム、Nim、Grundy数"]),
    k("mo-algorithm", "Moアルゴリズム", "F問題相当", "高速化系"),
    k("meet-in-the-middle", "半分全列挙", "F問題相当", "データ探索系", aliases=["meet-in-the-middle"]),
    k("inversion-number", "転倒数", "F問題相当", "典型問題集", related=["Fenwick木", "座標圧縮", "分割統治法"]),
    k("sqrt-decomposition", "平方分割", "F問題相当", "高速化系", aliases=["sqrt decomposition"]),

    # G問題以上相当
    k("catalan-number", "カタラン数", "G問題以上相当", "その他数学系", aliases=["Catalan number", "Catalan numbers", "Catalan数"], absorbs=["一般化カタラン数"], related=["順列組み合わせ", "二項係数"]),
    k("maximum-flow", "最大フロー", "G問題以上相当", "グラフ理論系"),
    k("minimum-cut", "最小カット", "G問題以上相当", "グラフ理論系"),
    k("bipartite-matching", "二部マッチング", "G問題以上相当", "グラフ理論系"),
    k("minimum-cost-flow", "最小費用流", "G問題以上相当", "グラフ理論系"),
    k("fps-convolution", "形式的冪級数、畳み込み", "G問題以上相当", "その他数学系", aliases=["FPS", "convolution", "NTT"], absorbs=["NTT"]),
    k("wavelet-matrix", "Wavelet Matrix", "G問題以上相当", "変数とデータ構造"),
    k("suffix-array", "Suffix Array", "G問題以上相当", "文字列系"),
    k("binary-tree", "二分木", "G問題以上相当", "変数とデータ構造"),

    # AHC用
    k("randomized-algorithm", "乱択アルゴリズム", "AHC用", "その他", aliases=["乱択", "randomized algorithm"]),
    k("ahc-greedy", "貪欲法（AHC）", "AHC用", "その他", aliases=["AHC貪欲", "greedy for AHC"], related=["貪欲法"]),
    k("hill-climbing", "山登り法", "AHC用", "その他", aliases=["hill climbing"], related=["乱択アルゴリズム"]),
    k("beam-search", "ビームサーチ", "AHC用", "その他", aliases=["beam search"]),
    k("simulated-annealing", "焼きなまし法", "AHC用", "その他", aliases=["simulated annealing", "SA"], related=["山登り法", "乱択アルゴリズム"]),

    # 未分類
    k("balanced-binary-search-tree", "平衡二分探索木", "未分類", "未分類"),
    k("scc", "強連結成分分解", "未分類", "未分類", aliases=["SCC"]),
    k("persistent-data-structure", "永続データ構造", "未分類", "未分類"),
    k("heavy-light-decomposition", "Heavy-Light Decomposition", "未分類", "未分類", aliases=["HLD"]),
    k("link-cut-tree", "Link-Cut Tree", "未分類", "未分類"),
    k("suffix-automaton", "Suffix Automaton", "未分類", "未分類"),
    k("min25-sieve", "Min25篩", "未分類", "未分類"),
    k("centroid-decomposition", "重心分解", "未分類", "未分類"),
    k("convex-hull", "凸包", "未分類", "未分類"),
    k("half-plane-intersection", "半平面交差", "未分類", "未分類"),
    k("farey-sequence", "Farey数列", "未分類", "未分類"),
    k("matroid", "マトロイド", "未分類", "未分類"),
    k("two-sat", "2-SAT", "未分類", "未分類"),
    k("zeckendorf-bruteforce", "Zeckendorf全探索", "未分類", "未分類"),
    k("vandermonde-identity", "ヴァンデルモンドの恒等式", "未分類", "未分類"),
]



def yaml_list(name: str, values: list[str]) -> str:
    if not values:
        return f"{name}: []\n"
    body = "".join(f"  - {v}\n" for v in values)
    return f"{name}:\n{body}"


def render_stub(stub: KnowledgeStub) -> str:
    return (
        "---\n"
        f"title: {stub.title}\n"
        f"level: {stub.level}\n"
        f"category: {stub.category}\n"
        f"summary: {stub.summary}\n"
        f"category_order: {stub.category_order}\n"
        f"level_order: {stub.level_order}\n"
        + yaml_list("aliases", stub.aliases)
        + yaml_list("absorbs", stub.absorbs)
        + yaml_list("related", stub.related)
        + "---\n\n"
        + "## 概要\n\n"
        + "今後記述予定。\n"
    )


def create_stubs(*, dry_run: bool = False) -> tuple[list[Path], list[Path]]:
    KNOWLEDGE_DIR.mkdir(parents=True, exist_ok=True)
    created: list[Path] = []
    skipped: list[Path] = []
    seen_slugs: set[str] = set()
    for stub in STUBS:
        if stub.slug in seen_slugs:
            raise ValueError(f"duplicate slug: {stub.slug}")
        seen_slugs.add(stub.slug)
        path = KNOWLEDGE_DIR / f"{stub.slug}.md"
        if path.exists():
            skipped.append(path)
            continue
        if not dry_run:
            path.write_text(render_stub(stub), encoding="utf-8")
        created.append(path)
    return created, skipped


def main() -> int:
    parser = argparse.ArgumentParser(description="未作成の知識記事Markdownスタブを生成します。")
    parser.add_argument("--dry-run", action="store_true", help="作成せずに対象だけ表示します。")
    parser.add_argument("--show-skipped", action="store_true", help="skip した既存ファイルのパスだけを表示します。本文や front matter は表示しません。")
    args = parser.parse_args()

    created, skipped = create_stubs(dry_run=args.dry_run)
    mode = "would create" if args.dry_run else "created"
    print(f"{mode}: {len(created)}")
    for path in created:
        print(f"  + {path.relative_to(ROOT)}")
    print(f"skipped existing: {len(skipped)}")
    if args.show_skipped:
        for path in skipped:
            print(f"  = {path.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
