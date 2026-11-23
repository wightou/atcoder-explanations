#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// ライブラリ https://github.com/wightou/library-public

/////////////////// メイン ///////////////////

int main () {

  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;

  vector<int> u(m), v(m);
  vector<int> c(m);
  vector<vector<pair<int,int>>> graph(n);
  for (int i=0; i<m; i++) {
    cin >> u.at(i) >> v.at(i) >> c.at(i);
    u.at(i)--;
    v.at(i)--;
    graph.at(u.at(i)).emplace_back(c.at(i),v.at(i));
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 来た道1色目の距離とその色
  vector<int> distances1(n,-1);
  vector<int> colors(n,-1);

  // 来た道2色目
  vector<int> distances2(n,-1);

  // 幅優先探索用のqueue（のつもりのdeque）に、存在しない色指定でスタート地点を入れておく
  deque<tuple<int,int,int>> que;
  que.emplace_back(0,-2,0);

  // 幅優先探索する
  while (!que.empty()) {

    // とりあえず取り出す
    auto [v0,c0,d0] = que.front();
    que.pop_front();

    // 既に2色で来ている場合と、1色目と重複の場合はスキップ
    if (distances2.at(v0)!=-1) continue;
    if (colors.at(v0)==c0) continue;

    // 1色目データがなければそれを、あれば2色目の方を距離更新
    if (distances1.at(v0)==-1) {
      distances1.at(v0) = d0;
      colors.at(v0) = c0;
    } else distances2.at(v0) = d0;

    // 次の頂点を入れる
    for (auto [c1,v1] : graph.at(v0)) {
      if (c0!=c1) que.emplace_back(v1,c1,d0+1);
    }

  }

  // 頂点Nの1色目の到達距離が答え
  result = distances1.back();

  //////////////////// 出力 ////////////////////

  cout << result << '\n';

  //////////////////// 終了 ////////////////////

  return 0;

}