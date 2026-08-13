#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> p(n);
  for (int i=0; i<n; i++) {
    cin >> p.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 全部取った場合の和を求めておく
  int sum = accumulate(p.begin(),p.end(),0);

  // DPテーブル
  // dp.at(j)は、総和jにできるか
  // 無の状態では和が0だけ達成できる
  vector<bool> dp(sum+1,false);
  dp.at(0) = true;

  // 1つずつ使える数を増やしていく
  // 0/1ナップサック問題なので、後ろから更新する
  for (int i=0; i<n; i++) {
    for (int j=sum; j>=p.at(i); j--) {
      dp.at(j) = dp.at(j)|dp.at(j-p.at(i));
    }
  }

  // DPテーブル内のtrueの個数が答え
  result = count(dp.begin(),dp.end(),true);

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}