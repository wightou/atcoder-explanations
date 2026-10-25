#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

vector<int> bfs01(const vector<vector<pair<int,int>>>& graph, const vector<int>& starts) {
  int n = ssize(graph);
  vector<int> distances(n,-1);
  deque<int> que;
  que.emplace_back(-1);
  for (int v : starts) {
    assert(v>=0);
    assert(v<n);
    que.emplace_front(v);
  }
  for (int d=0; ssize(que)>1; d++) {
    while (que.front()!=-1) {
      int v = que.front();
      que.pop_front();
      if (distances[v]!=-1) continue;
      distances[v] = d;
      for (auto [cost,next] : graph[v]) {
        assert(cost==0||cost==1);
        if (distances[next]==-1) {
          if (cost==0) que.emplace_front(next);
          else que.emplace_back(next);
        }
      }
    }
    que.pop_front();
    que.emplace_back(-1);
  }
  return distances;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m, s, t;
  cin >> n >> m >> s >> t;
  s--;
  t--;
  
  vector<int> u(m), v(m);
  vector<vector<pair<int,int>>> graph(n);
  for (int i=0; i<m; i++) {
    cin >> u.at(i) >> v.at(i);
    u.at(i)--;
    v.at(i)--;
    graph.at(u.at(i)).emplace_back(1,v.at(i));
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // グラフを各頂点ごとにソートして、先頭のコストを0に変更
  for (auto& vec : graph) {
    if (vec.empty()) continue;
    sort(vec.begin(),vec.end());
    vec.front().first = 0;
  }

  // sをスタート地点として、01BFSをする
  vector<int> d = bfs01(graph,{s});

  // tまでの最小コストが答え
  result = d.at(t);

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}