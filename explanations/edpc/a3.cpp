#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int q;
  cin >> q;

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  /////////////////// 前処理 ///////////////////

  // 最初の2つの足場の情報だけ先に受け取る
  int last_h, now_h;
  cin >> last_h >> now_h;
  int last_cost = 0;
  int now_cost = abs(now_h-last_h);

  /////////////////// ループ ///////////////////

  // 3つめ以降の処理
  for (int loop=2; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    // 次の足場の高さを受け取る
    int next_h;
    cin >> next_h;

    //////////////////// 処理 ////////////////////

    // 次の足場へのコストを求める
    int next_cost = min(last_cost+abs(last_h-next_h),now_cost+abs(now_h-next_h));

    // 次の足場へ進み、情報をずらす
    last_h = now_h;
    last_cost = now_cost;
    now_h = next_h;
    now_cost = next_cost;    

  }

  /////////////////// 後処理 ///////////////////

  // 最後の足場にたどり着いたときの「今の足場のコスト」が答え
  result = now_cost;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}