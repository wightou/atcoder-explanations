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

  // -1で初期化しておく
  int result = -1;

  //////////////////// 処理 ////////////////////

  // ソートすることで、9があれば末尾に移動し、その場合の次の球は先頭に移動する
  sort(a.begin(),a.end());

  // 末尾が9なら先頭が答え、違ったら-1のまま
  if (a.back()==9) result = a.front();

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}