#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 1000000007;

// 指数計算
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

// 階乗とその逆元
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

// 二項係数とその逆数
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

  int h, w, n;
  cin >> h >> w >> n;

  vector<int> r(n), c(n);
  for (int i=0; i<n; i++) {
    cin >> r.at(i) >> c.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 階乗データを作成
  make_factorial(h+w);

  // スタートとゴールを含めて、座標をソートする
  vector<pair<int,int>> vec(n+2);
  for (int i=0; i<n; i++) {
    vec.at(i) = {r.at(i),c.at(i)};
  }
  vec.at(n) = {1,1};
  vec.at(n+1) = {h,w};
  sort(vec.begin(),vec.end());

  // DPテーブル1 指定マスを奇数個通るパターン数
  vector<long long> odd(n+2,0);
  odd.at(0) = 1;

  // DPテーブル2 指定マスを偶数個通るパターン数
  vector<long long> even(n+2,0);

  // 1つずつテーブルを埋める
  for (int i=1; i<=n+1; i++) {

    // 若い番号の注目点を全部見る
    for (int j=0; j<i; j++) {

      // 座標の差を見て、左上になっていなかったら飛ばす
      int dr = vec.at(i).first-vec.at(j).first;
      int dc = vec.at(i).second-vec.at(j).second;
      if (dr<0||dc<0) continue;

      // j のところ経由で i のところに来るパターン数
      // 個数の偶奇が入れ替わる
      long long tmp = comb(dr+dc,dc);
      odd.at(i) += even.at(j)*tmp%mod;
      even.at(i) += odd.at(j)*tmp%mod;

    }

    // 剰余を取っておく
    odd.at(i) %= mod;
    even.at(i) %= mod;

  }

  // 包除原理より、ゴールマスの偶数個通るパターン数から奇数個のものを引いたら答え
  // 負数対応もする
  result = (even.at(n+1)-odd.at(n+1))%mod;
  if (result<0) result += mod;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}