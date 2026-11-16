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

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 各数が何回登場したかを数えるカウンター
  vector<int> counters(n+1,0);

  // 数列を前から順に見る
  for (int i=0; i<n; i++) {

    // i番目がi-1と一致でないならば、これまでに出た異なる数の個数を結果に足す
    if (i>0&&a.at(i-1)!=a.at(i)) result += i-counters.at(a.at(i));

    // 出た回数に計上
    counters.at(a.at(i))++;

  }

  result += 1;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}