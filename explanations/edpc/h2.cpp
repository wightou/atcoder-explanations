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

  // DPテーブル（インライン）
  // dp[i] は、その行の i マス目にたどり着く方法の数
  vector<int> dp(w,0);
  dp.at(0) = 1;

  // 各行ごと、テーブルを更新する
  // 同じ行で複数回右移動してもよいので、更新は左から
  for (int i=0; i<h; i++) {
    for (int j=0; j<w; j++) {
      if (a.at(i).at(j)=='#') dp.at(j) = 0;
      else if (j>0) dp.at(j) += dp.at(j-1);
      dp.at(j) %= mod;
    }
  }

  // 全て更新が終わった後の、最右列の値が答え
  result = dp.back();

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}