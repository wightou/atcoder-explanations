#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// ダイクストラ法
vector<long long> dijkstra(const vector<vector<pair<int,int>>>& graph, int start) {
  int n = ssize(graph);
  assert(start>=0);
  assert(start<n);
  vector<long long> distances(n,-1);
  priority_queue<pair<long long,int>,vector<pair<long long,int>>,greater<pair<long long,int>>> que;
  que.emplace(0,start);
  while (!que.empty()) {
    auto [dist,vertex] = que.top();
    que.pop();
    if (distances[vertex]!=-1) continue;
    distances[vertex] = dist;
    for (auto [cost,next] : graph[vertex]) {
      assert(cost>=0);
      if (distances[next]==-1) que.emplace(dist+cost,next);
    }
  }
  return distances;
}

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int n, q;
  cin >> n >> q;
  
  // 隣接リストにしつつ、aも残す
  vector<vector<pair<int,int>>> graph(n+1);
  vector<long long> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
    graph.at(i).emplace_back(a.at(i),(i+1)%n);
    graph.at((i+1)%n).emplace_back(a.at(i),i);
  }
  vector<long long> b(n);
  for (int i=0; i<n; i++) {
    cin >> b.at(i);
    graph.at(i).emplace_back(b.at(i),n);
    graph.at(n).emplace_back(b.at(i),i);
  }

  /////////////////// 前処理 ///////////////////

  // 中心にある頂点からの距離をダイクストラ法で出しておく
  vector<long long> dist = dijkstra(graph,n);
  
  // 外周だけ進む場合の距離の累積和
  vector<long long> sum(n+1,0);
  partial_sum(a.begin(),a.end(),sum.begin()+1);

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int s, t;
    cin >> s >> t;
    s--;
    t--;

    //////////////// 出力変数定義 ////////////////

    long long result = 0;

    //////////////////// 処理 ////////////////////

    // 片方が中心なら調査済
    // そうじゃないなら、「中心経由」「外周時計回り」「外周反時計回り」の3択の最小
    if (t!=n) {
      long long d1 = dist.at(s)+dist.at(t);
      long long d2 = sum.at(t)-sum.at(s);
      long long d3 = sum.at(n)-sum.at(t)+sum.at(s);
      result = min({d1,d2,d3});
    } else {
      result = dist.at(s);
    }

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}