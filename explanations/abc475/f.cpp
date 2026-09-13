#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int h, w;
  cin >> h >> w;

  vector<string> s(h);
  for (int i=0; i<h; i++) {
    cin >> s.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  // 何もしない場合の1をあらかじめ数えておく
  long long result = 1;

  //////////////////// 処理 ////////////////////

  // h>wの場合は、転置を行うことでh<wにする
  if (h>w) {
    vector<string> tmp(w,string(h,'.'));
    for (int i=0; i<h; i++) {
      for (int j=0; j<w; j++) {
        if (s.at(i).at(j)=='#') tmp.at(j).at(i) = '#';
      }
    }
    swap(s,tmp);
    swap(h,w);
  }

  // 上下範囲を選んでループ（上側）
  for (int u=0; u<h; u++) {

    // その上下範囲で、各列に白マスがあるかどうか
    vector<bool> flags(w,false);

    // 上下範囲を選んでループ（下側）
    for (int d=u; d<h; d++) {

      // 白マスがあった列の数
      int count = 0;

      // 最後に上側/下側に白マスがあったのはcountがいくつのときだったか
      int last_u = 0;
      int last_d = 0;

      // 右端位置を左から見ていくループ
      for (int j=0; j<w; j++) {

        // 各列に白マスがあるかどうかを更新
        if (s.at(d).at(j)=='.') flags.at(j) = true;

        // 白マスがないなら、その列は見なかったことにする
        if (!flags.at(j)) continue;

        // 各種値を更新
        count++;
        if (s.at(u).at(j)=='.') last_u = count;
        if (s.at(d).at(j)=='.') last_d = count;

        // 上下とも白マスが少なくとも1個あるような左端を選べる
        result += min(last_u,last_d);

      }

    }

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}