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

$P$ と $Q$ を両方通る円の中心は、必ず線分 $PQ$ の垂直二等分線上にある。
$R$ と $S$ についても同様。
よって、$PQ \nparallel RS$ であれば、垂直二等分線の交点を中心にすればよく、答えは `"Yes"`。

さて、$PQ \parallel RS$ である場合はどうか。
この場合は、垂直二等分線同士が平行になりそうに見えるが、同一直線になる場合だけは答えは `"Yes"`。
これは四角形 $PQRS$ または $PQSR$ が等脚台形になることを意味する。
したがって、$PR=QS$ かつ $PS=QR$ であるなら答えは `"Yes"`。

まとめると、$PQ \nparallel RS$ または $(PR=QS$ かつ $PS=QR)$ なら `"Yes"` で、それ以外は `"No"`。

平行かどうかはベクトルの外積で判定でき、点と点の距離もベクトルの絶対値の $2$ 乗を用いればよい。

## 入力例1での動作

テストケース数を受け取る。

```
q: 3
```

$1$ つめのテストケースを受け取る。

```
p: (2, 0)
q: (1, 1)
r: (-1, 0)
s: (1, 2)
```

答え用の変数 `result` を `"Yes"` で初期化する。

```
result: "Yes"
```

$\overrightarrow{PQ}=(-1,1)$、$\overrightarrow{RS}=(2,2)$ である。

この $2$ つのベクトルの外積は $-4$ であり、$PQ$ と $RS$ は平行でない。

`result` は変更しない。

```
result: "Yes"
```

`result` を出力する。

$2$ つめのテストケースを受け取る。

```
p: (1, 0)
q: (-1, 0)
r: (0, 1)
s: (0, -1)
```

答え用の変数 `result` を `"Yes"` で初期化する。

$\overrightarrow{PQ}=(-2,0)$、$\overrightarrow{RS}=(0,-2)$ である。

この $2$ つのベクトルの外積は $4$ であり、$PQ$ と $RS$ は平行でない。

`result` は変更しない。

```
result: "Yes"
```

`result` を出力する。

$3$ つめのテストケースを受け取る。

```
p: (4, 0)
q: (3, 1)
r: (2, 0)
s: (1, 1)
```

答え用の変数 `result` を `"Yes"` で初期化する。

$\overrightarrow{PQ}=(-1,1)$、$\overrightarrow{RS}=(-1,1)$ である。

この $2$ つのベクトルの外積は $0$ であり、$PQ$ と $RS$ は平行である。

次に、各ベクトルの長さの二乗を計算する。

```
(r-q).norm2(): 2
(s-p).norm2(): 10
```

この $2$ つは等しくないので、`result` を `"No"` にする。

```
result: "No"
```

`result` を出力する。

以上より、順に `"Yes"`、`"Yes"`、`"No"` と出力すればよい。

## 注意点

座標の差の二乗和や外積は、`int` 型からはみ出る。
`long long` 型を用いること。

## 別解

特になし。
