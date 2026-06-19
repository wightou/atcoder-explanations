---
contest: ABC461
problem: A
problem_title: "Armor"
problem_url: https://atcoder.jp/contests/abc461/tasks/abc461_a
submission_url: https://atcoder.jp/contests/abc461/submissions/76604367
tags:
  - 入出力
  - int型
  - if分岐
tag_note:
---

## 解法

入力は整数が $2$ つなので、`int` 型の変数を $2$ つ用意し、`cin` で入力を受け取る。

この防具は、威力 $D$ 以下の攻撃を防ぐ。
よって、威力 $A$ の攻撃を防げるかどうかは、 $A \leq D$ であるかを判定すればよい。
```cpp
if (a<=d) {
  // a<=dであるときの処理（`Yes` と答える）
} else {
  // そうでないときの処理（`No` と答える）
}
```

if文の中で直接 `cout` してもいいし、事前に出力用文字列を用意してもよい。

## 入力例1での動作

入力を受け取る。
```
a: 4
d: 5
```

`4` は `5` 以下なので、答えは `"Yes"`。
```
result: "Yes"
```

## 注意点

特になし。

## 別解

特になし。