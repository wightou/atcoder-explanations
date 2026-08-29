#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  // 整数とvectorを1つずつ用意し、入力を受け取る
  int n;
  cin >> n;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  // 0で初期化しておき、ここに合計していく
  int result = 0;

  //////////////////// 処理 ////////////////////

  // スタート位置を工夫したforループで合計する
  for (int i=n/2; i<n; i++) {
    result += a.at(i);
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}