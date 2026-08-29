#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 残っているカード一覧
  set<int> st;

  // 1枚ずつカードを見ていく
  for (int i : a) {

    // あるカードだったら、その2枚を食べる
    if (st.contains(i)) st.erase(i);

    // ないカードだったら、カード一覧に加える
    else st.emplace(i);

  }

  // カード一覧に残ったものの合計が答え
  for (int i : st) {
    result += i;
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}