#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int m, d;
  string s;
  cin >> m >> d >> s;

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 1文字ずつ前から見ていく
  for (int i=0; i<m; i++) {

    // 監視されていないマスかどうかを記録する変数
    bool flag = true;

    // Dマス以内にガードマンがいたら、監視されている
    // jの範囲がはみ出ないように工夫をする
    for (int j=max(0,i-d); j<m&&j<=i+d; j++) {
      if (s.at(j)=='G') flag = false;
    }

    // 監視されていないマスだったら、数える
    if (flag) result++;

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}