#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  // 変数がかぶっているので、aの長さにn、bの長さにmを用いる
  int n, m;
  cin >> n >> m;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }
  vector<int> b(m);
  for (int j=0; j<m; j++) {
    cin >> b.at(j);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 逆順（後ろから取る）の方が扱いやすいので、reverseする
  reverse(a.begin(),a.end());
  reverse(b.begin(),b.end());

  // DPテーブル
  // dp[i]は、指定のaまでで、bが前からi個までの場合の「先手スコア-後手スコア」
  // aが空であった場合のデータで埋めておく
  vector<int> dp(m+1,0);
  for (int j=0; j<m; j++) {
    dp.at(j+1) = b.at(j)-dp.at(j);
  }

  // aを1つずつ増やしながらDPテーブル更新
  for (int i=0; i<n; i++) {

    // bが空の場合
    dp.at(0) = a.at(i)-dp.at(0);

    // bが空でない場合
    // 連続でbから取ることもできるので、左から更新
    for (int j=0; j<m; j++) {
      dp.at(j+1) = max(a.at(i)-dp.at(j+1),b.at(j)-dp.at(j));
    }

  }

  // dp.back()が「すぬけ君の点数-すめけくんの点数」
  // これに全部の得点を足すとすぬけ君の点数の2倍になるので、その半分が答え
  result = (dp.back()+accumulate(a.begin(),a.end(),0)+accumulate(b.begin(),b.end(),0))/2;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}