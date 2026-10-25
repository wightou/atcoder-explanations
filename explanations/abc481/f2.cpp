#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;

// Dijkstra法（ライブラリから改造）
// 距離が近い方からi番目のところから距離jで行ける場合、そこの距離は10i+jとする
pair<vector<long long>,vector<int>> dijkstra(const vector<vector<pair<int,int>>>& graph, int start) {
  int n = ssize(graph);
  assert(start>=0);
  assert(start<n);
  vector<long long> distances(n,-1);
  int label = 1;  // 圧縮用に追加
  int now_dist = 0;  // 圧縮用に追加
  vector<int> remainder(n,-1);  // 剰余用に追加
  priority_queue<tuple<long long,long long,int>,vector<tuple<long long,long long,int>>,greater<tuple<long long,long long,int>>> que;
  que.emplace(0,0,start);
  while (!que.empty()) {
    auto [dist,rem,vertex] = que.top();
    que.pop();
    if (distances[vertex]!=-1) continue;
    distances[vertex] = dist;
    remainder[vertex] = rem;  // 剰余用に追加
    if (dist>now_dist) {  // 圧縮用に追加
      label++;  // 圧縮用に追加
      now_dist = dist;  // 圧縮用に追加
    }
    for (auto [cost,next] : graph[vertex]) {
      assert(cost>=0);
      if (distances[next]==-1) que.emplace(label*10+cost,(rem*10+cost)%mod,next);  // 編集
    }
  }
  return {distances,remainder};
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;
  
  vector<int> u(m), v(m), w(m);
  vector<vector<pair<int,int>>> graph(n);
  for (int i=0; i<m; i++) {
    cin >> u.at(i) >> v.at(i) >> w.at(i);
    u.at(i)--;
    v.at(i)--;
    graph.at(u.at(i)).emplace_back(w.at(i),v.at(i));
  }

  //////////////// 出力変数定義 ////////////////



  //////////////////// 処理 ////////////////////

  // 改造Dijkstra法で圧縮距離と余りを求める
  auto [dist,remainder] = dijkstra(graph,0);

  //////////////////// 出力 ////////////////////

  for (size_t i=1; i<remainder.size(); i++) {
    cout << remainder.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}