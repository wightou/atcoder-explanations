#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> a(n), b(n);
  vector<string> s(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i) >> b.at(i) >> s.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 情報を1つずつ見ていく
  for (int i=0; i<n; i++) {

    // keepのときの、本当は受け取るはずだったおつり分を加算していく
    if (s.at(i)=="keep") result += b.at(i)-a.at(i);

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}