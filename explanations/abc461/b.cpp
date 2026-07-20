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
    a.at(i)--;
  }
  vector<int> b(n);
  for (int i=0; i<n; i++) {
    cin >> b.at(i);
    b.at(i)--;
  }

  //////////////// 出力変数定義 ////////////////

  string result = "Yes";

  //////////////////// 処理 ////////////////////

  // すべての斧を順に見るループ
  for (int i=0; i<n; i++) {
    
    // 斧iの持ち主が主張している斧番号がiかどうかチェック
    // 1人でも違う人がいたなら"No"
    if (a.at(b.at(i))!=i) result = "No";

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}