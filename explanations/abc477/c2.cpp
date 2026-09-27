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

  // tの存在位置一覧
  vector<int> pos;
  
  // s上のスタート位置を全探索する
  for (int i=0; i<ssize(s)-ssize(t)+1; i++) {
    
    // tと一致しているか
    bool flag = true;

    // 1文字ずつ見て、tと不一致だったらfalseにする
    for (int j=0; j<ssize(t); j++) {
      if (s.at(i+j)!=t.at(j)) {
        flag = false;
        break;
      }
    }
    
    // trueだったら、posに追加
    if (flag) pos.emplace_back(i);

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

    // 二分探索で、範囲内の値が1つでもあるかを確認（本来はD問題レベル）
    auto it1 = lower_bound(pos.begin(),pos.end(),l);
    auto it2 = upper_bound(pos.begin(),pos.end(),r-ssize(t)+1);
    if (it2-it1>0) result = "Yes";

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}