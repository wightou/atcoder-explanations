---
contest: ABC476
problem: A
problem_title: "Appender"
problem_title_ja: "追加する人"
problem_url: https://atcoder.jp/contests/abc476/tasks/abc476_a
submission_url: https://atcoder.jp/contests/abc476/submissions/79393923
tags:
  - 入出力
  - string型
  - char型
  - if分岐
tag_note:
---

## 考え方

入力は文字列が $1$ つなので、`string` 型の変数を $1$ つ用意し、`cin` で入力を受け取る。

さて、末尾によってつける文字を変えるということだが、これは言い換えればこういうこと。
- まず、末尾が `e` でなければ、とりあえず `e` をつける
- その後、末尾に `r` をつける

末尾の文字は `s.back()` で取り出せるので、if文で以下のように書けば、「末尾が `e` でなければ」になる。

```cpp
if(s.back()!='e') {
  // 処理
}
```

末尾に文字を $1$ つくっつけるのは、`+= 'e'` や `+= 'r'` でよい。

最後に、完成した文字列を出力しておしまい。

## 入力例1での動作

入力を受け取る。

```text
s: "live"
```

末尾は `e` なので、`e` は追加しない。

その後、末尾に `r` を追加すると `liver` になる。

したがって、`liver` を出力する。

## 注意点

特になし。

## 別解

特になし。
