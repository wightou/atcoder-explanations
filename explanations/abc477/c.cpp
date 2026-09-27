#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int q;
  string s, t;
  cin >> q >> s >> t;

  /////////////////// 前処理 ///////////////////

  // s上の各位置、そこより前からスタートするtがいくつあるか
  vector<int> counters(max(0,(int)(ssize(s)-ssize(t)+2)),0);

  // s上のスタート位置を全探索する
  for (int i=0; i<ssize(s)-ssize(t)+1; i++) {

    // 次にコピー
    counters.at(i+1) = counters.at(i);

    // tと一致しているか
    bool flag = true;

    // 1文字ずつ見て、tと不一致だったらfalseにする
    for (int j=0; j<ssize(t); j++) {
      if (s.at(i+j)!=t.at(j)) {
        flag = false;
        break;
      }
    }

    // trueだったら、countersを更新
    if (flag) counters.at(i+1)++;

  }

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int l, r;
    cin >> l >> r;
    l--;
    r--;

    //////////////// 出力変数定義 ////////////////

    string result = "No";

    //////////////////// 処理 ////////////////////

    // スタートとしてあり得る範囲に1つでもあればYes
    // 範囲外になるやつは事前に弾いておく
    if (r-ssize(t)+2>=0&&l<ssize(counters)&&counters.at(r-ssize(t)+2)-counters.at(l)>0) result = "Yes";

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}