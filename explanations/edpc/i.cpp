#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<double> p(n);
  for (int i=0; i<n; i++) {
    cin >> p.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  double result = 0.0;

  //////////////////// 処理 ////////////////////

  // DPテーブル
  // dp[i] は、表がi個になる確率
  // ただし、dp[(n+1)/2] は、(n+1)/2 個以上（つまり表の方が多いことが確定）分の合計
  vector<double> dp((n+3)/2,0.0);
  dp.at(0) = 1.0;

  // コイン1枚ごとに、確率を更新
  // 1枚で2つ進まないよう、右から更新
  // また、既に (n+1)/2 個出ているものはこれ以上更新しない
  for (int i=0; i<n; i++) {
    for (int j=min(i,(n-1)/2); j>=0; j--) {
      dp.at(j+1) += dp.at(j)*p.at(i);
      dp.at(j) *= (1.0-p.at(i));
    }
  }

  // (n+1)/2 個以上出ている確率が答え
  result = dp.back();

  //////////////////// 出力 ////////////////////

  cout << fixed << setprecision(10) << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}