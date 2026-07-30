#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 1000000007;

// 幅優先探索（詳細調査）
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
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  // 隣接リストで受け取る
  int m = n-1;
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

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 0を根とする根付き木にしたときの、距離、親、子、トポロジカルソート順を作る
  auto [distances,parents,children,order] = bfs(graph,{0});

  // トポロジカルソート順を逆にしておく
  reverse(order.begin(),order.end());

  // その頂点を白/黒にして、その頂点以下の部分木を塗る方法
  vector<long long> white(n,1);
  vector<long long> black(n,1);

  // トポロジカルソートの逆順にデータを埋めていく
  for (int v : order) {

    // 子を全て見る
    // - 黒の塗り方は、子が全て白である必要がある
    // - 白の塗り方は、子が白でも黒でもよい
    for (int u : children.at(v)) {
      white.at(v) *= white.at(u)+black.at(u);
      white.at(v) %= mod;
      black.at(v) *= white.at(u);
      black.at(v) %= mod;
    }

  }
  
  // 頂点0の部分木は全体を意味するので、その塗り方の数が答え
  result = (white.at(0)+black.at(0))%mod;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}