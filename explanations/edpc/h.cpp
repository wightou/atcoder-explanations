#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 1000000007;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int h, w;
  cin >> h >> w;

  vector<string> a(h);
  for (int i=0; i<h; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // DPテーブル（インラインじゃない）
  // dp[i][j] は、i 行目の j マス目にたどり着く方法の数
  vector<vector<int>> dp(h,vector<int>(w,0));
  dp.at(0).at(0) = 1;

  // テーブルを更新する
  for (int i=0; i<h; i++) {
    for (int j=0; j<w; j++) {
      if (a.at(i).at(j)=='#') dp.at(i).at(j) = 0;
      else {
        if (i>0) dp.at(i).at(j) += dp.at(i-1).at(j);
        if (j>0) dp.at(i).at(j) += dp.at(i).at(j-1);
      }
      dp.at(i).at(j) %= mod;
    }
  }

  // 全て更新が終わった後の、最下段最右列の値が答え
  result = dp.back().back();

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}