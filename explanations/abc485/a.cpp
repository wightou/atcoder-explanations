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

  // 整数を2つ用意し、入力を受け取る
  int a, b;
  cin >> a >> b;

  //////////////// 出力変数定義 ////////////////

  // "No" で初期化しておくことで、同じ側だった時に "Yes" に変えるだけでよくする
  string result = "No";

  //////////////////// 処理 ////////////////////

  // 同じ符号だったら "Yes"
  // これはかけて正かどうかで判定できる
  if (a*b>0) result = "Yes";

  //////////////////// 出力 ////////////////////

  cout << result << '\n';

  //////////////////// 終了 ////////////////////

  return 0;

}