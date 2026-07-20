#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int x, y, l, r, a, b;
  cin >> x >> y >> l >> r >> a >> b;

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 仮に全時間Y円だった場合の料金
  result = y*(b-a);

  // X円の時間帯に何時間駐車したかを求める
  int d = min(b,r)-max(a,l);
  d = max(d,0);

  // 差分を反映
  result += (x-y)*d;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}