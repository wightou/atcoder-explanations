#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int INF = 1001001001;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int h, w;
  cin >> h >> w;

  vector<string> s(h);
  for (int i=0; i<h; i++) {
    cin >> s.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 各マス分+SとTの2頂点の最大流グラフ
  mf_graph<int> graph(h*w+2);

  // 全マスをチェックするループ
  for (int i=0; i<h; i++) {
    for (int j=0; j<w; j++) {

      // 壁だったらスキップ
      if (s.at(i).at(j)=='#') continue;

      // 下と左右が壁でなかったら、選択を連動させる辺を張る
      if (i<h-1&&s.at(i+1).at(j)!='#') graph.add_edge(i*w+j,(i+1)*w+j,INF);
      if (j>0&&s.at(i).at(j-1)!='#') graph.add_edge(i*w+j,i*w+j-1,INF);
      if (j<w-1&&s.at(i).at(j+1)!='#') graph.add_edge(i*w+j,i*w+j+1,INF);

      // `+`なら`#`にすると1損するので、Tに流量1の辺を張る
      // `-`なら`#`にすると1得するので、Sから流量1の辺を張る
      // ついでに、`+`の個数を数えておく
      if (s.at(i).at(j)=='+') {
        graph.add_edge(i*w+j,h*w+1,1);
        result++;
      }
      else graph.add_edge(h*w,i*w+j,1);

    }
  }

  // 制約無視で選んだ最大得点（つまり`+`の個数）から、不正で得た得点（最大フローの値）を引けば答え
  result -= graph.flow(h*w,h*w+1);

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}