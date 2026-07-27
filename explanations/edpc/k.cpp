#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

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

  // "Second" で初期化しておく
  string result = "Second";

  //////////////////// 処理 ////////////////////

  // DPテーブル
  // dp[i]は、i個の状態が先手必勝かどうか
  vector<bool> dp(k+1,false);

  // 個数が少ない方からどちら必勝なのか確定していく
  for (int i=0; i<=k; i++) {
    for (int s : a) {
      if (i-s>=0&&!dp.at(i-s)) dp.at(i) = true;
    }
  }

  // k個のときに先手必勝なら、result を書き換える
  if (dp.at(k)) result = "First";

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}