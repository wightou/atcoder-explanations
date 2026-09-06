#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> p(n);
  for (int i=0; i<n; i++) {
    cin >> p.at(i);
    p.at(i)--;
  }

  //////////////// 出力変数定義 ////////////////

  string result = "Yes";

  //////////////////// 処理 ////////////////////

  // 0-indexedで、座席番号と順番を両方10で割った商が一致していれば問題ない
  // 不一致が1人でもいればNo
  for (int i=0; i<n; i++) {
    if (p.at(i)/10!=i/10) result = "No";
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}