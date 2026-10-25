---
contest: ABC481
problem: A
problem_title: "Digits of A+B"
problem_title_ja: "A+Bの桁数"
problem_url: https://atcoder.jp/contests/abc481/tasks/abc481_a
submission_url: https://atcoder.jp/contests/abc481/submissions/80392625
tags:
  - 入出力
  - int型
  - if分岐
  - 四則演算
tag_note:
---

## 考え方

入力は整数が $2$ つなので、`int` 型の変数を $2$ つ用意し、`cin` で入力を受け取る。

まず、入力が $1$ から $9$ までなので、足した値は $2$ から $18$ のいずれか。
つまり、答えは $1$ か $2$ しかありえない。
そして、$2$ になる条件は、和が $10$ 以上になること。

よって、if文で「$10$ 未満なら $1$、$10$ 以上なら $2$」を出力すればよい。
if文の中で直接 `cout` してもいいし、出力用の変数を用意して書き換えてもよい。

## 入力例1での動作

入力を受け取る。

```text
a: 4
b: 9
```

出力用変数を $1$ で初期化する。

$a+b=4+9=13$ であり、$10$ 以上なので、出力用変数を $2$ に書き換える。

したがって、$2$ を出力する。

## 注意点

特になし。

## 別解

特になし。
