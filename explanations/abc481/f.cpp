#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;

// 幅優先探索
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

// 座標圧縮
struct Compression {
  vector<long long> vec;
  Compression(const vector<long long>& vec_ = {}) {
    vec = vec_;
    sort(vec.begin(),vec.end());
    vec.erase(unique(vec.begin(),vec.end()),vec.end());
  }
  int lower(long long x) {
    int y = lower_bound(vec.begin(),vec.end(),x)-vec.begin();
    return y;
  }
  int upper(long long x) {
    int y = upper_bound(vec.begin(),vec.end(),x)-vec.begin();
    return y;
  }
  int encode(long long x) {
    int y = lower_bound(vec.begin(),vec.end(),x)-vec.begin();
    assert(y<ssize(vec)&&vec[y] == x);
    return y;
  }
  long long decode (int y) {
    assert(0<=y&&y<ssize(vec));
    return vec.at(y);
  }
  long long left (int y) {
    assert(1<=y&&y<ssize(vec));
    return vec.at(y)-vec.at(y-1);
  }
  long long right (int y) {
    return left(y+1);
  }
  int size() {
    return ssize(vec);
  }
};

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;
  
  // コストは受け取るが、記録だけして重みなし有向グラフにしておく
  vector<int> u(m), v(m), w(m);
  vector<vector<int>> graph(n);
  for (int i=0; i<m; i++) {
    cin >> u.at(i) >> v.at(i) >> w.at(i);
    u.at(i)--;
    v.at(i)--;
    graph.at(u.at(i)).emplace_back(v.at(i));
  }

  //////////////// 出力変数定義 ////////////////

  vector<long long> result(n,0);

  //////////////////// 処理 ////////////////////

  // bfsで頂点0からの距離をもとめ、距離ごとに分類しておく
  vector<int> dist = bfs(graph,{0});
  int mx = *max_element(dist.begin(),dist.end())+1;
  vector<vector<int>> bucket(mx);
  for (int i=0; i<n; i++) bucket.at(dist.at(i)).emplace_back(i);

  // 最短路に使われる可能性がある辺の逆向きに重み付き辺を張ってグラフを再構築
  vector<vector<pair<int,int>>> parents(n);
  for (int i=0; i<m; i++) {
    if (dist.at(u.at(i))<dist.at(v.at(i))) parents.at(v.at(i)).emplace_back(w.at(i),u.at(i));
  }

  // 距離が近い側から、順位付けしながら答えを求めていく
  vector<int> label(n,mod);
  label.at(0) = 0;
  for (int i=1; i<mx; i++) {

    // 座標圧縮に渡すデータ
    vector<long long> tmp;

    // 距離がiである頂点jそれぞれ、親kから来るルートを調べ、最小値となる経路とその場合の剰余を記録
    for (int j : bucket.at(i)) {
      for (auto [c,k] : parents.at(j)) {
        int new_label = label.at(k)*10+c;
        if (new_label<label.at(j)) {
          label.at(j) = new_label;
          result.at(j) = (result.at(k)*10+c)%mod;
        }
      }
      tmp.emplace_back(label.at(j));
    }

    // 距離がiである頂点のラベルを座標圧縮する
    Compression comp(tmp);
    for (int j : bucket.at(i)) {
      label.at(j) = comp.encode(label.at(j));
    }

  }

  //////////////////// 出力 ////////////////////

  for (size_t i=1; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}