#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// 詳細なBFS
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

  // 最初から、全部の辺の値を合計しておく
  // あとで、各頂点から「足されなかった番号の合計」をひく
  vector<long long> result(n,1LL*n*(n-1)/2);

  //////////////////// 処理 ////////////////////

  // 木の重心を求める、ここから（他の方法でもよい）

  // まず、適当に根付き木にして、トポロジカルソートの逆順も取得
  auto [distances0,parents0,children0,order0] = bfs(graph,{0});
  reverse(order0.begin(),order0.end());

  // 重心、すなわち本当に根としたい頂点番号
  int root = -1;

  // 葉側から順に、そこから先の部分木のサイズを求める
  // Nが奇数なので、初めてサイズがN/2を超えたところが唯一の重心
  vector<int> size0(n,1);
  for (int v : order0) {
    for (int c : children0.at(v)) {
      size0.at(v) += size0.at(c);
    } 
    if (size0.at(v)>n/2) {
      root = v;
      break;
    }
  }

  // 木の重心を求める、ここまで（他の方法でもよい）

  // あらためて、重心を根にして根付き木化する、トポロジカルソートも取得
  auto [distances,parents,children,order] = bfs(graph,{root});

  // 各頂点にxを用意する
  vector<long long> x(n,0);

  // 全ての辺について、葉側のxに辺番号を足す
  for (int i=0; i<m; i++) {
    if (distances.at(u.at(i))>distances.at(v.at(i))) x.at(u.at(i)) += i+1;
    else x.at(v.at(i)) += i+1;
  }

  // 根から葉に向かって累積和を取ると、各頂点の「足してもらえなかった値の合計」になる
  for (int v : order) {
    for (int c : children.at(v)) {
      x.at(c) += x.at(v);
    } 
  }

  // 全辺番号の合計から、それぞれの足してもらえなかった分を引くと答え
  for (int i=0; i<n; i++) {
    result.at(i) -= x.at(i);
  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i);
    if (i!=result.size()-1) {
      cout << " ";
    }
  }
  cout << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}