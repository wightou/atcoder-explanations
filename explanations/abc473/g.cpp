#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;

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

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, k;
  cin >> n >> k;

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 階乗を準備しておく
  make_factorial(n);

  // 畳み込みで掛け算したい一次式を全部入れる
  // i回目（0-indexed）の新規カードで、一発で目的のものを引くパターン（1通り）と、失敗するパターン（n-i-1通り）
  deque<vector<long long>> que;
  vector<long long> sample = {1,n};
  for (int i=0; i<n; i++) {
    sample.at(1)--;
    que.emplace_back(sample);
  }

  // 全部かける
  // 長いのを何度も扱うと計算量が重いので、その時点で短い方から2つをかけることを繰り返す
  while (ssize(que)>1) {
    que.emplace_back(convolution(que.at(0),que.at(1)));
    que.pop_front();
    que.pop_front();
  }
  
  // n!パターンのうち、失敗がk-n回あるものの割合が答え
  if (k<2*n) result = que.at(0).at(k-n)*fact_inv.at(n)%mod;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}