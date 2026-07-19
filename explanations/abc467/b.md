---
contest: ABC467
problem: B
problem_title: "Keep the Change"
problem_title_ja: "釣りは取っとけ"
problem_url: https://atcoder.jp/contests/abc467/tasks/abc467_b
submission_url: https://atcoder.jp/contests/abc467/submissions/77654138
tags:
tag_note: A問題以下レベルの内容は省略。
---

## 考え方

問題文の通りにシミュレーションをしてもよいが、実際に買い物をする様子を想像すればもっと簡単。
要するに、辞退したおつりの合計額を出せばよいだけである。

つまり、各買い物について、以下を行うだけでよい。
- まず、$S$ が `"keep"` かどうかを確認
- 違ったらこの時点で次の買い物へ
- `"keep"` だった場合、$B$ から $A$ を引いておつり額を出し、結果に加える

なお、最初に $10000$ 円という数値があるが、これは全買い物で所持金が不足しないと言っているだけ。
大した意味はない。

## 入力例1での動作

入力を受け取る。

```
n: 3
a: {1, 3, 5}
b: {2, 6, 9}
s: {"keep", "take", "keep"}
```

答え用の変数 `result` を `0` で初期化する。

```
result: 0
```

$i=0$ のとき、`s[0]` は `"keep"` である。

`b[0]-a[0]` は $2-1=1$ なので、`result` に `1` を加える。

```
result: 1
```

$i=1$ のとき、`s[1]` は `"take"` である。

`result` は変更しない。

```
result: 1
```

$i=2$ のとき、`s[2]` は `"keep"` である。

`b[2]-a[2]` は $9-5=4$ なので、`result` に `4` を加える。

```
result: 5
```

## 注意点

特になし。

## 別解

特になし。
