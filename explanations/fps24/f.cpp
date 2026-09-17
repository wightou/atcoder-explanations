#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;

long long power_mod(long long a, long long p) {
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

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // {3^N-(-1)^N}/2^2 が答え
  result = (power_mod(3,n)-power_mod(-1,n))*(power_mod(2,-2));
  result %= mod;
  if (result<0) result += mod;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}