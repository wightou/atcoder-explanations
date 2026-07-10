---
contest: ABC462
problem: A
problem_title: "Secret Numbers"
problem_title_ja: "隠された数"
problem_url: https://atcoder.jp/contests/abc462/tasks/abc462_a
submission_url: https://atcoder.jp/contests/abc462/submissions/76685454
alternative_submission_urls:
  - label: 別解
    url: https://atcoder.jp/contests/abc462/submissions/76713168
tags:
  - 入出力
  - int型
  - char型
  - string型
  - if分岐
  - forループ
---

## 考え方

入力は文字列が $1$ つなので、`string` 型の変数を $1$ つ用意し、`cin` で入力を受け取る。

文字列の中身を $1$ 文字ずつ見ていきたいので、forループを用意する。
それぞれの文字について、if文で以下を行えばよい。

- 数字なら、それを答え用の文字列に追加する
- 数字でないなら何もしない

数字かどうかの判定は、文字が `'0'` 以上 `'9'` 以下かどうかで行える。

```cpp
if (s.at(i) >= '0' && s.at(i) <= '9') {
  result += s.at(i);
}
```

また、標準ライブラリ関数 `isdigit` を用いてもよい。

```cpp
if (isdigit(s.at(i))) {
  result += s.at(i);
}
```

最後に、作った答え用の文字列を `cout` で出力すればよい。

## 入力例1での動作

入力を受け取る。

```
s: "abc462"
```

$0$ 文字目をチェックする。
`'a'` は数字でないので何もしない。

```
s: "abc462"
result: ""
```

$1$ 文字目をチェックする。
`'b'` は数字でないので何もしない。

```
s: "abc462"
result: ""
```

$2$ 文字目をチェックする。
`'c'` は数字でないので何もしない。

```
s: "abc462"
result: ""
```

$3$ 文字目をチェックする。
`'4'` は数字なので答え用の文字列に加える。

```
s: "abc462"
result: "4"
```

$4$ 文字目をチェックする。
`'6'` は数字なので答え用の文字列に加える。

```
s: "abc462"
result: "46"
```

$5$ 文字目をチェックする。
`'2'` は数字なので答え用の文字列に加える。

```
s: "abc462"
result: "462"
```

最終的に、`result` を出力すればよい。

## 注意点

特になし。

## 別解

ループ内で「今何番目であるか」を使わないので、範囲for文を使ってもよい。

```cpp
for (char c : s) {
  if (isdigit(c)) {
    result += c;
  }
}
```

いちいち `.at(i)` を書く手間が省けるし、文字列の長さを自分で取得するのも不要になる。
