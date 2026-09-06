#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

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

    vector<int> a(n);
    for (int i=0; i<n; i++) {
      cin >> a.at(i);
    }

    //////////////// 出力変数定義 ////////////////

    long long result = 0;

    //////////////////// 処理 ////////////////////

    // aのうち、大きい方2つは1回ずつ、それ以外は2回ずつ足しておく
    sort(a.rbegin(),a.rend());
    result = accumulate(a.begin(),a.end(),0LL)*2;
    result -= a.at(0)+a.at(1);

    // ここから、偶奇境界の最大数を引きたい

    // とりあえず、偶奇それぞれ個数を数える
    int count_o = 0;
    int count_e = 0;
    for (int i : a) {
      if (i%2) count_o++;
      else count_e++;
    }

    // 両端が偶数なら、奇数の個数x2か、(偶数の個数-1)x2の小さい方
    // 片方が偶数なら、奇数の個数x2-1か、偶数の個数x2-1の小さい方
    // 両端が奇数なら、偶数の個数x2か、(奇数の個数-1)x2の小さい方
    if (a.at(0)%2+a.at(1)%2==0) {
      result -= 2*min(count_o,count_e-1);
    } else if (a.at(0)%2+a.at(1)%2==1) {
      result -= 2*min(count_o,count_e)-1;
    } else if (a.at(0)%2+a.at(1)%2==2) {
      result -= 2*min(count_e,count_o-1);
    }

    // 引き算の結果が答えの2倍になる、つまり結果を半分にすれば答え
    result /= 2;

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}