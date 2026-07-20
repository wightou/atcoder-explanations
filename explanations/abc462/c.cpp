#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> x(n), y(n);
  for (int i=0; i<n; i++) {
    cin >> x.at(i) >> y.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // xが小さい順にyを並べる
  vector<int> vec(n,-1);
  for (int i=0; i<n; i++) {
    vec.at(x.at(i)-1) = y.at(i);
  }

  // 最小値記録用
  int mn = n+1;

  // 「自分より前に自分より小さい数がない」やつが条件を満たすので、それを数える
  for (int i=0; i<n; i++) {
    if (mn>vec.at(i)) {
      mn = vec.at(i);
      result++;
    }
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}