#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

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

  int n, mod;
  cin >> n >> mod;
  
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

  vector<long long> result(n);

  //////////////////// 処理 ////////////////////

  // 0を根とする根付き木にしたときの、距離、親、子、トポロジカルソート順を作る
  auto [distances,parents,children,order] = bfs(graph,{0});

  // その頂点を黒部分の根とする塗り方の数
  vector<long long> black_root(n,0);

  // その頂点を黒部分の葉とする塗り方の数
  vector<long long> black_leaf(n,1);

  // トポロジカルソートの逆順にループ
  for (int id=n-1; id>=0; id--) {

    // 調査する頂点番号
    int i = order.at(id);

    // 子の個数
    int s = ssize(children.at(i));

    // black_leafに左から順に累積積をつくる
    long long prod = 1;
    for (int j=0; j<s; j++) {
      black_leaf.at(children.at(i).at(j)) *= prod;
      black_leaf.at(children.at(i).at(j)) %= mod;
      prod *= black_root.at(children.at(i).at(j))+1;
      prod %= mod;
    }

    // black_leafに右から順に累積積をかける
    // （自身以外全ての積になる）
    prod = 1;
    for (int j=s-1; j>=0; j--) {
      black_leaf.at(children.at(i).at(j)) *= prod;
      black_leaf.at(children.at(i).at(j)) %= mod;
      prod *= black_root.at(children.at(i).at(j))+1;
      prod %= mod;
    }

    // 自身のblack_rootにも、全ての積を入れておく
    black_root.at(i) = prod;

  }

  // トポロジカルソートの正順にループ
  for (int id=0; id<n; id++) {

    // 調査する頂点番号
    int i = order.at(id);

    // 全ての子に対し、自分が葉になる数をかけてから+1する
    for (int child : children.at(i)) {
      black_leaf.at(child) *= black_leaf.at(i);
      black_leaf.at(child) += 1;
      black_leaf.at(child) %= mod;
    }

  }

  // 各頂点、「そこが黒の根になる数×そこが黒の葉になる数」が答え
  for (int i=0; i<n; i++) {
    result.at(i) = (black_root.at(i)*black_leaf.at(i))%mod;
  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}