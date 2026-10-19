#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // とりあえず、1人に配る量を求めておく
  long long d = accumulate(a.begin(),a.end(),0LL)/m;

  // bitDPで、「適切な順列に並べることで、累積和がdの倍数になるタイミングを最大何個作れるか」を求める
  // secondに、今のところの累積和を入れておく
  vector<pair<int,long long>> dp(1<<n,{0,0});

  // bitDPを実行
  for (int i=1; i<(1<<n); i++) {

    // そのデータに含まれるケーキについて、全探索
    for (int j=0; j<n; j++) {
      if ((i&(1<<j))==0) continue;

      // とりあえず、そこまでの達成回数最大値を求める
      // secondは毎回同じになるはずだけど一応計算している
      pair<int,long long> p = dp.at(i^(1<<j));
      p.second += a.at(j);
      dp.at(i) = max(dp.at(i),p);

    }

    // 累積和がdの倍数になっていたら、達成回数を+1
    if (dp.at(i).second%d==0) dp.at(i).first++;

  }

  // 全ケーキでの順列での最大値が、M回から節約できる回数
  result = m-dp.back().first;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}