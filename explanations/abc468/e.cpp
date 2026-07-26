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

  vector<long long> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 累積和を作っておく
  vector<long long> sum(n+1);
  partial_sum(a.begin(),a.end(),sum.begin()+1);

  // 今考えている長さの区間の総和の総和
  long long s = 0;

  // 長さ順に考える
  for (int i=1; i<=n; i++) {

    // 今考えている長さの区間の総和の総和を更新
    // 実は、ある区間分を増減するだけでよく、n+1-iとi-1の大小が逆転してもこれでよい
    s += sum.at(n+1-i)-sum.at(i-1);
    s %= mod;

    // s/i を結果に足しこむ
    result += (s*(power_mod(i,-1)))%mod;
    result %= mod;

  }

  // resultが負数になっていた場合の対応
  if (result<0) result += mod;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}