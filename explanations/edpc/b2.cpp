#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int INF = 1001001001;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, k;
  cin >> n >> k;

  vector<int> h(n);
  for (int i=0; i<n; i++) {
    cin >> h.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // DPテーブル
  // dp.at(i) は、i番目の足場までの最少コスト
  vector<int> dp(n,INF);

  // スタートの足場までのコストは0
  dp.at(0) = 0;
  
  // 前から順に、そこからk個先までの足場の到達コストが最少記録だったら更新しておく
  // 範囲外にはみ出る場合に注意
  for (int i=0; i<n; i++) {
    for (int j=i+1; j<=min(i+k,n-1); j++) {
      dp.at(j) = min(dp.at(j),dp.at(i)+abs(h.at(j)-h.at(i)));
    }
  }
  
  // 最後の足場の値が答え
  result = dp.back();

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}