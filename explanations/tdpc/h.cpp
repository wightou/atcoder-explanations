#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  // 変数かぶりがあるので、重量制限をMで、色数制限をKで扱う
  int n, m, k;
  cin >> n >> m >> k;

  vector<int> w(n), v(n), c(n);
  for (int i=0; i<n; i++) {
    cin >> w.at(i) >> v.at(i) >> c.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 荷物の色ごとにバケツソートしておく
  vector<vector<pair<int,int>>> items((*max_element(c.begin(),c.end()))+1);
  for (int i=0; i<n; i++) {
    items.at(c.at(i)).emplace_back(w.at(i),v.at(i));
  }

  // DPテーブル
  // dp[i][j]は、i色以内で重さj以下での最大価値
  vector<vector<int>> dp(k+1,vector<int>(m+1,0));

  // 色1つごとに見るループ
  for (auto& vec : items) {

    // その色の荷物がないならスキップ
    if (vec.empty()) continue;

    // 色数ごとに見るループ
    // 同じ荷物を2回以上使わないように、色数が多い方から処理する
    for (int i=k; i>0; i--) {

      // 更新処理用一時データに、色数が1つ少ないところのデータをコピー
      vector<int> tmp = dp.at(i-1);

      // その色の全ての荷物について、普通のナップサック問題のようにtmpを更新
      for (auto [weight,value] : vec) {
        for (int j=m; j>=weight; j--) {
          tmp.at(j) = max(tmp.at(j),tmp.at(j-weight)+value);
        }
      }

      // tmpの結果をその色数のところへ反映
      for (int j=0; j<=m; j++) {
        dp.at(i).at(j) = max(dp.at(i).at(j),tmp.at(j));
      }

    }

  }

  // k色以下、重量m以下でのデータが答え
  result = dp.at(k).at(m);

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}