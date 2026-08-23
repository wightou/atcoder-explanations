#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// BFSによる根付き木化
// ライブラリそのままのためいろいろ求めているが、今回必要なのは距離情報と親情報だけ
tuple<vector<int>,vector<int>,vector<vector<int>>,vector<int>> bfs(const vector<vector<int>>& graph, const vector<int>& starts) {
  int n = ssize(graph);
  vector<int> distances(n,-1);
  vector<int> parents(n,-1);
  vector<vector<int>> children(n);
  vector<int> order;
  deque<int> que;
  for (int v : starts) {
    assert(v>=0);
    assert(v<n);
    assert(distances[v]==-1);
    distances[v] = 0;
    que.emplace_back(v);
    order.emplace_back(v);
  }
  while (!que.empty()) {
    for (int v : graph[que.front()]) {
      if (distances[v]==-1) {
        distances[v] = distances[que.front()]+1;
        que.emplace_back(v);
        order.emplace_back(v);
        parents[v] = que.front();
        children[que.front()].emplace_back(v);
      }
    }
    que.pop_front();
  }
  return {distances,parents,children,order};
}

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////



  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int n, m;
    cin >> n >> m;

    // 隣接リストで受け取る
    vector<int> u(m), v(m);
    vector<vector<int>> graph(n);
    for (int i=0; i<m; i++) {
      cin >> u.at(i) >> v.at(i);
      u.at(i)--;
      v.at(i)--;
      graph.at(u.at(i)).emplace_back(v.at(i));
      graph.at(v.at(i)).emplace_back(u.at(i));
    }

    //////////////// 出力変数定義 ////////////////

    deque<int> result;

    //////////////////// 処理 ////////////////////

    // BFSで、0を根とする根付き木としての距離と親（と不要なもの2つ）を受け取る
    auto [distances,parents,children,order] = bfs(graph,{0});

    // 辺全てを調査するループ
    for (int i=0; i<m; i++) {

      // 奇数長閉路があるなら、同じ距離同士を連結する辺があるので、それを検出
      if (distances.at(u.at(i))==distances.at(v.at(i))) {

        // 最近共通祖先を求める
        // 同じ距離にいるので、同時にシンプルに1つずつ遡ればよい
        // u側はresultの前から、v側はresultの後ろから入れていく
        int x = u.at(i);
        int y = v.at(i);
        while (x!=y) {
          result.emplace_front(x+1);
          result.emplace_back(y+1);
          x = parents.at(x);
          y = parents.at(y);
        }

        // 最近共通祖先を先頭か末尾かどちらかに入れれば完成
        result.emplace_front(x+1);

        // 1つ見つけたので脱出（ここを通ったかどうかは、resultが空かどうかで判別できる）
        break;

      }

    }

    //////////////////// 出力 ////////////////////

    if (result.empty()) {
      cout << -1 << endl;
    } else {
      cout << result.size() << endl;
      for (size_t i=0; i<result.size(); i++) {
        cout << result.at(i);
        if (i!=result.size()-1) {
          cout << " ";
        }
      }
      cout << endl;
    }

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}