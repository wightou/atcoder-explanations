#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  // Wの文字がかぶっているので、目標値はmとする
  int n, m;
  cin >> n >> m;
  
  vector<int> w(n), v(n);
  for (int i=0; i<n; i++) {
    cin >> w.at(i) >> v.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // DPテーブル
  // dp.at(j)は、総重量j以下での最大価値
  vector<long long> dp(m+1,0);

  // 0/1ナップサック問題なので、後ろから更新する
  for (int i=0; i<n; i++) {
    for (int j=m; j>=w.at(i); j--) {
      dp.at(j) = max(dp.at(j),dp.at(j-w.at(i))+v.at(i));
    }
  }

  // DPテーブルの一番後ろの値が答え
  result = dp.back();

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}