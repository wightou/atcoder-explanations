#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

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
    
    vector<int> u(m), v(m), w(m);
    for (int i=0; i<m; i++) {
      cin >> u.at(i) >> v.at(i) >> w.at(i);
      u.at(i)--;   // 0-indexedにしておく
      v.at(i)--;
    }

    //////////////// 出力変数定義 ////////////////

    vector<int> result;

    //////////////////// 処理 ////////////////////

    // それぞれの人が移動できる範囲をUnionFind木にする
    dsu takahashi(n);
    dsu taka(n);
    dsu hashi(n);
    for (int i=0; i<m; i++) {
      if (w.at(i)==1) takahashi.merge(u.at(i),v.at(i));
      if (w.at(i)==2) taka.merge(u.at(i),v.at(i));
      if (w.at(i)==3) hashi.merge(u.at(i),v.at(i));
    }

    // takaでもhashiでも同じグループにいる頂点の組を探す
    // みつけたら、takahashiを連結してしまってよい
    map<pair<int,int>,int> mp;
    for (int i=0; i<n; i++) {
      if (mp.contains({taka.leader(i),hashi.leader(i)})) {
        takahashi.merge(i,mp[{taka.leader(i),hashi.leader(i)}]);
      } else {
        mp[{taka.leader(i),hashi.leader(i)}] = i;
      }
    }

    // takahashiでスタート地点からつながっているものを列挙
    for (int i=0; i<n; i++) {
      if (takahashi.same(i,0)) result.emplace_back(i+1);
    }

    //////////////////// 出力 ////////////////////

    cout << ssize(result) << endl;
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