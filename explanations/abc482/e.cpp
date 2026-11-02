#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

vector<int> bfs(const vector<vector<int>>& graph, const vector<int>& starts) {
  int n = ssize(graph);
  vector<int> distances(n,-1);
  deque<int> que;
  for (int v : starts) {
    assert(v>=0);
    assert(v<n);
    assert(distances[v]==-1);
    distances[v] = 0;
    que.emplace_back(v);
  }
  while (!que.empty()) {
    for (int v : graph[que.front()]) {
      if (distances[v]==-1) {
        distances[v] = distances[que.front()]+1;
        que.emplace_back(v);
      }
    }
    que.pop_front();
  }
  return distances;
}

tuple<vector<int>,vector<int>,vector<vector<int>>,vector<int>> bfs2(const vector<vector<int>>& graph, const vector<int>& starts) {
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

  vector<int> c(n);
  for (int i=0; i<n; i++) {
    cin >> c.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 木の直径を求め、その両端からのBFSをする
  int vertex0 = 0;
  vector<int> dist0 = bfs(graph,{vertex0});
  int vertex1 = max_element(dist0.begin(),dist0.end())-dist0.begin();
  vector<int> dist1 = bfs(graph,{vertex1});
  int d = *max_element(dist1.begin(),dist1.end());
  int vertex2 = max_element(dist1.begin(),dist1.end())-dist1.begin();
  vector<int> dist2 = bfs(graph,{vertex2});

  // 直径が偶数か奇数かで、別アルゴリズムで処理
  if (d%2) {

    // 奇数の場合

    // 中心になる辺の両端を探す
    int center1 = -1;
    int center2 = -1;
    for (int i=0; i<n; i++) {
      if (dist1.at(i)==d/2&&dist2.at(i)==d/2+1) center1 = i;
      if (dist1.at(i)==d/2+1&&dist2.at(i)==d/2) center2 = i;
    }

    // 中心辺の両端からの簡易BFS
    vector<int> dist3 = bfs(graph,{center1});
    vector<int> dist4 = bfs(graph,{center2});
    vector<int> counters(n+1,0);

    // 片方から距離 d/2+1 （つまり中心辺の奥に距離d/2）である点の数を色を調べる
    for (int i=0; i<n; i++) {
      if (dist3.at(i)==d/2+1) {
        counters.at(c.at(i))++;
      }
    }

    // もう片方から距離 d/2+1 （つまり中心辺の奥に距離d/2）である点ごとに色を調べ、直径の相方数をカウント
    for (int i=0; i<n; i++) {
      if (dist4.at(i)==d/2+1) {
        result += counters.at(c.at(i));
      }
    }

  } else {

    // 偶数の場合

    // 中心となる頂点を探す
    int center = -1;
    for (int i=0; i<n; i++) {
      if (dist1.at(i)==d/2&&dist2.at(i)==d/2) center = i;
    }

    // 中心からの詳細BFSをする
    auto [dist3,parents,children,order] = bfs2(graph,{center});

    // 木DPで、中心から各頂点へ向かうときの1つめの頂点番号を求めておく
    vector<int> branch(n,-1);
    for (int v : order) {
      if (parents.at(v)==-1) continue;
      if (parents.at(v)==center) branch.at(v) = v;
      else branch.at(v) = branch.at(parents.at(v));
    }

    // 中心から距離 d/2 である点で、同じ色のペアであるものを数える
    // ただし、1つめの頂点番号も同じであるペアは足さないようにする
    vector<int> counters(n+1,0);
    map<pair<int,int>,int> counters_with_branch;
    for (int i=0; i<n; i++) {
      if (dist3.at(i)==d/2) {
        result += counters.at(c.at(i));
        counters.at(c.at(i))++;
        result -= counters_with_branch[{c.at(i),branch.at(i)}];
        counters_with_branch[{c.at(i),branch.at(i)}]++;
      }
    }

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}