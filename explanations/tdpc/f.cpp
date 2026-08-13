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

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // DPテーブル
  // dp[i]は、i番目の駅までルール通りに進んでそこを通過するパターン数
  // 先頭と末尾に、番兵として通過する駅を置いておく
  vector<int> dp(n+2,0);

  // sliding windowの区間和
  int sum = 0;

  // DPテーブルを更新
  // 0は確定で通過、1とnは確定で停車
  for (int i=0; i<n+2; i++) {

    // テーブルを埋める
    if (i>1&&i!=n) dp.at(i) = sum; 
    else if (i==0) dp.at(i) = 1;
    else dp.at(i) = 0;

    // windowをずらして区間和更新（負の数対応もする）
    sum += dp.at(i);
    if (i>=k) sum -= dp.at(i-k);
    sum %= mod;
    if (sum<0) sum += mod;

  }

  // 番兵で置いたn+1での値が答え
  result = dp.back();

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}