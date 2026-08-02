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

  // aの中の最大値を取得しておく
  int m = *max_element(a.begin(),a.end());

  // 各iごとに、iの倍数が何番目にあるか管理する二次元配列
  vector<vector<int>> vec(m+1);

  // 各 a[i] ごとに、その約数のところに値を入れていく
  for (int i=0; i<n; i++) {
    int num = a.at(i);
    for (int j=1; j*j<=num; j++) {
      if (num%j) continue;
      vec.at(j).emplace_back(i);
      if (j*j!=num) vec.at(num/j).emplace_back(i);
    }
  }

  // UnionFind木を用いて、大きい値から順に「それらの頂点が全部連結になるようにする」をやる
  dsu d(n);
  for (int i=m; i>0; i--) {
    for (int j=1; j<ssize(vec.at(i)); j++) {
      if (d.same(vec.at(i).at(0),vec.at(i).at(j))) continue;
      d.merge(vec.at(i).at(0),vec.at(i).at(j));
      result += i;
    }
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}