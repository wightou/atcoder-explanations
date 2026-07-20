#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  // 入力を受け取る
  string s;
  cin >> s;

  // 文字列の長さも取得しておく
  int n = s.size();

  //////////////// 出力変数定義 ////////////////

  string result = "";

  //////////////////// 処理 ////////////////////

  // 1文字ずつ見るループ
  for (int i=0; i<n; i++) {

    // 数字だったら、出力用文字列にくっつける
    if (isdigit(s.at(i))) result += s.at(i);

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}