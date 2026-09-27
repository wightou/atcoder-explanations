---
contest: ABC477
problem: B
problem_title: "Standing Outliers"
problem_title_ja: "一線を画す"
problem_url: https://atcoder.jp/contests/abc477/tasks/abc477_b
submission_url: https://atcoder.jp/contests/abc477/submissions/79571517
tags:
  - 全探索
  - 多重ループ
  - 絶対値
tag_note: A問題以下レベルの内容は省略。
---

## 考え方

素直に二重ループの全探索でよい。

$i$ 番の人を見るとき、$i$ 番以外の全ての $j$ 番の人との距離が全て $D$ 以上であればよい。
これは、最初に `bool` 型の `true` を用意して、$1$ 人でも距離 $D$ 未満の人がいたら `false` にする。
全員見終わっても `true` のままだったら、$i$ 番の人は「一線を画す」人である。

これを、全ての $i$ について調査し、配列に記録していけばよい。

## 入力例1での動作

入力を受け取る。

```text
n: 3
d: 3
x: {1, 5, 7}
```

各人について、自分以外の人との距離を確認する。

<!-- table-row-header: true -->
| 人 | 自分以外の人との距離 | 全て $D=3$ 以上か |
| ---: | :---: | :---: |
| $1$ | $4,6$ | はい |
| $2$ | $4,2$ | いいえ |
| $3$ | $6,2$ | いいえ |

条件を満たすのは人 $1$ だけである。
したがって、「一線を画す」人の人数は $1$ 人で、番号は $1$ となる。

## 注意点

特になし。

## 別解

特になし。
