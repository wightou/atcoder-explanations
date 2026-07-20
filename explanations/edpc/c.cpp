#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> a(n), b(n), c(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i) >> b.at(i) >> c.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // DPテーブル（インライン更新）
  // 中身は「その日にAをやる場合の幸福度最大値、同じくB、同じくC」
  // 初日分で初期化しておく
  vector<int> dp = {a.at(0),b.at(0),c.at(0)};

  // 初日以降の分を更新していく
  for (int i=1; i<n; i++) {
    dp = {max(dp.at(1),dp.at(2))+a.at(i),max(dp.at(0),dp.at(2))+b.at(i),max(dp.at(0),dp.at(1))+c.at(i)}; 
  }

  // 最終日には何をやってもいいので、3つの行動から最大値を探す
  result = *max_element(dp.begin(),dp.end());

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}