#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;
  
  vector<vector<int>> f(n,vector<int>(n));
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      cin >> f.at(i).at(j);
    }
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // DPテーブル
  // dp[j] は、末尾の猫から左1以内にいる猫の最小番号がjの場合の最大スコアの半分
  vector<int> dp(n);

  // 猫を1匹ずつ追加する
  for (int i=0; i<n; i++) {

    // 追加した猫の幸福度
    int sum = accumulate(f.at(i).begin(),f.at(i).begin()+i,0);

    // 更新前の dp[0] から dp[j] までの最大値
    int mx = dp.at(0);

    // DPテーブル更新
    for (int j=0; j<=i; j++) {

      // mxを先に更新する
      mx = max(mx,dp.at(j));

      // DPテーブルを更新
      dp.at(j) = mx+sum;

      // j番の猫をi番の猫の幸福度から外す
      sum -= f.at(i).at(j);

    }
  }

  // 結果は DP テーブル内最大値の倍
  result = *max_element(dp.begin(),dp.end())*2;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}