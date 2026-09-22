---
contest: ABC463
problem: B
problem_title: "Train Reservation"
problem_title_ja: "列車の予約"
problem_url: https://atcoder.jp/contests/abc463/tasks/abc463_b
submission_url: https://atcoder.jp/contests/abc463/submissions/76862462
tags:
  - 全探索
tag_note: A問題以下レベルの内容は省略。
---

## 考え方

問題の指示通りに、全ての列車について、目的の席があいているかチェックすればよい。

少し難しいのは、`'A'` のときは $0$ 番目、`'D'` のときは $3$ 番目、などの変換。
これは実は `x - 'A'` という計算をすることで、`x` が `'A'` のいくつ後の文字なのかを求められる。

これさえわかれば、あとは指示通りのコードを書くだけである。

## 入力例1での動作

入力を受け取る。

```text
n: 3
x: 'A'
s: {"xoxox",
    "xxooo",
    "oxxxx"}
```

`'A'` は $0$ 番目の列に対応するので、各列車の $0$ 番目の文字を順に確認する。

<!-- table-row-header: false -->
| 列車 | 座席情報 | A列の文字 | 空席があるか |
| ---: | :---: | :---: | :---: |
| $1$ | `"xoxox"` | `'x'` | ない |
| $2$ | `"xxooo"` | `'x'` | ない |
| $3$ | `"oxxxx"` | `'o'` | ある |

$3$ 本目の列車の A 列に空席があるので、答えは `"Yes"` となる。

## 注意点

特になし。

## 別解

特になし。
