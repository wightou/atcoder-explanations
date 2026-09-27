---
contest: ABC477
problem: A
problem_title: "Traffic Light"
problem_title_ja: "信号機"
problem_url: https://atcoder.jp/contests/abc477/tasks/abc477_a
submission_url: https://atcoder.jp/contests/abc477/submissions/79571511
tags:
  - 入出力
  - char型
  - string型
  - if分岐
tag_note:
---

## 考え方

入力は文字が $1$ つなので、`char` 型の変数を $1$ つ用意し、`cin` で入力を受け取る。
もしくは、今回の場合は `string` 型で、長さが $1$ 文字の文字列として受け取っても問題ない。

if文で $3$ つの動作を分けたいので、以下のように記述する。

``` cpp
if (c=='B') {
  // 'B'だったときの動作
} else if (c=='Y') {
  // 'Y'だったときの動作
} else if (c=='R') {
  // 'R'だったときの動作
}
```

中身は、直接文字を `cout` してもいいし、事前に用意した出力用変数を書き換えてもよい。

## 入力例1での動作

入力を受け取る。

```text
c: 'B'
```

`c=='B'` が `true` なので、次の信号機の色である `'Y'` を出力する。

## 注意点

特になし。

## 別解

特になし。
