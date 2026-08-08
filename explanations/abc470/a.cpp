#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  // 整数を1つ用意し、入力を受け取る
  int n;
  cin >> n;

  //////////////////// 出力 ////////////////////

  // 1からnまで全部をループに渡す
  for (int i=1; i<=n; i++) {

    // iが3の倍数か判定
    if (i%3) {

      // 3の倍数でないなら、そのまま出力
      cout << i << endl;

    } else {

      // 3の倍数なら、"Fizz" を出力
      cout << "Fizz" << endl;

    }

  }

  //////////////////// 終了 ////////////////////

  return 0;

}