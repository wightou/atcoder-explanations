#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// Dijkstra法
void dijkstra(const vector<vector<pair<int,int>>>& graph, vector<long long>& distances, const int& start) {
  priority_queue<pair<long long,int>,vector<pair<long long,int>>, greater<pair<long long,int>>> que;
  que.emplace(0,start);
  while (!que.empty()) {
    auto [dist,vertex] = que.top();
    que.pop();
    if (distances[vertex]!=-1) continue;
    distances[vertex] = dist;
    for (pair<long long,int> next : graph[vertex]) {
      next.first += dist;
      if (distances[next.second]==-1||next.first<distances[next.second]) que.emplace(next);
    }
  }
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m, y;
  cin >> n >> m >> y;

  // 重み付き隣接リスト「重み、行き先頂点」で受け取る
  vector<int> u(m), v(m), w(m);
  vector<vector<pair<int,int>>> graph(n+2);  // 超頂点2つを追加で用意
  for (int i=0; i<m; i++) {
    cin >> u.at(i) >> v.at(i) >> w.at(i);
    u.at(i)--;
    v.at(i)--;
    graph.at(u.at(i)).emplace_back(w.at(i),v.at(i));
    graph.at(v.at(i)).emplace_back(w.at(i),u.at(i));
  }

  // 各頂点→ワープ入口、ワープ出口→各頂点を用意
  vector<long long> x(n);
  for (int i=0; i<n; i++) {
    cin >> x.at(i);
    graph.at(i).emplace_back(x.at(i),n);
    graph.at(n+1).emplace_back(x.at(i),i);
  }

  // ワープ入口→ワープ出口を用意
  graph.at(n).emplace_back(y,n+1);

  //////////////// 出力変数定義 ////////////////

  vector<long long> result(n+2,-1);

  //////////////////// 処理 ////////////////////

  // Dijkstra法を実行
  dijkstra(graph,result,0);

  //////////////////// 出力 ////////////////////

  // 該当範囲だけ出力する
  for (int i=1; i<n; i++) {
    cout << result.at(i);
    if (i!=n-1) {
      cout << " ";
    }
  }
  cout << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}