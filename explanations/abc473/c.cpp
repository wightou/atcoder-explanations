#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, k;
  cin >> n >> k;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
    a.at(i)--;
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 各クラスの人数をカウンティングする
  vector<int> counters(k);
  for (int i : a) {
    counters.at(i)++;
  }

  // 最も人数が多いクラスの人数を取得
  int m = *max_element(counters.begin(),counters.end());

  // 最大人数か、それ-1ならヨシとして数える
  for (int i : counters) {
    if (i>=m-1) result++;
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}