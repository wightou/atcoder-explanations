---
contest: ABC485
problem: A
problem_title: "Same Side"
problem_title_ja: "同じ側"
problem_url: https://atcoder.jp/contests/abc485/tasks/abc485_a
submission_url: https://atcoder.jp/contests/abc485/submissions/81039257
alternative_submission_urls:
  - label: 別解
    url: https://atcoder.jp/contests/abc485/submissions/81039259
tags:
  - 入出力
  - int型
  - string型
  - char型
  - if分岐
  - 四則演算
tag_note:
---

## 考え方

入力は整数が $2$ つなので、`int` 型の変数 $2$ つ用意し、`cin` で入力を受け取る。

条件より、if文で「両方正の数、または両方負の数」になっているかどうかを確認すればよい。
素直に書いてもよいが、「かけて正の数か」という判定だと楽に書ける。

判定の結果にあわせて `"Yes"` または `"No"` を出力すればよい。
if文の中で直接 `cout` してもいいし、出力用の変数を用意して書き換えてもよい。

## 入力例1での動作

入力を受け取る。

```text
a: 40
b: -20
```

出力用の変数を `"No"` で初期化する。
$a\times b=40\times(-20)=-800$ なので、$a\times b>0$ は成り立たず、`"No"` のままである。
したがって、`No` を出力する。

## 別解

条件は、片方が負の数であること。
つまり、数値ではなく文字列として入力を受け取り、そのなかに `'-'` の文字が何回あるか数えてもよい。
それが $1$ 回なら `"No"`、$0$ 回か $2$ 回なら `"Yes"`。
