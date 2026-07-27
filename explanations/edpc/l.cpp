#include <bits/stdc++.h>
using namespace std;

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

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // DPテーブル
  // dp.at(l).at(r)は、l番目からr番目までが初形だった場合の先手の得点-後手の得点
  vector<vector<long long>> dp(n,vector<long long>(n,0));

  // 残り1つの場合は結果が明らか
  for (int l=0; l<n; l++) {
    dp.at(l).at(l) = a.at(l);
  }

  // DP表のl<rの場合を下段から順に左から埋める
  for (int l=n-1; l>=0; l--) {
    for (int r=l+1; r<n; r++) {

      // l側を取った場合の自分の得点予測
      long long tmp1 = a.at(l)-dp.at(l+1).at(r);

      // r側を取った場合の自分の得点予測
      long long tmp2 = a.at(r)-dp.at(l).at(r-1);

      // 大きい方がその場合の得点予測
      dp.at(l).at(r) = max(tmp1,tmp2);

    }
  }

  // 全部ある場合の得点予測が答え
  result = dp.at(0).at(n-1);

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}