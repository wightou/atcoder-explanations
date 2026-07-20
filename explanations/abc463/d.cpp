#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, k;
  cin >> n >> k;

  vector<int> l(n), r(n);
  for (int i=0; i<n; i++) {
    cin >> l.at(i) >> r.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 布範囲を「右端、左端」でソートして、右端が小さい順に取り出せるようにしておく
  vector<pair<int,int>> vec(n);
  for (int i=0; i<n; i++) {
    vec.at(i) = {r.at(i),l.at(i)};
  }
  sort(vec.begin(),vec.end());

  // ある値がスコアとして達成可能か？　を二分探索するループ
  long long inside = 0;
  long long outside = vec.back().first-vec.front().second;
  while (abs(inside-outside)>1) {
    long long mid = inside+(outside-inside)/2;

    // 右端の位置が最も左である布は確定で採用する
    long long border = vec.front().first+mid;
    int counter = 1;

    // 前の布の右端からmid以上空く限りで、右端が小さい順に貪欲に採用する
    for (auto [right,left] : vec) {
      if (left>=border) {
        border = right+mid;
        counter++;
      }
    }

    // k枚以上の布を採用できていたなら、そのスコアは達成可能
    if (counter>=k) {
      inside = mid;
    } else {
      outside = mid;
    }

  }
  result = inside;

  // 0だった場合は不可能なので、-1に書き換えておく
  if (result==0) result = -1;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}