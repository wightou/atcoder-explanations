#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 1000000007;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  string s;
  cin >> n >> s;

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // DPテーブル
  // dp[i]は、条件を満たす順列のうち、末尾がiであるもの
  // 最初は1だけなので、末尾1のものが1つあるだけ
  deque<int> dp(1,1);

  // 順列の個数を1ずつ増やしていく
  for (int i=0; i<n-1; i++) {

    // '<'か'>'かで処理を分ける
    if (s.at(i)=='<') {

      // '<'の場合は、先頭に0をつけ足してから、前から累積和を取る
      dp.emplace_front(0);
      for (int j=1; j<=i+1; j++) {
        dp.at(j) += dp.at(j-1);
        dp.at(j) %= mod;
      }

    } else {

      // '>'の場合は、末尾に0をつけ足してから、後ろから累積和を取る
      dp.emplace_back(0);
      for (int j=i; j>=0; j--) {
        dp.at(j) += dp.at(j+1);
        dp.at(j) %= mod;
      }

    }

  }

  // Nまで拡張したときの全体の和が答え
  result = accumulate(dp.begin(),dp.end(),0LL)%mod;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}