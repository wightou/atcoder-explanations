#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int n, q;
  cin >> n >> q;

  vector<int> l(n,0);
  for (int i=1; i<n; i++) {
    cin >> l.at(i);
  }
  vector<int> r(n,0);
  for (int i=0; i<n-1; i++) {
    cin >> r.at(i);
  }

  /////////////////// 前処理 ///////////////////

  // そこより右にある「左への移動の移動コスト」の最小値
  vector<int> mn_l(n,0);
  mn_l.at(n-1) = l.at(n-1);
  for (int i=n-2; i>=0; i--) {
    mn_l.at(i) = min(mn_l.at(i+1),l.at(i));
  }

  // そこより左にある「右への移動の移動コスト」の最小値
  vector<int> mn_r(n,0);
  mn_r.at(0) = r.at(0);
  for (int i=1; i<n; i++) {
    mn_r.at(i) = min(mn_r.at(i-1),r.at(i));
  }

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int s, t;
    cin >> s >> t;
    s--;
    t--;

    //////////////// 出力変数定義 ////////////////

    int result = 0;

    //////////////////// 処理 ////////////////////

    if (s<t) {

      // 右へ行くなら「最初から右へ」「左にある最も安い右移動を利用」の2択だけでいい
      result = min(r.at(s),l.at(s)+mn_r.at(s));

    } else {

      // 左へ行くなら「最初から左へ」「右にある最も安い左移動を利用」の2択だけでいい
      result = min(l.at(s),r.at(s)+mn_l.at(s));

    }

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}