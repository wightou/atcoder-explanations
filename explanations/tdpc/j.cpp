#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr double INF = 1e200;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> x(n);
  for (int i=0; i<n; i++) {
    cin >> x.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  double result = 0;

  //////////////////// 処理 ////////////////////

  // 最大で何個の的を考慮すべきか考える
  // xの最大値+1まで、ただし最低3個
  sort(x.begin(),x.end());
  int m = max(x.back()+1,3);

  // 各位置に的があるかないかをビット表現して bitDP するテーブル
  // 0のときだけ0を入れて、残りは INF にしておく
  vector<double> dp(1<<m,INF);
  dp.at(0) = 0;

  // bitDPを実行
  for (int i=1; i<(1<<m); i++) {

    // 狙う位置ごとに期待値を求める（両端は1つ内側より損なのでスキップ）
    for (int j=1; j<m-1; j++) {

      // 近隣3つのうち、的がある個数
      int count = 0;

      // 的がある位置の、その後の期待値の和
      double sum = 0;

      // 左右1つ先までを見て、それらを求める
      for (int k=-1; k<=1; k++) {

        int pos = j+k;
        if (i&(1<<pos)) {
          sum += dp.at(i-(1<<pos));
          count++;
        }

      }

      // 自己遷移の影響を加味して期待値を求める
      // 的がないところだったらスキップ
      if (count!=0) dp.at(i) = min(dp.at(i),(3+sum)/count);

    }

  }

  int val = 0;
  for (int i : x) {
    val += (1<<i);
  }
  result = dp.at(val);

  //////////////////// 出力 ////////////////////

  cout << fixed << setprecision(10) << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}