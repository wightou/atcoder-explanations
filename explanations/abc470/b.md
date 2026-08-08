---
contest: ABC470
problem: B
problem_title: "Monocolor"
problem_title_ja: "単色"
problem_url: https://atcoder.jp/contests/abc470/tasks/abc470_b
submission_url: https://atcoder.jp/contests/abc470/submissions/78235624
tags:
tag_note: A問題以下レベルの内容は省略。
---

## 考え方

色を変えるボールの個数を最小にしたい。
それはつまり、色を変えなくていいボールの個数を最大にすればよい。

どの色が何個あるかカウンティングを行って、最大値を探す。
すると、それが色を変えなくていいボールの最大値となる。
よって、それを全ボール数から引けば答えとなる。

## 入力例1での動作

入力を受け取る。

```text
n: 4
a: {3, 1, 2, 1}
```

各色のボールが何個あるかを `counters` に数える。
`counters[0]` はダミーとして、次のようになる。

```text
counters: {-, 2, 1, 1, 0}
```

最も多い色は色 $1$ で、その個数は $2$ 個である。
この $2$ 個以外のボールの色を変えればよいので、答えは $4-2=2$ となる。

## 注意点

特になし。

## 別解

特になし。
