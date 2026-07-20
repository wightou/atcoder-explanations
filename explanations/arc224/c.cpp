#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// 深さ優先探索
void dfs(const vector<vector<int>>& graph, vector<int>& d, int cur) {
  for (int next : graph.at(cur)) {
    if (d.at(next)==-1) {
      d.at(next) = d.at(cur) + 1;
      dfs(graph,d,next);
    }
  }
}

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////



  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int n, m;
    cin >> n >> m;

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

    vector<int> result(n,-1);

    //////////////////// 処理 ////////////////////

    // DFSするだけ
    result.at(0) = 0;
    dfs(graph,result,0);

    //////////////////// 出力 ////////////////////

    for (size_t i=0; i<result.size(); i++) {
      cout << result.at(i);
      if (i!=result.size()-1) {
        cout << " ";
      }
    }
    cout << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}