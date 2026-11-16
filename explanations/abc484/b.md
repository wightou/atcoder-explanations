---
contest: ABC484
problem: B
problem_title: "Nineball"
problem_title_ja: "ナインボール"
problem_url: https://atcoder.jp/contests/abc484/tasks/abc484_b
submission_url: https://atcoder.jp/contests/abc484/submissions/80895872
tags:
tag_note: A問題以下レベルの内容は省略。
---

## 考え方

問題が言っていることは、以下。
- $9$ がなかった場合は、$-1$ を出力する
- $9$ があった場合には、$A$ の中の最も小さい番号を出力する

入力の $A$ はソートされているとは限らないが、ならば自分でソートすればよい。
$9$ があるとしたら必ず末尾なので、以下の処理をすればよい。
- 末尾が $9$ でなかった場合は、$-1$ を出力する
- 末尾が $9$ だった場合には、先頭を出力する

## 入力例1での動作

入力を受け取る。

```text
n: 5
a: {6, 4, 2, 9, 8}
```

$A$ をソートすると、次のようになる。

```text
a: {2, 4, 6, 8, 9}
```

末尾が $9$ なので、先頭の $2$ が答えとなる。

## 注意点

特になし。

## 別解

特になし。
