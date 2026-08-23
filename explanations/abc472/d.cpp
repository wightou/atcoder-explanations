#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// 幅優先探索（多始点）
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

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int h, w, k;
  cin >> h >> w >> k;

  string maze_txt = "";
  for (int i=0; i<h; i++){
    string tmp;
    cin >> tmp;
    maze_txt += tmp;
  }
  
  // 迷路作成
  vector<vector<int>> maze(h*w);
  for (int i=0; i<h*w; i++){
    if (i<w*(h-1) && maze_txt.at(i)!='#' && maze_txt.at(i+w)!='#') {
      maze.at(i).emplace_back(i+w);
      maze.at(i+w).emplace_back(i);
    }
    if (i%w<w-1 && maze_txt.at(i)!='#' && maze_txt.at(i+1)!='#') {
      maze.at(i).emplace_back(i+1);
      maze.at(i+1).emplace_back(i);
    }
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 爆弾マスの位置から、安全な列と行を調査する
  vector<bool> safe_h(h,true);
  vector<bool> safe_w(w,true);
  for (int i=0; i<h; i++) {
    for (int j=0; j<w; j++) {
      if (maze_txt.at(i*w+j)=='#') {
        safe_h.at(i) = false;
        safe_w.at(j) = false;
      }
    }
  }

  // 安全な列と行の情報から、安全マスを列挙する
  vector<int> starts;
  for (int i=0; i<h; i++) {
    for (int j=0; j<w; j++) {
      if (safe_h.at(i)&&safe_w.at(j)) starts.emplace_back(i*w+j);
    }
  }

  // 多始点BFSに投げる
  vector<int> distances = bfs(maze,starts);
  
  // 安全マスまでk歩以内であるマスの数を数える
  for (int d : distances) {
    if (d>=0&&d<=k) result++;
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}