#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> h(n);
  for (int i=0; i<n; i++) {
    cin >> h.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // DPテーブル
  // dp.at(i) は、i番目の足場までの最少コスト
  vector<int> dp(n);

  // スタートの足場までのコストは0
  dp.at(0) = 0;

  // スタートの次の足場は、スタートからの移動コスト
  dp.at(1) = abs(h.at(1)-h.at(0));

  // そこから先は、1つ前と2つ前の足場の、コストが低く来れる方
  for (int i=2; i<n; i++) {
    dp.at(i) = min(dp.at(i-1)+abs(h.at(i-1)-h.at(i)),dp.at(i-2)+abs(h.at(i-2)-h.at(i)));
  }
  
  // 最後の足場の値が答え
  result = dp.back();

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}