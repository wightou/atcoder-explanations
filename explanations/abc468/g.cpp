#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;

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

// 階乗とその逆数
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

  int n;
  string s;
  cin >> n >> s;

  //////////////// 出力変数定義 ////////////////

  // 先頭と末尾が 'o' じゃない場合、明らかに無理なので、もう 0 と答えてしまう
  if (s.front()!='o'||s.back()!='o') {
    cout << 0 << endl;
    return 0;
  }

  // n が 1 の場合、明らかに 1 通りなので、後の処理の RE 防止で先に答えてしまう
  if (n==1) {
    cout << 1 << endl;
    return 0;
  }

  long long result = 1;

  //////////////////// 処理 ////////////////////

  // 階乗データを作る
  make_factorial(n);

  // DPテーブル
  // 0-indexedで考える
  // dp[i][j] は、0からiまで並べる上で、0 番目の次に 'o' が出てくるのが j 番目であるパターン数
  // dp[0][j] や dp[i][0] はダミーデータ
  vector<vector<long long>> dp(n+1,vector<long long>(n,0));

  // DPのスタートを埋める
  dp.at(1).at(1) = 2;

  // i>=2のパターン埋め
  for (int i=2; i<n; i++) {

    // 'o' が最初と最後のみ、というパターン数を、とりあえず 0 から i までの全並び替え数で初期化
    dp.at(i).at(i) = factorial.at(i+1);

    // j<i の場合、どこかに dp[j][j] 通りのどれかの並びで 0 から j までがいればよい
    // それに該当するパターンを、dp[i][i] から dp[i][j] に移植 
    for (int j=1; j<i; j++) {
      dp.at(i).at(j) = dp.at(j).at(j)*factorial.at(i-j+1)%mod;
      dp.at(i).at(i) -= dp.at(i).at(j);
    }

    // dp[i][i] を適切な範囲に収めておく
    dp.at(i).at(i) %= mod;
  }

  // 前の 'o' から何文字経過したか数えるカウンター
  int counter = 0;

  // 先頭を除いて前から見ていく
  for (int i=1; i<n; i++) {

    // 1文字経過
    counter++;

    // 'o' だったら、前の 'o' までを 1 つのデータとしてみることで、パターン数が求まる
    // 最後の 1 文字は、コードがここまで実行されている時点で必ず 'o' なので、終了時の特殊処理は不要
    if (s.at(i)=='o') {
      result *= dp.at(counter).at(counter);
      result %= mod;
      counter = 0;
    }
  }

  // resultが負数になっていた場合の対応
  if (result<0) result += mod;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}