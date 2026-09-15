#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;

// 累乗剰余
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

// 階乗剰余
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

// 二項係数剰余
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
long long comb_inv(int n, int r) {
  assert(r>=0);
  assert(r<=n);
  assert(n<mod);
  long long result = fact_inv[n];
  result *= factorial[r];
  result %= mod;
  result *= factorial[n-r];
  result %= mod;
  return result;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 階乗データを用意
  make_factorial(m+n);

  // 3パターンに場合分けして、ソート済のBのパターン数を求める
  if (m<n-1) result = 0;
  else if ((m-n)%2) result = comb((m+n-1)/2,n)+comb((m+n+1)/2,n);
  else result = 2*comb((m+n)/2,n);

  // N!倍して、Aのパターン数にする
  result %= mod;
  result *= factorial.at(n);
  result %= mod;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}