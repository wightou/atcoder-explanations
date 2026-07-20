#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int INF = 1001001001;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, k;
  cin >> n >> k;

  vector<int> h(n);
  for (int i=0; i<n; i++) {
    cin >> h.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // DPテーブル
  // dp.at(i) は、i番目の足場までの最少コスト
  vector<int> dp(n,INF);

  // スタートの足場までのコストは0
  dp.at(0) = 0;
  
  // そこから先は、1つ前からk個前までの足場の、コストが低く来れるもの
  // INFで初期化してあるので、最小値更新だけでいい
  for (int i=1; i<n; i++) {
    for (int j=max(0,i-k); j<i; j++) {
      dp.at(i) = min(dp.at(i),dp.at(j)+abs(h.at(j)-h.at(i)));
    }
  }
  
  // 最後の足場の値が答え
  result = dp.back();

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}