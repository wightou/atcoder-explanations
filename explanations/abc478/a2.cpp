#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  // 整数を2つ用意し、入力を受け取る
  int n, m;
  cin >> n >> m;

  //////////////// 出力変数定義 ////////////////

  vector<int> result(n);

  //////////////////// 処理 ////////////////////

  // m/nを、適切に切り上げたり切り捨てたりする（数学的にやや高度）
  for (int i=0; i<n; i++) {
    result.at(i) = (m+n-i-1)/n;
  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}