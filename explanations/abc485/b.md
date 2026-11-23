---
contest: ABC485
problem: B
problem_title: "Faucets and Drains"
problem_title_ja: "蛇口と排水管"
problem_url: https://atcoder.jp/contests/abc485/tasks/abc485_b
submission_url: https://atcoder.jp/contests/abc485/submissions/81039261
tags:
  - 多重ループ
tag_note: A問題以下レベルの内容は省略。
---

## 考え方

`'o'` が記録されているところは、その蛇口と排水管は正常である保証が取れたということ。
よって、各蛇口と各排水管が一度でも正常動作したかを記録する `vector<bool>` を用意すればよい。
各データを見て、正常保証が取れたところを記録していく。
すると、最後に $1$ つだけ保証がないところが残るはずである。

あとは、これを二重ループで書く実装力があるかどうかの勝負。

## 入力例1での動作

入力を受け取る。

```text
n: 3
m: 4
s: {"oooo", "xxxx", "oooo"}
```

各蛇口の記録から、正常と確認できる設備を整理する。

<!-- table-row-header: true -->
| 蛇口 | 記録 | 正常と確認できる設備 |
| :---: | :---: | :--- |
| $1$ | `oooo` | 蛇口 $1$、排水管 $1,2,3,4$ |
| $2$ | `xxxx` | なし |
| $3$ | `oooo` | 蛇口 $3$、排水管 $1,2,3,4$ |

これをまとめると、各設備の正常確認の状態は次のようになる。

```text
faucet: {true, false, true}
drain:  {true, true, true, true}
```

正常と確認できないのは蛇口 $2$ のみなので、答えは `Faucet 2` となる。
