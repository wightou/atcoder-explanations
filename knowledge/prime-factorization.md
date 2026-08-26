---
title: 素因数分解
level: D問題相当
category: その他数学系
summary: 与えられた数を素数の積に分解する。
category_order: 9999
level_order: 9999
aliases: []
absorbs: []
related:
  - 素数判定
  - エラトステネスの篩
  - 約数と倍数
---

## 概要

与えられた数を素数の積に分解するアルゴリズム。
毎回 $O(\sqrt{n})$ かかるものと、前処理に $O(n\log\log n)$ かかるがその後は毎回 $O(\log n)$ ですむものがある。

最大 $10^{12}$ くらいの数を分解する必要があるが、$1$ 回だけでいい場合は前者を用いる。
最大 $10^7$ くらいでいいが、大量に分解を実行する必要がある場合は後者を用いる。

## アルゴリズム内容1

素数判定と同じ要領で $2$ から小さい順に割っていく。
割りきれる場合、それが何回割れるか確認し、出力に詰める。
その際、一度調べた素数については二度と調べないので、本当に割ってしまうとよい。
平方が残っている数より大きくなれば残ったものは素数である。
$O(\sqrt{n})$ で済むが、計算結果の使いまわしが効かないので、大量に素因数分解するには不向き。

C++での実際の参考コードは以下。

```cpp
vector<pair<long long,int>> prime_factorization(long long n) {
  assert(n>0);
  vector<pair<long long,int>> primes;
  long long p = 2;
  while (p*p<=n) {
    int counter = 0;
    while (n%p==0) {
      counter++;
      n /= p;
    }
    if (counter>0) primes.emplace_back(p,counter);
    if (p&1) p += 2;
    else p++;
  }
  if (n>1) primes.emplace_back(n,1);
  return primes;
}
```

## アルゴリズム内容2

エラトステネスの篩により $n$ 以下の全ての数の最小素因数を求めるアルゴリズムを利用する。
（「エラトステネスの篩」の記事参照。）

その結果を利用すれば次に割れる素数は何か、それで何回割れるかもすぐわかる。
毎回の処理が $O(\log n)$ で済むが、最初に $O(n\log\log n)$ かかる準備が必要になる。
$10^9$ クラスの $n$ が混ざっていると苦しい。

C++での実際の参考コードは以下。
上半分は最小素因数を求めるもの。

```cpp
vector<int> s_primes;
void make_s_primes (int n) {
  assert(n>=0);
  s_primes.assign(n+1,-1);
  bool flag = true;
  for (int i=2; i<=n; i++) {
    if (s_primes[i]!=-1) continue;
    s_primes[i] = i;
    if (!flag) continue;
    for (int j=i*i; j<=n; j+=i) {
      if (s_primes[j]==-1) s_primes[j] = i;
    }
    if (i*i>n) flag = false;
  }
}
vector<pair<int,int>> prime_factorization(int n) {
  assert(n>0);
  assert(n<ssize(s_primes));
  vector<pair<int,int>> result;
  while (n>1) {
    if (!result.empty()&&result.back().first==s_primes[n]) result.back().second++;
    else result.emplace_back(s_primes[n],1);
    n /= s_primes[n];
  }
  return result;
}

// main関数内
make_s_primes(n);
```

main関数内で、上限を指定して最小素因数一覧を構築する必要があることに注意。

## 関連知識

### 素数判定

素数かどうか判定するだけで、分解はしないやつ。

### エラトステネスの篩

$n$ 未満の全ての素数を洗い出すアルゴリズム。
分解したい個数が多いなら、あちらを使った方が全体としては速い場合もある。

### 約数と倍数

素因数分解を利用して、約数の個数や総和を効率的に求めることができる。
