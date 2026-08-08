---
contest: ABC470
problem: A
problem_title: "Fizz"
problem_title_ja: "Fizz"
problem_url: https://atcoder.jp/contests/abc470/tasks/abc470_a
submission_url: https://atcoder.jp/contests/abc470/submissions/78235612
tags:
  - 入出力
  - int型
  - if分岐
  - forループ
  - 四則演算
  - 倍数判定
tag_note:
---

## 考え方

プログラミングの練習問題として有名な FizzBuzz 問題の Fizz だけバージョン。

入力は整数が $1$ つなので、`int` 型の変数を用意し、`cin` で入力を受け取る。

まず、Fizz のことを無視して、$1$ から $N$ までを順番に出力していくコードを作る。
```cpp
for (int i=1; i<=n; i++) {
  cout << i << endl;
}
```

この出力部分を、$3$ の倍数のときだけ `Fizz` に変えてやればよい。

```cpp
if (3の倍数じゃないなら) {
  cout << i << endl;
} else {
  cout << "Fizz" << endl;
}
```

$3$ の倍数でない判定は、`i%3` をすればよい。
これが $0$ でない場合は、$i$ は $3$ の倍数ではない。
if 文の判定条件に `int` 型の値を書いた場合、 $0$ 以外だったら `true` とみなされる仕様を利用できる。

## 入力例1での動作

入力を受け取る。

```text
n: 4
```

`i=1` から `i=4` まで順に処理する。

`i=1` では、$1$ は $3$ の倍数ではないので、`1` を出力する。

`i=2` では、$2$ は $3$ の倍数ではないので、`2` を出力する。

`i=3` では、$3$ は $3$ の倍数なので、`"Fizz"` を出力する。

`i=4` では、$4$ は $3$ の倍数ではないので、`4` を出力する。

したがって、出力は次のようになる。

```text
1
2
Fizz
4
```

## 注意点

特になし。

## 別解

特になし。
