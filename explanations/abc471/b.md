---
contest: ABC471
problem: B
problem_title: "Survey Tabulation"
problem_title_ja: "アンケート集計"
problem_url: https://atcoder.jp/contests/abc471/tasks/abc471_b
submission_url: https://atcoder.jp/contests/abc471/submissions/78494136
tags:
  - map
tag_note: A問題以下レベルの内容は省略。
---

## 考え方

指示通り、同内容の回答数をカウンティングすればよい。
問題は $2$ 点。

$1$ つは、回答の大文字小文字を区別せずに数えなければならないこと。
これは、全ての回答を小文字に（または大文字に）統一してしまえばよい。
$1$ 文字ずつ見て、`isupper(c)` で大文字かどうか判定し、`c += 'a'-'A';` で小文字化する。

もう $1$ つは、票の内容が文字列であること。
カウンティングでよく行う、$i$ があったら配列の $i$ 番目を $1$ 増やす方法が使えない。
これは `map` を使うことで解決する。
その後最大回答数を調べるときには、範囲for文を用いればよい。

## 入力例1での動作

回答を順に小文字化すると、次のようになる。

| 元の回答 | 小文字化後 |
|---|---|
| `ARC` | `arc` |
| `abc` | `abc` |
| `ahc` | `ahc` |
| `ABC` | `abc` |
| `beginner` | `beginner` |
| `AbC` | `abc` |
| `ahc` | `ahc` |

これらを `map` で数えると、各回答数は次のようになる。

| 回答 | 回答数 |
|---|---:|
| `abc` | 3 |
| `ahc` | 2 |
| `arc` | 1 |
| `beginner` | 1 |

このなかの最大値は $3$ なので、答えは $3$ である。

## 注意点

特になし。

## 別解

特になし。
