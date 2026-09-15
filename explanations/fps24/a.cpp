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

  int d, n;
  cin >> d >> n;

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 階乗データを用意
  make_factorial(d);

  // 事前の手計算により、(1+x^3)^d*(1+x^2)^dのx^(n-d)の係数が答え
  // x^3をi回取るとすると、x^2は(n-d-3i)/2回取ればよい
  for (int i=0; i<=d&&3*i<=n-d; i++) {

    // (n-d-3i)/2が整数にならないなら、スキップ
    if ((n-d-3*i)%2) continue;

    // x^2を取る回数
    int j = (n-d-3*i)/2;

    // 二項係数の積を足していく
    result += comb(d,i)*comb(d,j);
    result %= mod;

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}