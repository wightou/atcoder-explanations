#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;

  vector<int> a(m), b(m);
  for (int i=0; i<m; i++) {
    cin >> a.at(i) >> b.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  // 条件を満たす人の組を入れる set （重複削除目的）
  set<pair<int,int>> result;

  //////////////////// 処理 ////////////////////

  // 最初の試合に出ていた人中心で候補ごとに全探索
  for (int i : {a.at(0),b.at(0)}) {

    // 中心にする人以外を全員入れた set を用意
    set<int> st;
    for (int j=1; j<=n; j++) {
      if (i!=j) st.emplace(j);
    }

    // 中心にいる人が出ていない試合について、その試合に出ていた人以外を消す
    // （本当に消す実装にすると面倒なので、残る人を入れた set を新しく作って swap）
    for (int k=0; k<m; k++) {
      if (a.at(k)==i||b.at(k)==i) continue;
      set<int> next;
      if (st.contains(a.at(k))) next.emplace(a.at(k));
      if (st.contains(b.at(k))) next.emplace(b.at(k));
      swap(st,next);
    }

    // 最後まで残っていた人に対し、ペアを作って result に入れる
    for (int j : st) {
      result.emplace(min(i,j),max(i,j));
    }

  }

  //////////////////// 出力 ////////////////////

  cout << result.size() << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}