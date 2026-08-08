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

  // どの色が何回出ているかをカウント
  // counters[0]はダミー
  vector<int> counters(n+1);
  for (int i : a) {
    counters.at(i)++;
  }

  // 最大個数の色ではないボールの個数が答え
  result = n-(*max_element(counters.begin(),counters.end()));

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}