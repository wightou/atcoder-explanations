#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr long long INF = 1001001001001001001LL;

pair<long long,long long> op (pair<long long,long long> a, pair<long long,long long> b) {
  return min(a,b);
}
pair<long long,long long> e() {
  return {INF,INF};
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> p(n);
  for (int i=0; i<n; i++) {
    cin >> p.at(i);
    p.at(i)--; // 0-indexed化
  }

  vector<long long> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 自分より前を見ていき最初に出てくる自分より大きな数の位置
  // 存在しなければ、先頭を示す（先頭自身は使わないのでダミーを入れておく）
  vector<long long> b(n,0);
  b.at(0) = -1;

  // それを stack（のつもりのdeque）を用いて、後ろから順に構築する
  // 最後に残ったままになるデータは、初期値の0が入ったままになるのでよし
  deque<pair<int,int>> que;
  for (int i=n-1; i>=0; i--) {
    while (!que.empty()&&que.back().first<p.at(i)){
      b.at(que.back().second) = i;
      que.pop_back();
    }
    que.emplace_back(p.at(i),i);
  }

  // 後で、順列Pの順に頂点を生やしていく
  // その際、i番目に並べられることになる頂点の、子を持つときのコスト
  // （iが頂点番号ではないので注意）
  // k個子がある状態で次の子を持つには、A[頂点番号]*(2k+1) かかる
  // 最小値segment木で、最小値とその位置の取得、値の動的更新ができるようにしておく
  segtree<pair<long long,long long>,op,e> seg(n);
  vector<int> counters(n,0);
  for (int i=0; i<n; i++) {
    seg.set(i,{a.at(p.at(i))*(2*counters.at(i)+1),i});
  }

  // 順列Pの順に頂点を生やしていく（先頭は最初からおいてある）
  // bを見て親として選択できる範囲を調べ、最も安いものを選択する
  // それを結果に加算したら、その親のコストを更新する
  for (int i=1; i<n; i++) {
    auto [cost,pos] = seg.prod(b.at(i),i);
    result += cost;
    counters.at(pos)++;
    seg.set(pos,{a.at(p.at(pos))*(2*counters.at(pos)+1),pos});
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}