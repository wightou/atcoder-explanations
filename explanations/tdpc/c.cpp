#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int k;
  cin >> k;

  // 2のk乗
  int n = (1<<k);

  vector<int> r(n);
  for (int i=0; i<n; i++) {
    cin >> r.at(i);
  }

  //////////////// 出力変数定義 ////////////////



  //////////////////// 処理 ////////////////////

  // DPテーブル（？）
  // firstが山の先頭の人の番号、secondがその山からそれぞれの人が勝ち上がる確率
  deque<pair<int,vector<double>>> dp;
  for (int i=0; i<n; i++) {
    dp.emplace_back(i,vector<double>(1,1.0));
  }

  // 山が1つになるまで、統合していく
  while(ssize(dp)>1) {

    // 先頭の人の番号と、片側の山の人数
    int f = dp.at(0).first;
    int m = ssize(dp.at(0).second);

    // とりあえず、f番から始まる2m人の山用の場所を作る
    dp.emplace_back(f,vector(2*m,0.0));

    // 統合後のそれぞれの人の勝ち上がり確率を求めて足していく
    for (int i=0; i<m; i++) {
      for (int j=0; j<m; j++) {
        double p = 1/(1+pow(10,(r.at(f+m+j)-r.at(f+i))/400.0));
        dp.back().second.at(i) += p*dp.at(0).second.at(i)*dp.at(1).second.at(j);
        dp.back().second.at(m+j) += (1-p)*dp.at(0).second.at(i)*dp.at(1).second.at(j);
      }
    }

    // 処理が終わった山を削除
    dp.pop_front();
    dp.pop_front();

  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<dp.at(0).second.size(); i++) {
    cout << fixed << setprecision(10) << dp.at(0).second.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}