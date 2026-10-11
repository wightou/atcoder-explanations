#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr long long INF = 1001001001001001001LL;

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////



  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int n;
    cin >> n;

    vector<int> x(n), y(n), z(n);
    for (int i=0; i<n; i++) {
      cin >> x.at(i) >> y.at(i) >> z.at(i);
    }

    //////////////// 出力変数定義 ////////////////

    long long result = 0;

    //////////////////// 処理 ////////////////////

    // 配列サイズ用に、xとyの最大値+1を求めておく
    int lim_x = *max_element(x.begin(),x.end())+1;
    int lim_y = *max_element(y.begin(),y.end())+1;

    // 差分としてありえる最小値
    long long mn = 0;

    // その場所にいるためのコストの最小値（と全魔法との差分）
    vector<vector<long long>> dp(lim_x,vector<long long>(lim_y,INF));

    // 最初に、そこからx方向移動だけするのを反映しておく
    for (int r=0; r<lim_x; r++) dp.at(r).at(0) = r*r;

    for (int i=0; i<n; i++) {

      // y方向の移動で目的地に行く最小コストを出す
      for (int c=0; c<lim_y; c++) {
        dp.at(x.at(i)).at(y.at(i)) = min(dp.at(x.at(i)).at(y.at(i)),dp.at(x.at(i)).at(c)+(c-y.at(i))*(c-y.at(i)));
      }

      // 魔法を使うコストが浮いたので、その分を引く
      dp.at(x.at(i)).at(y.at(i)) -= z.at(i);

      // そこで最後の移動とした場合の差分を最小値候補にする
      mn = min(mn,dp.at(x.at(i)).at(y.at(i)));

      // そこからx方向移動だけするのを反映しておく
      for (int r=0; r<lim_x; r++) {
        dp.at(r).at(y.at(i)) = min(dp.at(r).at(y.at(i)),dp.at(x.at(i)).at(y.at(i))+(r-x.at(i))*(r-x.at(i)));
      }

    }
    
    // 全部魔法で倒す場合のコスト+差分最小値が答え
    result = accumulate(z.begin(),z.end(),0LL)+mn;

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}