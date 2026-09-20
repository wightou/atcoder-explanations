#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  string s, t;
  cin >> n >> s >> t;

  //////////////// 出力変数定義 ////////////////

  string result = "Yes";

  //////////////////// 処理 ////////////////////

  // 同じ位置の文字の組を全探索する
  // bの方が'*'であるか、aとbで同じならよい
  // そうでないものが1つでもあったら、答えは"No"
  for (int i=0; i<n; i++) {
    if (t.at(i)!='*'&&t.at(i)!=s.at(i)) result = "No";
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}