#include <bits/stdc++.h>
using namespace std;

constexpr long long INF = 1001001001001001001LL;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<long long> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // とりあえず累積和を作る
  vector<long long> sum(n+1,0);
  partial_sum(a.begin(),a.end(),sum.begin()+1);

  // DPテーブル
  // dp.at(l).at(r)は、l番目からr番目までのスライムを合体させる場合の最小コスト
  vector<vector<long long>> dp(n,vector<long long>(n,INF));
  
  // 範囲内にスライムが1匹しかいない場合は、コスト0
  for (int l=0; l<n; l++) {
    dp.at(l).at(l) = 0;
  }

  // DP表のl<rの場合を下段から順に左から埋める
  for (int l=n-1; l>=0; l--) {
    for (int r=l+1; r<n; r++) {

      // 最後に合体する境界を全パターン試し、コストの和の最小値を探す
      for (int m=l+1; m<=r; m++) {
        dp.at(l).at(r) = min(dp.at(l).at(r),dp.at(l).at(m-1)+dp.at(m).at(r));
      }

      // 今回の合体のコストを加算
      dp.at(l).at(r) += sum.at(r+1) - sum.at(l);

    }
  }

  // 全スライムの結果は右上
  result = dp.at(0).at(n-1);

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}