---
contest: ABC469
problem: A
problem_title: "Train Car"
problem_title_ja: "電車の車両"
problem_url: https://atcoder.jp/contests/abc469/tasks/abc469_a
submission_url: https://atcoder.jp/contests/abc469/submissions/78057055
tags:
  - 入出力
  - int型
  - 四則演算
tag_note:
---

## 考え方

入力は整数が $2$ つなので、`int` 型の変数を $2$ つ用意し、`cin` で入力を受け取る。

前から $K$ 番目の車両は、自分より後ろに $N-K$ 両ある。
よって、後ろから何番目かというと、これに $1$ 足した $N-K+1$ が答え。

計算して、`cout` で出力しておしまい。

## 入力例1での動作

入力を受け取る。

```text
n: 5
k: 2
```

前から $2$ 両目の車両より後ろには、$5-2=3$ 両の車両がある。

後ろから数えると、この $3$ 両の次なので、$3+1=4$ 両目となる。

これをまとめて次のように計算する。

```text
result: 5+1-2=4
```

最後に、`result` の値である $4$ を出力する。

## 注意点

特になし。

## 別解

特になし。
