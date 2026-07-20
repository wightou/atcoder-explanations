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

  //////////////// 出力変数定義 ////////////////

  string result = "";

  //////////////////// 処理 ////////////////////

  // 1文字ずつ見るループ
  for (char c : s) {

    // 数字だったら、出力用文字列にくっつける
    if (isdigit(c)) result += c;

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}