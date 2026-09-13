#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// 指数計算
long long power(long long a, long long p) {
  assert(p>=0);
  long long result = 1;
  long long b = a;
  while (p>0) {
    if (p&1LL) result = result*b;
    p >>= 1;
    if (p>0) b = b*b;
  }
  return result;
}

// 部分的素因数分解（70未満のみ）
vector<pair<long long,int>> prime_factorization(long long n) {
  assert(n>0);
  vector<pair<long long,int>> primes;
  long long p = 2;
  while (p*p<=n&&p<70) {
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

// 素因数分解で指数が降順になる数の探索
// ただし、1つだけ例外にしてもよく、その場合はaの該当素数を-1にしてあり、該当素数分をnumとcountに入れてある
void calc (const vector<long long>& a, long long num, int count, int idx, int m, long long lim, vector<pair<long long,int>>& result) {
  if (a.at(idx)==-1) idx++;
  if (num>lim) return;
  if (m==0) {
    result.emplace_back(num,count);
    return;
  }
  long long p = 1;
  for (int i=0; i<=m; i++) {
    if (num>lim/p) break;
    calc(a,num*p,count*(i+1),idx+1,i,lim,result);
    p *= a.at(idx);
  }
}

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////



  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    long long n, d;
    cin >> n >> d;

    //////////////// 出力変数定義 ////////////////

    long long result = 0;

    //////////////////// 処理 ////////////////////

    // dを素因数分解する
    // ただし、70以上の素数は無視し、番兵で101を追加しておく
    vector<pair<long long,int>> primes = prime_factorization(d);
    primes.emplace_back(101,1);

    // 70未満の素因数一覧
    vector<long long> a = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67};
    
    // 探索すべき値とその約数の数の組の一覧
    vector<pair<long long,int>> vec;

    // とりあえず、素因数分解で指数が完全に降順になる数を探索（dの倍数も混ざる）
    calc(a,1,1,0,60,n,vec);

    // aの1要素を一時的に封印するためのブロッカー
    long long blocker = -1;

    // dの素因数一覧用のインデックス
    int id = 0;

    // 70未満の素数について探索
    for (int i=0; i<ssize(a); i++) {

      // dの素因数にないならスキップ
      if (primes.at(id).first!=a.at(i)) continue;

      // 該当素因数を一時封印して、dより1少ない指数に固定した例外入り探索
      swap(a.at(i),blocker);
      calc(a,pow(primes.at(id).first,primes.at(id).second-1),primes.at(id).second,0,60,n,vec);
      swap(a.at(i),blocker);
      id++;

    }

    // 個数最大値
    int mx = 0;
    for (auto [num,count] : vec) {

      // dの倍数だったら排除
      if (num%d==0) continue;

      // 個数が最大だったら、更新する
      if (mx<count) {
        mx = count;
        result = num;
      }

    }

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}