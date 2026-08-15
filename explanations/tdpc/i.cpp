#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  string s;
  cin >> s;

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 番兵として "ww" を足しておく
  s += "ww";

  // 番兵を含めた文字数を取得しておく
  int n = ssize(s);

  // 'i' と 'w' の数のカウント
  int count_i = 0;
  int count_w = 0;

  // 前から1文字ずつ見るループ
  for (int i=0; i<n; i++) {

    // そこが、'i'か、'i'の次の'w'か、それ以外かで分ける
    if (s.at(i)=='i') {

      // 'i'だったら、その個数をカウント
      count_i++;

    } else if (i>0&&s.at(i-1)=='i') {

      // 'i'の次の'w'だったら、その個数をカウント
      count_w++;

    } else {

      // 'w'の次または先頭の'w'だったら、その直前のブロックでの削除数を足す
      result += min(max(0,count_w-1),count_i/2);
      count_i = 0;
      count_w = 0;

    }

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}