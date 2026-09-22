---
contest: ABC467
problem: D
problem_title: "Concentric Circles"
problem_title_ja: "同心円"
problem_url: https://atcoder.jp/contests/abc467/tasks/abc467_d
submission_url: https://atcoder.jp/contests/abc467/submissions/77654287
tags:
  - ベクトル（幾何学）
  - 考察問題
tag_note: B問題以下レベルの内容は省略。
---

## 考え方

$\mathrm{P}$ と $\mathrm{Q}$ を両方通る円の中心は、必ず線分 $\mathrm{PQ}$ の垂直二等分線上にある。
$\mathrm{R}$ と $\mathrm{S}$ についても同様。
よって、$\mathrm{PQ} \nparallel \mathrm{RS}$ であれば、垂直二等分線の交点を中心にすればよく、答えは `"Yes"`。

さて、$\mathrm{PQ} \parallel \mathrm{RS}$ である場合はどうか。
この場合は、垂直二等分線同士が平行になりそうに見えるが、同一直線になる場合だけは答えは `"Yes"`。
これは四角形 $\mathrm{PQRS}$ または $\mathrm{PQSR}$ が等脚台形になることを意味する。
したがって、$\mathrm{PR}=\mathrm{QS}$ かつ $\mathrm{PS}=\mathrm{QR}$ であるなら答えは `"Yes"`。

まとめると、$\mathrm{PQ} \nparallel \mathrm{RS}$ または $(\mathrm{PR}=\mathrm{QS}$ かつ $\mathrm{PS}=\mathrm{QR})$ なら `"Yes"` で、それ以外は `"No"`。

平行かどうかはベクトルの外積で判定でき、点と点の距離もベクトルの絶対値の $2$ 乗を用いればよい。

## 入力例1での動作

テストケース数は $3$ である。

$1$ つめのテストケースでは、

```text
P: (2, 0)
Q: (1, 1)
R: (-1, 0)
S: (1, 2)
```

となる。

$\overrightarrow{\mathrm{PQ}}=(-1,1)$、$\overrightarrow{\mathrm{RS}}=(2,2)$ であり、
外積は $-4$ である。
したがって $\mathrm{PQ}$ と $\mathrm{RS}$ は平行でなく、答えは `Yes` となる。

$2$ つめのテストケースでは、

```text
P: (1, 0)
Q: (-1, 0)
R: (0, 1)
S: (0, -1)
```

となる。

$\overrightarrow{\mathrm{PQ}}=(-2,0)$、$\overrightarrow{\mathrm{RS}}=(0,-2)$ であり、
外積は $4$ である。
したがって、この場合も答えは `Yes` となる。

$3$ つめのテストケースでは、

```text
P: (4, 0)
Q: (3, 1)
R: (2, 0)
S: (1, 1)
```

となる。

$\overrightarrow{\mathrm{PQ}}=(-1,1)$、$\overrightarrow{\mathrm{RS}}=(-1,1)$ なので、
今度は $\mathrm{PQ}$ と $\mathrm{RS}$ が平行である。

そこで距離を調べると、$\mathrm{QR}^2=2$、$\mathrm{PS}^2=10$ で一致しない。
よって等脚台形になる条件を満たさず、答えは `No` となる。

以上より、出力は順に `Yes`、`Yes`、`No` となる。

## 注意点

座標の差の二乗和や外積は、`int` 型からはみ出る。
`long long` 型を用いること。

## 別解

特になし。
