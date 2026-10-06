#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, q;
  cin >> n >> q;

  vector<int> t(q), u(q), v(q);
  for (int i=0; i<q; i++) {
    cin >> t.at(i) >> u.at(i) >> v.at(i);
    u.at(i)--;
    v.at(i)--;
  }

  //////////////// 出力変数定義 ////////////////

  bool flag = true;
  vector<int> result(n);

  //////////////////// 処理 ////////////////////

  // 入力を有向グラフとして、強連結成分分解する
  scc_graph g(n);
  for (int i=0; i<q; i++) {
    g.add_edge(u.at(i),v.at(i));
  }
  vector<vector<int>> s = g.scc();

  // 各頂点、1-indexedで成分番号を入れていく
  for (int i=0; i<ssize(s); i++) {
    for (int v : s.at(i)) {
      result.at(v) = i+1;
    }
  }

  // これで作ったのが条件を満たしていればそれが答え、満たしていなければどうやっても不可能
  for (int i=0; i<q; i++) {
    if (t.at(i)==0) continue;
    if (result.at(u.at(i))==result.at(v.at(i))) flag = false;
  }

  //////////////////// 出力 ////////////////////

  if (flag) {
    cout << "Yes" << endl;
    for (size_t i=0; i<result.size(); i++) {
      cout << result.at(i);
      if (i!=result.size()-1) {
        cout << " ";
      }
    }
    cout << endl;
  } else {
    cout << "No" << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}