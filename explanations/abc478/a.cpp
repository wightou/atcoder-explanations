#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////
  
  // 整数を2つ用意し、入力を受け取る
  int n, m;
  cin >> n >> m;

  //////////////// 出力変数定義 ////////////////

  // 長さnのvectorをすべて0で初期化しておく
  vector<int> result(n,0);

  //////////////////// 処理 ////////////////////

  // 0-indexedである場合、i個目のぶどうは、iをnで割った余りのところへ配られる
  // それをシミュレーションする
  for (int i=0; i<m; i++) {
    result.at(i%n)++;
  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}