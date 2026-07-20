#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  char x;
  cin >> n >> x;

  vector<string> s(n);
  for (int i=0; i<n; i++) {
    cin >> s.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  string result = "No";

  //////////////////// 処理 ////////////////////

  //　全ての列車を順に見ていく
  for (const string& str : s) {

    // x-'A' で何番目の座席化を求め、それに該当するいちに 'o' があれば "Yes"
    if (str.at(x-'A')=='o') result = "Yes";

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}