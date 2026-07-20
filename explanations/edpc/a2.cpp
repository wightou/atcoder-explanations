#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int INF = 1001001001;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> h(n);
  for (int i=0; i<n; i++) {
    cin >> h.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // DPテーブル（最小を取りたいので、すべてINFで初期化しておく）
  // dp.at(i) は、i番目の足場までの最少コスト
  vector<int> dp(n,INF);

  // スタートの足場までのコストは0
  dp.at(0) = 0;

  // 前から順に、次とその次の足場の到達コストが最少記録だったら更新しておく
  // 範囲外にはみ出る場合に注意
  for (int i=0; i<n; i++) {
    if (i+1<n) dp.at(i+1) = min(dp.at(i+1),dp.at(i)+abs(h.at(i+1)-h.at(i)));
    if (i+2<n) dp.at(i+2) = min(dp.at(i+2),dp.at(i)+abs(h.at(i+2)-h.at(i)));
  }
  
  // 最後の足場の値が答え
  result = dp.back();

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}