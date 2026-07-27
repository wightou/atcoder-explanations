#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// トポロジカルソート
vector<int> topological_sort(const vector<vector<int>>& graph) {
  int n = graph.size();
  vector<int> degrees(n,0);
  deque<int> que;
  vector<int> result;
  result.reserve(n);
  for (const auto& vec : graph) for (int v : vec) degrees[v]++;
  for (int i=0; i<n; i++) if (degrees[i]==0) que.emplace_back(i);
  while (!que.empty()) {
    int v = que.front();
    que.pop_front();
    result.emplace_back(v);
    for (int u : graph[v]) {
      degrees[u]--;
      if (degrees[u]==0) que.emplace_back(u);
    }
  }
  if (ssize(result)<n) result.clear();
  return result;
}

/////////////////// メイン ///////////////////

int main () {
  
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
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // トポロジカルソートして逆順にしておく
  vector<int> vec = topological_sort(graph);
  reverse(vec.begin(),vec.end());

  // DPテーブル
  // dp[i] は、頂点 i から始めて最大で何本の辺を辿れるか
  vector<int> dp(n,0);

  // トポロジカルソートの逆順にループしてDPテーブルを埋める
  for (int i : vec) {
    for (int j : graph.at(i)) {
      dp.at(i) = max(dp.at(i),dp.at(j)+1);
    }
  }
  
  // DPテーブル内の最大値が答え
  result = *max_element(dp.begin(),dp.end());

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}