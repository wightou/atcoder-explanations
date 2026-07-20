#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  // 整数とvectorを1つずつ用意し、入力を受け取る
  int n;
  cin >> n;

  vector<int> x(n);
  for (int i=0; i<n; i++) {
    cin >> x.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  // "Yes"で初期化することで、"No"のときに書き換えるだけで済むようにする
  string result = "Yes";

  //////////////////// 処理 ////////////////////

  // 前から順に、x内の要素を1つずつ確認するループ
  for (int i=0; i<n; i++) {

    // 0以上の値が1つでもあったら"No"
    if (x.at(i)>=0) result = "No";

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}