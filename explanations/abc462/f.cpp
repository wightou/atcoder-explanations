#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int INF = 1001001001;

/////////////////// メイン ///////////////////

int main () {
  
  /////////////////// 前入力 ///////////////////
  
  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////



  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {
    
    //////////////////// 入力 ////////////////////

    string s;
    int k;
    cin >> s >> k;

    //////////////// 出力変数定義 ////////////////

    int result = 0;

    //////////////////// 処理 ////////////////////

    // ABCのAの位置になる可能性は、sの長さ-2
    int n = ssize(s)-2;
    if (n<0) n = 0;

    // 各位置、そこをAとするABCを作るのに何回操作が必要か
    vector<int> vec(n,3);
    for (int i=0; i<n; i++) {
      if (s.at(i)=='A') vec.at(i)--;
      if (s.at(i+1)=='B') vec.at(i)--;
      if (s.at(i+2)=='C') vec.at(i)--;
    }

    // 動的計画法をする
    // dp.at(i)は、「ここまでi/3回追加で作っていて、直前のi%3文字ではABCを作っていない」の最小コスト
    // 既存ABCを破壊して新しくABCを作るケースがあるので、k+1回作れる分までデータ領域を確保しておく
    deque<int> dp(3*k+6,INF);
    dp.at(0) = 0;
    dp.at(1) = 0;
    dp.at(2) = 0;

    // vecの各要素を1文字ずつチェック
    for (int i=0; i<n; i++) {

      // 3の倍数とそれ以外にわけて、dpデータをインライン更新
      // 直前の2文字にABCを作っていないところだけ、新たにABCを作れる
      for (int j=3*k+5; j>0; j--) {
        if (j%3==0) {
          dp.at(j) = min(dp.at(j),dp.at(j-1)+vec.at(i));
        } else {
          dp.at(j) = min(dp.at(j),dp.at(j-1));
        }
      }

      // もし元々ABCだった場合、追加で作った回数が1回減る
      if (vec.at(i)==0) {
        for (int j=0; j<3; j++) {
          dp.pop_front();
          dp.emplace_back(INF);
        }
      }

    }

    // at(3*k)が答え、INFなら無理なので-1にする
    result = dp.at(3*k);
    if (result==INF) result = -1;

    //////////////////// 出力 ////////////////////

    cout << result << endl;
    
  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}