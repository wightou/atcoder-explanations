---
contest: TDPC
problem: E
problem_title: "数"
problem_url: https://atcoder.jp/contests/tdpc/tasks/tdpc_number
submission_url: https://atcoder.jp/contests/tdpc/submissions/78345000
tags:
  - 動的計画法
  - 桁DP
  - 剰余類環
tag_note: 簡単なレベルの内容は省略。
---

## 考え方

ABCでいうと、E問題級。

EDPCのS問題と同じなので、あちらを参照。
相違点は、以下の $3$ つの非常に細かい点だけである。
- 変数名が $K$ か $N$ か
- その範囲が $10^{10000}$ 「以下」か「未満」か
- 入力の順序

計算量は $O(|N|D)$。