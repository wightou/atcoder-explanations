#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr long long INF = 1001001001001001001LL;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  // Wの文字がかぶっているので、目標値はmとする
  int n, m;
  cin >> n >> m;
  
  vector<int> w(n), v(n);
  for (int i=0; i<n; i++) {
    cin >> w.at(i) >> v.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // とりあえず、価値合計を求めておく
  int sum = accumulate(v.begin(),v.end(),0);

  // DPテーブル
  // dp.at(j)は、総価値jでの最小総重量
  // 0個のときだけ0、それ以外はINFで初期化しておく
  vector<long long> dp(sum+1,INF);
  dp.at(0) = 0;

  // 0/1ナップサック問題なので、後ろから更新する
  for (int i=0; i<n; i++) {
    for (int j=sum; j>=v.at(i); j--) {
      dp.at(j) = min(dp.at(j),dp.at(j-v.at(i))+w.at(i));
    }
  }

  // 最小総重量がm以下である価値のうち、最も高いものが答え
  for (int i=sum; i>=0; i--) {
    if (dp.at(i)<=m) {
      result = i;
      break;
    }
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}