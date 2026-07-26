#include <bits/stdc++.h>
using namespace std;

constexpr long long INF = 1001001001001001001LL;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> w(n), s(n), v(n);
  for (int i=0; i<n; i++) {
    cin >> w.at(i) >> s.at(i) >> v.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // w+sが小さい順にソートしておく
  vector<tuple<int,int,int,int>> vec(n);
  for (int i=0; i<n; i++) {
    vec.at(i) = {w.at(i)+s.at(i),w.at(i),s.at(i),v.at(i)};
  }
  sort(vec.begin(),vec.end());

  // DPテーブル
  // dp.at(j)は、総重量jでの最大価値で、j<=(w+sの最大値) しか更新されることはない
  // 重量 0 の場合を 0 で初期化しておく
  vector<long long> dp(get<0>(vec.back())+1,-INF);
  dp.at(0) = 0;

  // ブロックを1つずつ見ていく
  for (int i=0; i<n; i++) {

    // ブロックの情報を取り出す
    auto [ws,w,s,v] = vec.at(i);

    // ナップサック問題としてテーブル更新
    for (int j=s; j>=0; j--) {
      dp.at(j+w) = max(dp.at(j+w),dp.at(j)+v);
    }

  }

  // テーブル内の最大値が答え
  result = *max_element(dp.begin(),dp.end());

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}