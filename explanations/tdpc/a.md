---
contest: TDPC
problem: A
problem_title: "コンテスト"
problem_url: https://atcoder.jp/contests/tdpc/tasks/tdpc_contest
submission_url: https://atcoder.jp/contests/tdpc/submissions/78342155
tags:
  - 動的計画法
  - ナップサック問題
tag_note: 簡単なレベルの内容は省略。
---

## 考え方

ABCでいうと、D問題級。
典型的な部分和問題である。

`dp[j]` を、総和を $j$ にできるかどうかとする。
各数について、使う場合と使わない場合を調べて DP テーブルを更新する。

ただし、同じ数を $2$ 回以上使わないようにするため、`dp` は大きい方から更新する。
詳しくは「動的計画法」「ナップサック問題」の記事参照。

計算量は $O(N\sum p_i)$。

## 入力例1での動作

入力を受け取る。

```text
n: 3
p: {2, 3, 5}
```

配点の総和は $2+3+5=10$ なので、`dp[0]` から `dp[10]` までを用意する。
`dp[j]` を、総得点 $j$ を作れるかとする。

DP テーブルは次のように更新される。
`T` は `true`、`F` は `false` を表す。

<!-- table-row-header: true -->
| 使用した配点＼総得点 | $0$ | $1$ | $2$ | $3$ | $4$ | $5$ | $6$ | $7$ | $8$ | $9$ | $10$ |
|---|---|---|---|---|---|---|---|---|---|---|---|
| なし | {green}T | F | F | F | F | F | F | F | F | F | F |
| $2$ まで | {green}T | F | {green}T | F | F | F | F | F | F | F | F |
| $3$ まで | {green}T | F | {green}T | {green}T | F | {green}T | F | F | F | F | F |
| $5$ まで | {green}T | F | {green}T | {green}T | F | {green}T | F | {green}T | {green}T | F | {green}T |

最終的に `dp` が `true` である総得点は $7$ 個あるので、答えは $7$。

## 注意点

特になし。

## 別解

特になし。
