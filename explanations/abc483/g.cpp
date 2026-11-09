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

long long perm(int n, int r) {
  if (r<0) return 0;
  if (r>n) return 0;
  long long result = factorial[n];
  result *= fact_inv[n-r];
  result %= mod;
  return result;
}

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////

  // 1000000までの階乗とその逆数を用意
  make_factorial(1000000);

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int n, k;
    cin >> n >> k;

    //////////////// 出力変数定義 ////////////////

    long long result = 0;

    //////////////////// 処理 ////////////////////

    // P固定時、両方で上位k人にいる人がi人である場合の寄与は、i=0のとき0、i>0のとき以下
    // kCi×kPi×{(n-k)P(k-i)}^2×i×(n-2k)×(n-2k+i-1)!
    for (int i=1; i<=k; i++) {
      long long tmp = 1;
      tmp *= perm(n-k,k-i);
      tmp *= tmp;
      tmp %= mod;
      tmp *= comb(k,i);
      tmp %= mod;
      tmp *= perm(k,i);
      tmp %= mod;
      tmp *= i;
      tmp %= mod;
      tmp *= n-2*k;
      tmp %= mod;
      tmp *= factorial.at(n-2*k+i-1);
      tmp %= mod;
      result += tmp;
    }

    // その総和にさらにPの並び順の数n!をかけたものが答え
    result %= mod;
    result *= factorial.at(n);
    result %= mod;

    // 負数の場合の対応
    if (result<0) result++;

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}