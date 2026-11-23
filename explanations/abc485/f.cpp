#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// ライブラリ https://github.com/wightou/library-public

constexpr long long INF = 1001001001001001001LL;

/////////////////// メイン ///////////////////

int main () {

  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;

  vector<int> c(n), v(n);
  for (int i=0; i<n; i++) {
    cin >> c.at(i) >> v.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  vector<long long> result(m+1,0);

  //////////////////// 処理 ////////////////////

  // 商品を高い順に取り出すpriority_queue、番兵も入れておく
  priority_queue<pair<int,int>> que;
  for (int i=0; i<n; i++) {
    que.emplace(c.at(i),v.at(i));
  }
  que.emplace(-1,-1);

  // 無料でもらえる商品の価値合計として全体底上げする分
  long long boost = accumulate(v.begin(),v.end(),0LL);

  // DPテーブル
  // dp[i][k]は、j円以上の商品を割引価格でi個買ってk円以下にしたときの最大価値
  vector<deque<long long>> dp(n+1,deque<long long>(m+1,-INF));
  dp.at(0) = deque<long long>(m+1,0);

  // j円以上の商品の個数
  int counter = 0;

  // 割引額が大きい方から調査
  for (int j=m; j>=0; j--) {

    // 割引額が1円減ったので、買う個数×1円だけ値上がりする
    for (int i=0; i<=counter; i++) {
      for (int k=0; k<i; k++) {
        dp.at(i).emplace_front(-INF);
        dp.at(i).pop_back();
      }
    }

    // ちょうどj円の商品があったら、割引価格0円として商品リストに追加
    while (que.top().first==j) {

      // 取り出す
      int v0 = que.top().second;
      que.pop();
      boost -= v0;
      counter++;

      // 0円の商品をもらう方がお得だったら更新
      for (int i=counter; i>0; i--) {
        for (int k=0; k<=m; k++) {
          dp.at(i).at(k) = max(dp.at(i).at(k),dp.at(i-1).at(k)+v0);
        }
      }

    }

    // 各個数のM円以下での値の最大値+無料商品分底上げ が答え
    for (int i=0; i<=counter; i++) {
      result.at(j) = max(result.at(j),dp.at(i).at(m));
    }
    result.at(j) += boost;

  } 

  //////////////////// 出力 ////////////////////

  for (int i=0; i<ssize(result); i++) {
    cout << result.at(i) << '\n';
  }

  //////////////////// 終了 ////////////////////

  return 0;

}