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

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> a(m), b(m), c(m);
    for (int i=0; i<m; i++) {
      cin >> a.at(i) >> b.at(i) >> c.at(i);
      a.at(i)--;
      b.at(i)--;
    }

    //////////////// 出力変数定義 ////////////////

    bool flag = false;
    set<int> result;

    //////////////////// 処理 ////////////////////

    // 重み0の辺だけ拾ったUnion-Find木
    dsu d0(n);

    // 重み1の辺だけ拾ったUnion-Find木
    dsu d1(n);

    // それぞれのUnion-Find木を作る
    for (int i=0; i<m; i++) {
      if (c.at(i)==0) d0.merge(a.at(i),b.at(i));
      else d1.merge(a.at(i),b.at(i));
    }

    // ssize(d0.groups())-1だけ重み1の辺を使う必要がある
    // ssize(d1.groups())-1だけ重み0の辺を使う必要がある
    // それを満たしているなら可能
    if (ssize(d0.groups())-1<=k&&ssize(d1.groups())-1<=n-1-k) flag = true;

    // 可能だったら構築する
    if (flag) {

      // 重み0の辺の残り本数
      int count0 = n-1-k;

      // 重み1の辺の残り本数
      int count1 = k;

      // 目的の木を作成するUnion-Find木
      dsu d(n);

      // まず、最低限必要な辺を確保するループ
      for (int i=0; i<m; i++) {

        if (c.at(i)==0&&!d1.same(a.at(i),b.at(i))) {

          // d1側で非連続なところを結ぶ重み0の辺は採用
          result.emplace(i);
          d.merge(a.at(i),b.at(i));
          count0--;

          // d1も同じところを連結にしてしまう
          d1.merge(a.at(i),b.at(i));

        } else if (c.at(i)==1&&!d0.same(a.at(i),b.at(i))) {

          // d0側で非連続なところを結ぶ重み1の辺は採用
          result.emplace(i);
          d.merge(a.at(i),b.at(i));
          count1--;

          // d0も同じところを連結にしてしまう
          d0.merge(a.at(i),b.at(i));

        }
      }

      // 残りの連結成分は重み0でも重み1でも他の連結成分につなげるので、適当に残り本数を見ながら木にする
      for (int i=0; i<m; i++) {
        if (d.same(a.at(i),b.at(i))) continue;
        if (c.at(i)==0) {
          if (count0==0) continue;
          result.emplace(i);
          d.merge(a.at(i),b.at(i));
          count0--;
        } else {
          if (count1==0) continue;
          result.emplace(i);
          d.merge(a.at(i),b.at(i));
          count1--;
        }
      }

    }

    //////////////////// 出力 ////////////////////

    if (flag) {
      for (int i : result) {
        cout << i+1 << " ";
      }
      cout << endl;
    } else {
      cout << -1 << endl;
    }

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}