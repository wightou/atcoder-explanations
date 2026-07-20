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
    string s;
    cin >> n >> s;

    vector<int> x(n);
    for (int i=0; i<n; i++) {
      cin >> x.at(i);
    }

    vector<int> y(n-1);
    for (int i=0; i<n-1; i++) {
      cin >> y.at(i);
    }

    //////////////// 出力変数定義 ////////////////

    long long result = 0;

    //////////////////// 処理 ////////////////////

    // DPテーブル
    // dp_s.at(i)は、i日目（0-indexed）が晴の場合、その日までの最大嬉しさ
    // dp_r.at(i)は、同じく雨の場合
    vector<long long> dp_s(n,0);
    vector<long long> dp_r(n,0);

    // 1日ずつ見る
    for (int i=0; i<n; i++) {

      // 1日目以降だったら、前の日の記録を見て嬉しさが大きい方を採用する
      if (i>0) {
        dp_s.at(i) += max(dp_s.at(i-1),dp_r.at(i-1)+y.at(i-1));
        dp_r.at(i) += max(dp_s.at(i-1),dp_r.at(i-1));
      }

      // 天気を変える側には、その分下げておく
      if (s.at(i)=='R') dp_s.at(i) -= x.at(i);
      else dp_r.at(i) -= x.at(i);

    }

    // 最後の日がどちらの天気でもいいので、両者の最終日の最大嬉しさの大きい方が答え
    result = max(dp_s.at(n-1),dp_r.at(n-1));

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}