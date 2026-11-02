#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  // 文字列を用意し、入力を受け取る
  string s;
  cin >> s;

  //////////////// 出力変数定義 ////////////////

  // "even" で初期化しておくことで、奇数だった時に "odd" に変えるだけでよくする
  string result = "even";

  //////////////////// 処理 ////////////////////

  // カウンターを用意して、いきなり 'o' の個数で初期化
  int counter = count(s.begin(),s.end(),'o');

  // カウンターが奇数だったら、"odd" に書き換え
  if (counter%2==1) result = "odd";

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}