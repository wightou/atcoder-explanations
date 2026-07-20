#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  string s;
  cin >> n >> s;

  //////////////// 出力変数定義 ////////////////

  deque<int> que;

  //////////////////// 処理 ////////////////////

  // 今逆転状態かどうか
  bool reversed = false;

  // 「i番目までを反転」を、「i番目まで追加した状態で全体反転」と考える
  // 逆転状態のときには.front()を後ろとみなすことで、毎回「後ろ」に追加する
  for (int i=0; i<n; i++) {
    if (reversed) que.emplace_front(i+1);
    else que.emplace_back(i+1);
    if (s.at(i)=='o') reversed = !reversed;
  }

  // 最終的に反転状態で終わった場合には、前後を実際に合わせる
  if (reversed) reverse(que.begin(),que.end());

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<que.size(); i++) {
    cout << que.at(i);
    if (i!=que.size()-1) {
      cout << " ";
    }
  }
  cout << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}