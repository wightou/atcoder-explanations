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
if文の中で `cout` してもいいし、出力用の変数を用意しておいてif文の外で `cout` してもいい。

## 入力例1での動作

入力を受け取る。

```text
s: "EEWEW"
```

`E` ならカウンターを $1$ 減らし、`W` なら $1$ 増やす。
カウンターを $0$ から始めると、次のように変化する。

<!-- table-row-header: true -->
| 読んだ文字 | カウンター |
|---|---:|
| `E` | $-1$ |
| `E` | $-2$ |
| `W` | $-1$ |
| `E` | $-2$ |
| `W` | $-1$ |

最後のカウンターは $-1$ で $0$ より大きくないので、`"East"` を出力する。

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
