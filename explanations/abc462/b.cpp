#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<vector<int>> a(n);
  for (int i=0; i<n; i++) {
    int k;
    cin >> k;
    a.at(i).resize(k);
    for (int j=0; j<k; j++) {
      cin >> a.at(i).at(j);
    }
  }

  //////////////// 出力変数定義 ////////////////

  vector<vector<int>> result(n);

  //////////////////// 処理 ////////////////////

  // aのi番目にjさんがいたら、答えのj番目にiさんがいる
  // 1-indexと0-indexが入り混じっているので気を付ける
  for (int i=0; i<n; i++) {
    for (int p : a.at(i)) {
      result.at(p-1).emplace_back(i+1);
    }
  }

  //////////////////// 出力 ////////////////////

  for (size_t j=0; j<result.size(); j++) {
    cout << result.at(j).size() << " ";
    for (size_t i=0; i<result.at(j).size(); i++) {
      cout << result.at(j).at(i);
      if (i!=result.at(j).size()-1) {
        cout << " ";
      }
    }
    cout << endl; 
  }

  //////////////////// 終了 ////////////////////

  return 0;

}