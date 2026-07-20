#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;
  
  vector<vector<int>> x(n,vector<int>(n));
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      cin >> x.at(i).at(j);
    }
  }

  //////////////// 出力変数定義 ////////////////

  vector<vector<int>> result(n,vector<int>(n));

  //////////////////// 処理 ////////////////////

  // 全部、2倍して5で割った余りに変換する
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      result.at(i).at(j) = (x.at(i).at(j)*2)%5;
    }
  }

  //////////////////// 出力 ////////////////////

  for (size_t j=0; j<result.size(); j++) {
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