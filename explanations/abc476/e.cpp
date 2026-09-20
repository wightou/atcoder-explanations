#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int INF = 1001001001;

int op1(int a, int b) {
  return max(a,b);
}
int e1() {
  return 0;
};
int op2(int a, int b) {
  return min(a,b);
}
int e2() {
  return INF;
};

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int n, q;
  cin >> n >> q;

  vector<int> p(n);
  for (int i=0; i<n; i++) {
    cin >> p.at(i);
  }

  /////////////////// 前処理 ///////////////////

  // 逆写像を作っておく
  vector<int> pos(n+1,-1);
  for (int i=0; i<n; i++) {
    pos.at(p.at(i)) = i;
  }

  // 範囲内最大値/最小値を求めるsegment木
  segtree<int,op1,e1> seg1(p);
  segtree<int,op2,e2> seg2(p);

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int l, r;
    cin >> l >> r;
    l--;
    r--;

    //////////////////// 処理 ////////////////////

    // segment木から範囲内最大値/最小値を求める
    int mx = seg1.prod(l,r+1);
    int mn = seg2.prod(l,r+1);

    // 逆写像から、それらの位置を求める
    int pos_mx = pos.at(mx);
    int pos_mn = pos.at(mn);

    // セグ木上のデータを入れ替える
    seg1.set(pos_mx,mn);
    seg1.set(pos_mn,mx);
    seg2.set(pos_mx,mn);
    seg2.set(pos_mn,mx);

    // pと逆写像のデータも入れ替える
    swap(p.at(pos.at(mx)),p.at(pos.at(mn)));
    swap(pos.at(mx),pos.at(mn));

  }

  /////////////////// 後処理 ///////////////////

  for (size_t i=0; i<p.size(); i++) {
    cout << p.at(i);
    if (i!=p.size()-1) {
      cout << " ";
    }
  }
  cout << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}