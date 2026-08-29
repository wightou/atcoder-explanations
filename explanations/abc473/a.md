---
contest: ABC473
problem: A
problem_title: "Second Half Sum"
problem_title_ja: "後半の総和"
problem_url: https://atcoder.jp/contests/abc473/tasks/abc473_a
submission_url: https://atcoder.jp/contests/abc473/submissions/78830469
tags:
  - 入出力
  - int型
  - vector
  - forループ
  - 四則演算
tag_note:
---

## 考え方

入力は整数と数列が $1$ つずつ。
`int` 型の変数と `vector` を $1$ つずつ用意し、`cin` で入力を受け取る。

配列を後半だけ足す、ということなので、まずは全部足す場合のコードを用意する。
これは、$0$ で初期化した整数型変数に、forループで以下のように $1$ つずつ足せばよい。

```cpp
int result = 0;
for (int i=0; i<n; i++) {
  result += a.at(i);
}
```

後半だけ足したいならば、$i$ が $0$ からになっているところを $n/2$ にすればよい。
$n$ は偶数なので、$n/2$ の切り上げ切り捨ては心配する必要がない。

最後に、忘れずに `cout` で答えを出力しておしまい。

## 入力例1での動作

$n=8$、`a={1,3,7,8,4,2,5,6}` を受け取る。
`result` は $0$ で初期化する。

$n/2=8/2=4$ なので、0-indexed で `a[4]` から順に足していく。

- $i=4$ では、`a[4]=4` なので、`result` は $0+4=4$ となる。
- $i=5$ では、`a[5]=2` なので、`result` は $4+2=6$ となる。
- $i=6$ では、`a[6]=5` なので、`result` は $6+5=11$ となる。
- $i=7$ では、`a[7]=6` なので、`result` は $11+6=17$ となる。

ループが終わったら、`result` の $17$ を出力する。

## 注意点

特になし。

## 別解

特になし。
