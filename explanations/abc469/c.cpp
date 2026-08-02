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

  vector<int> result;

  //////////////////// 処理 ////////////////////

  // 前から何番目の 'x' が全体の何番目にあるのか調べる
  // （0-indexedと1-indexedが混ざっているので注意）
  for (int i=0; i<n; i++) {
    if (s.at(i)=='x') result.emplace_back(i+1);
  }

  // 'x' の数が足りないものについては、最後まで
  result.resize(n,n);

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i);
    if (i!=result.size()-1) {
      cout << " ";
    }
  }
  cout << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}