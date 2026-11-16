---
contest: ABC484
problem: A
problem_title: "Chocolate Bar Count"
problem_title_ja: "チョコバーの数"
problem_url: https://atcoder.jp/contests/abc484/tasks/abc484_a
submission_url: https://atcoder.jp/contests/abc484/submissions/80895868
tags:
  - 入出力
  - int型
  - 四則演算
tag_note:
---

## 考え方

入力は整数が $2$ つなので、`int` 型の変数 $2$ つ用意し、`cin` で入力を受け取る。

さて、これは簡単な算数の問題。
受け取った $B$ と $R$ をどちらも $30$ から引き、それらの積が答え。

求めたものを `cout` で出力しておしまい。

## 入力例1での動作

入力を受け取る。

```text
b: 8
r: 8
```

$30-8=22$ より、残るチョコレートは縦 $22$ 行、横 $22$ 列となる。
したがって、$22\times22=484$ 片が答えとなる。

## 注意点

特になし。

## 別解

特になし。
