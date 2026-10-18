#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  // 文字列を2つ用意し、入力を受け取る
  string s1, s2;
  cin >> s1 >> s2;

  //////////////// 出力変数定義 ////////////////

  // "Yes"で初期化しておく
  string result = "Yes";

  //////////////////// 処理 ////////////////////

  // 'A'と'D'が対角線にいたら"No"
  if (s1.at(0)=='A'&&s2.at(1)=='D') result = "No";
  else if (s1.at(1)=='A'&&s2.at(0)=='D') result = "No";
  else if (s2.at(0)=='A'&&s1.at(1)=='D') result = "No";
  else if (s2.at(1)=='A'&&s1.at(0)=='D') result = "No";

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}