#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr long long INF = 1001001001001001001LL;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, k;
  cin >> n >> k;

  vector<int> a(n), b(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i) >> b.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 長さ2K+1のDPテーブルを用意（インライン更新する）
  // i番目は、A区間とB区間の境界をi回まで通過してよい制限下での最大値
  vector<long long> dp(2*k+1,-INF);
  dp.at(0) = 0;

  // カードを1枚ずつ見る
  for (int i=0; i<n; i++) {

    // DPテーブルを後ろから順に更新したい
    for (int j=2*k; j>0; j--) {

      // 偶数回通過か奇数回通過かで加算値をわけながら、実際にテーブル更新
      if (j%2) dp.at(j) = max(dp.at(j),dp.at(j-1))+b.at(i);
      else dp.at(j) = max(dp.at(j),dp.at(j-1))+a.at(i);

    }

    // 0回の場合だけ、テーブルからはみ出ないように特殊処理にする
    dp.at(0) = dp.at(0)+a.at(i);

  }

  // 境界は2K回以下何回でもよいので、DPテーブル全体の最大値が答え
  result = *max_element(dp.begin(),dp.end());

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}