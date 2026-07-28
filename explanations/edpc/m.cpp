#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 1000000007;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, k;
  cin >> n >> k;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // DPテーブル
  // dp[i] は、ここまで見た子供たちに合計i個の飴を配る方法数
  vector<int> dp(k+1,0);
  dp.at(0) = 1;

  // 子供を1人ずつ増やす
  for (int j=0; j<n; j++) {

    // 更新用の区間和
    int sum = accumulate(dp.end()-a.at(j)-1,dp.end()-1,0LL)%mod;

    // 各飴の個数ごとに右から処理する
    for (int i=k; i>=0; i--) {

      // テーブル更新
      dp.at(i) += sum;
      dp.at(i) %= mod;

      // 区間和更新
      if (i>=1) {
        sum -= dp.at(i-1);
        sum %= mod;
      }
      if (i>=a.at(j)+1) {
        sum += dp.at(i-a.at(j)-1);
        sum %= mod;
      }

    }

  }

  // 全ての子供を処理した後の、テーブル末尾の値が答え（負数対応もする）
  result = dp.back();
  if (result<0) result += mod;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}