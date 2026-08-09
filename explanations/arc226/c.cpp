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

    int h, w;
    cin >> h >> w;

    //////////////// 出力変数定義 ////////////////

    long long result = 0;
    vector<tuple<int,int,int>> vec;

    //////////////////// 処理 ////////////////////

    if (h%2==0||w%2==0) {

      // もし片方が偶数なら、左上詰めで 2x2 の正方形を並べればよい
      for (int i=1; i<h; i+=2) {
        for (int j=1; j<w; j+=2) {
          vec.emplace_back(i,j,1);
        }
      }

    } else {

      // 以下は、両方奇数の場合

      // はみ出さないように取れる、辺の長さが 4N+1 型の数である正方形の辺の長さ mn を求める
      int mn = min(h,w);
      if (mn%4==3) mn -= 2;

      // 以下、左上 mn x mn の正方形を、mn(mn-1)/4 セットで埋める

      // 左上の 1/4
      // 2x2で左上から埋めるが、対角線にかかるやつ以右を1つ右にずらす
      for (int i=1; i<mn/2; i+=2) {
        for (int j=1; j<mn/2; j+=2) {
          vec.emplace_back(i,j+(i<=j),1);
        }
      }

      // 右上の 1/4
      // 2x2で右上から埋めるが、対角線にかかるやつ以下を1つ下にずらす
      for (int i=1; i<mn/2; i+=2) {
        for (int j=mn/2+2; j<mn; j+=2) {
          vec.emplace_back(i+(i+j>=mn),j,1);
        }
      }

      // 左下の 1/4
      // 2x2で左下から埋めるが、対角線にかかるやつ以上を1つ上にずらす
      for (int i=mn/2+2; i<mn; i+=2) {
        for (int j=1; j<mn/2; j+=2) {
          vec.emplace_back(i-(i+j<=mn),j,1);
        }
      }

      // 右下の 1/4
      // 2x2で右下から埋めるが、対角線にかかるやつ以左を1つ左にずらす
      for (int i=mn/2+2; i<mn; i+=2) {
        for (int j=mn/2+2; j<mn; j+=2) {
          vec.emplace_back(i,j-(i>=j),1);
        }
      }

      // 対角線上、中心から偶数マス目にあたる4点の正方形を作る
      for (int i=1; i<mn/2; i+=2) {
        vec.emplace_back(i,i,mn+1-i*2);
      }

      // 左上 mn x mn の正方形以外の部分は、右下寄せで 2x2 で埋める
      for (int i=2; i<h; i+=2) {
        for (int j=2; j<w; j+=2) {
          if (i>=mn||j>=mn) vec.emplace_back(i,j,1);
        }
      }

    }

    // 実際に作ったデータのサイズが答え
    result = ssize(vec);

    //////////////////// 出力 ////////////////////

    cout << result << endl;
    for (auto [r, c, s] : vec) {
      cout << r << " " << c << " " << s << endl;
    }

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}