---
contest: ABC463
problem: B
problem_title: "Train Reservation"
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
```
n: 3
x: 'A'
s: {"xoxox",
    "xxooo",
    "oxxxx"}
```

答えを表す変数 `result` を `"No"` で初期化する。
```
result: "No"
```

それぞれの列車について、`x-'A'`、つまり $0$ 番目の文字を確認していく。

$1$ 本目の列車を見る。
$0$ 番目の文字は `'x'` なので、指定された列に空席はない。
```
str: "xoxox"
str[x-'A']: 'x'
result: "No"
```

$2$ 本目の列車を見る。
$0$ 番目の文字は `'x'` なので、指定された列に空席はない。
```
str: "xxooo"
str[x-'A']: 'x'
result: "No"
```

$3$ 本目の列車を見る。
$0$ 番目の文字は `'o'` なので、指定された列に空席がある。
そのため、出力文字列を `"Yes"` に変更する。
```
str: "oxxxx"
str[x-'A']: 'o'
result: "Yes"
```

結果として `"Yes"` を出力する。

## 注意点

特になし。

## 別解

特になし。
