#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;

  vector<int> c(n), s(n);
  for (int i=0; i<n; i++) {
    cin >> c.at(i) >> s.at(i);
    c.at(i)--;   // 0-index にしておく
  }

  //////////////// 出力変数定義 ////////////////

  // 長さmの-1で初期化された配列として用意する
  // ここに「今のところの最大値」を書き込んでいけば、1つもないものは自然と-1になる
  vector<int> result(m,-1);

  //////////////////// 処理 ////////////////////

  // ボールを1つずつ見ていくループ
  for (int i=0; i<n; i++) {

    // その色に対応するところを、最大値更新する
    result.at(c.at(i)) = max(result.at(c.at(i)),s.at(i));

  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i);
    if (i!=result.size()-1) {
      cout << " ";
    }
  }
  cout << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}