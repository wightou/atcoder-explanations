#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr long long INF = 1001001001001001001LL;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  // long long型にしておく
  vector<long long> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 番兵を入れる
  a.emplace_back(-INF);
  a.emplace_back(INF);

  // aをソートする
  sort(a.begin(),a.end());

  // 右側用インデックス
  int r = 0;
  while (r<=n&&a.at(r)<0) r++;

  // 左側用インデックス
  int l = r-1;

  // 現在地
  int pos = 0;

  // クッキーが残っている限り続けるループ
  while (0<l||r<=n) {

    // 「左がない」「左右両方あるが、左の方が遠い」いずれかに該当するか判定
    if (pos-a.at(l)>a.at(r)-pos) {

      // 該当するなら右を1つ回収
      result += a.at(r)-pos;
      pos = a.at(r);
      r++;

    } else {

      // 該当しないなら左を1つ回収
      result += pos-a.at(l);
      pos = a.at(l);
      l--;

    }

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}