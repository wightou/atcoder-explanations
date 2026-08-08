#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;

// 累乗mod（マイナス乗対応）
long long power_mod(long long a, long long p) {
  if (p==0) return 1LL;
  a %= mod;
  if (a<0) a += mod;
  if (p<0) {
    assert(gcd(a,mod)==1);
    p = (p%(mod-1))+mod-1;
  }
  long long result = 1;
  long long b = a;
  while (p>0) {
    if (p&1LL) result = result*b%mod;
    b = b*b%mod;
    p >>= 1;
  }
  return result;
}

// 階乗をその逆元
vector<long long> factorial;
vector<long long> fact_inv;
void make_factorial(int n, bool make_inv = true) {
  assert(n>=0);
  if (make_inv) assert(n<mod);
  factorial.assign(n+1,1);
  for (int i=1; i<=n; i++) {
    factorial[i] = factorial[i-1]*i%mod;
  }
  if (!make_inv) return;
  fact_inv.assign(n+1,power_mod(factorial[n],-1));
  for (int i=n; i>0; i--) {
    fact_inv[i-1] = fact_inv[i]*i%mod;
  }
}

// 二項係数
long long comb(int n, int r) {
  if (r<0) return 0;
  if (r>n) return 0;
  long long result = factorial[n];
  result *= fact_inv[r];
  result %= mod;
  result *= fact_inv[n-r];
  result %= mod;
  return result;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  string s;
  cin >> n >> m >> s;

  vector<int> a(m), b(m);
  for (int i=0; i<m; i++) {
    cin >> a.at(i) >> b.at(i);
    a.at(i)--;
    b.at(i)--;
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 1;

  //////////////////// 処理 ////////////////////

  // 階乗を作る
  make_factorial(n);

  // UnionFind木で連結性判定
  dsu d(n);
  for (int i=0; i<m; i++) {
    d.merge(a.at(i),b.at(i));
  }

  // 同じ文字同士の入れ替えが発生する可能性があるか
  bool flag = false;

  // 連結成分ごとに計算
  vector<vector<int>> g = d.groups();
  for (const vector<int>& vec : g) {

    // その連結成分にどの文字が何個あるか
    vector<int> counters(26);
    for (int i : vec) {
      counters.at(s.at(i)-'a')++;
    }

    // その連結成分内での並べ方の総数をresultにかける
    int num = ssize(vec);
    for (int i=0; i<26; i++) {
      if (counters.at(i)>0) {
        result *= comb(num,counters.at(i));
        result %= mod;
        num -= counters.at(i);
        if (counters.at(i)>1) flag = true;
      }
    }
    
  }

  // 同じ文字の入れ替えが絶対にできない場合、偶数回目にできるものは半分しかない
  if (!flag) {
    result *= power_mod(2,-1);
    result %= mod;
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}