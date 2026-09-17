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

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 階乗データを用意
  make_factorial(max(n,m));

  // 畳み込みの結果の配列,最初は1
  vector<long long> f(1,1);

  // かけていく母関数
  // 1+x+(1/2)x^2+(1/6)x^3+……、のx^m項まで
  vector<long long> a(1,1);

  // 1つずつ母関数を作って畳み込んでいく
  for (int i=1; i<=m; i++) {
    a.emplace_back(fact_inv.at(i));
    f = convolution(f,a);
    if (ssize(f)>n) f.resize(n+1);
  }

  // 全部かけ終わった後のx^nの係数にn!をかけたものが答え
  if (ssize(f)<=n) f.resize(n+1,0);
  result = f.at(n)*factorial.at(n)%mod;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}