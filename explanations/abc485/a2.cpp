#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// ライブラリ https://github.com/wightou/library-public

/////////////////// メイン ///////////////////

int main () {

  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  //////////////////// 入力 ////////////////////

  // 文字列を2つ用意し、入力を受け取る
  string a, b;
  cin >> a >> b;

  //////////////// 出力変数定義 ////////////////

  // "Yes" で初期化しておくことで、逆側だった時に "No" に変えるだけでよくする
  string result = "Yes";

  //////////////////// 処理 ////////////////////

  // '-'の個数を数える
  int count = 0;
  for (char c : a) {
    if (c=='-') count++;
  }
  for (char c : b) {
    if (c=='-') count++;
  }

  // 逆符号だったら "No"
  // これは'-'が1つだけかどうかで判定できる
  if (count==1) result = "No";

  //////////////////// 出力 ////////////////////

  cout << result << '\n';

  //////////////////// 終了 ////////////////////

  return 0;

}