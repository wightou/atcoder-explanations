#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  // 文字を用意し、入力を受け取る
  char c;
  cin >> c;

  //////////////// 出力変数定義 ////////////////

  char result = 0;

  //////////////////// 処理 ////////////////////

  // もし'B'だったら'Y'にする 
  if (c=='B') result = 'Y';

  // それ以外で、もし'Y'だったら'R'にする 
  else if (c=='Y') result = 'R';

  // それ以外で、もし'R'だったら'B'にする 
  else if (c=='R') result = 'B';

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}