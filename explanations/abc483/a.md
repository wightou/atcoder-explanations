---
contest: ABC483
problem: A
problem_title: "Double-Sided Printing"
problem_title_ja: "両面印刷"
problem_url: https://atcoder.jp/contests/abc483/tasks/abc483_a
submission_url: https://atcoder.jp/contests/abc483/submissions/80702091
tags:
  - 入出力
  - int型
  - 四則演算
tag_note:
---

## 考え方

入力は整数が $1$ つなので、`int` 型の変数を用意し、`cin` で入力を受け取る。

さて、簡単な算数により、$N$ を $2$ で割って切り上げた答えを `cout` で出力すればよい。
しかし、C++の割り算は基本的に切り捨て処理なので一工夫必要。

$M$ で割って切り上げるときには、$M-1$ を足してから $M$ で割って切り捨てればよい。
今回の場合は、$2-1=1$ を足してから `/2` すればよい。

計算できたら、それを `cout` しておしまい。

## 入力例1での動作

入力を受け取る。

```text
n: 3
```

入力された $N=3$ に $1$ を加え、$2$ で割ると $(3+1)/2=2$ となる。

## 注意点

特になし。

## 別解

特になし。
