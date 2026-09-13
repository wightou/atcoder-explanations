---
contest: ABC475
problem: A
problem_title: "mnclr"
problem_title_ja: "単色"
problem_url: https://atcoder.jp/contests/abc475/tasks/abc475_a
submission_url: https://atcoder.jp/contests/abc475/submissions/79235178
tags:
  - 入出力
  - string型
  - if分岐
  - forループ
tag_note:
---

## 考え方

入力は文字列が $1$ つなので、`string` 型の変数を $1$ つ用意し、`cin` で入力を受け取る。

文字列の中身を $1$ 文字ずつ見ていきたいので、forループを用意する。
そして、出力用文字列に $1$ 文字ずつコピーしていく。
```cpp
for (int i=0; i<n; i++) {
  result += s.at(i);
}
```

これで、全く同じ文字列を $1$ 文字ずつコピーすることはできる。
これを、$1$ 文字ずつ `'o'` を挟んでいくには、コピー後に `'o'` を追加すればよい。
```cpp
result += 'o';
```

ただし、これでは最後の文字の後ろに余計な `'o'` が追加されてしまう。
そこで、`'o'` の追加はif文で「最後じゃなかったら追加する」ようにすればよい。

最後に忘れずに、作った答え用の文字列を `cout` で出力しておしまい。

## 入力例1での動作

`S="mtr"` を受け取る。
`result` は空文字列で初期化する。

1文字ずつ順に見ていく。

$i=0$ では、`s[0]='m'` を追加して `result="m"` となる。
最後の文字ではないので、さらに `'o'` を追加して `result="mo"` となる。

$i=1$ では、`s[1]='t'` を追加して `result="mot"` となる。
最後の文字ではないので、さらに `'o'` を追加して `result="moto"` となる。

$i=2$ では、`s[2]='r'` を追加して `result="motor"` となる。
最後の文字なので、`'o'` は追加しない。

最後に、`result` の `motor` を出力する。

## 注意点

特になし。

## 別解

特になし。
