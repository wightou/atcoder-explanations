---
contest: ABC464
problem: A
problem_title: "Decisive Battle"
problem_title_ja: "決戦"
problem_url: https://atcoder.jp/contests/abc464/tasks/abc464_a
submission_url: https://atcoder.jp/contests/abc464/submissions/77039499
alternative_submission_urls:
  - label: 別解
    url: https://atcoder.jp/contests/abc464/submissions/77039500
tags:
  - 入出力
  - int型
  - char型
  - string型
  - if分岐
  - forループ
  - 数え上げ
tag_note:
---

## 考え方

入力は文字列が $1$ つなので、`string` 型の変数を $1$ つ用意し、`cin` で入力を受け取る。

文字列の中身を $1$ 文字ずつ見ていきたいので、forループを用意する。
問題の指示通り、文字列に含まれる `E` と `W` の個数を比べればよい。

そのために、下準備として、まずカウンターを用意して、$0$ で初期化する。
そして、$1$ 文字ごとに、それが `E` なら $-1$、`W` なら $+1$ をしていく。
最終的にカウンターが正の数なら西軍の方が多く、負の数なら東軍の方が多い。

ということで、カウンターの値が $0$ より大きいなら `"West"`、そうでないなら `"East"` と答えればよい。
if分岐の中でcoutしてもいいし、出力用の変数を用意しておいてif分岐の外でcoutしてもいい。

## 入力例1での動作

入力を受け取る。

```
s: "EEWEW"
```

文字列の長さを取得する。

```
n: 5
```

答え用の変数 `result` を `"East"` で初期化する。

```
result: "East"
```

`counter` を $0$ で初期化する。

```
counter: 0
```

$0$ 文字目をチェックする。
`'E'` なので `counter` を $1$ 減らす。

```
counter: -1
```

$1$ 文字目をチェックする。
`'E'` なので `counter` を $1$ 減らす。

```
counter: -2
```

$2$ 文字目をチェックする。
`'W'` なので `counter` を $1$ 増やす。

```
counter: -1
```

$3$ 文字目をチェックする。
`'E'` なので `counter` を $1$ 減らす。

```
counter: -2
```

$4$ 文字目をチェックする。
`'W'` なので `counter` を $1$ 増やす。

```
counter: -1
```

最終的に、`counter` は $0$ より大きくないので、`result` は `"East"` のままでよい。

```
result: "East"
```

最後に、`result` を出力すればよい。

## 注意点

特になし。

## 別解

ループ内で「今何番目であるか」を使わないので、範囲for文を使ってもよい。

```cpp
for (char c : s) {
  if (c == 'E') counter--;
  else counter++;
}
```

いちいち `.at(i)` を書く手間が省けるし、文字列の長さを自分で取得するのも不要になる。
