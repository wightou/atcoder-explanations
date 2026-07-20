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

  vector<string> result;

  //////////////////// 処理 ////////////////////

  // 最も左/右/上/下の黒マスの位置を記録する変数
  // lとuは十分大きな値、rとuは十分小さな値で初期化しておく
  int l = w;
  int r = -1;
  int u = h;
  int d = -1;

  // 全マスをチェックし、上下左右の新記録だったら更新する
  for (int i=0; i<h; i++) {
    for (int j=0; j<w; j++) {
      if (s.at(i).at(j)=='#') {
        l = min(l,j);
        r = max(r,j);
        u = min(u,i);
        d = max(d,i);
      }
    }
  }

  // u行目からd行目までの、l文字目からr文字目までをresultに詰めていく
  for (int i=u; i<=d; i++) {
    result.emplace_back(s.at(i).substr(l,r-l+1));
  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}