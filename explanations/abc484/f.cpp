#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

int op1(int x, int y) {
  return max(x,y);
}
int op2(int x, int y) {
  if (x==-1) return y;
  if (y==-1) return x;
  return min(x,y);
}
int e() {
  return -1;
}

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int n, q;
  cin >> n >> q;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  /////////////////// 前処理 ///////////////////

  // 最大値segment木
  segtree<int,op1,e> seg1(n);

  // -1を除く最小値segment木
  segtree<int,op2,e> seg2(n);

  // segment木内にデータを入れる
  for (int i=0; i<n; i++) {
    seg1.set(i,a.at(i));
    seg2.set(i,a.at(i));
  }

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {
    
    int type;
    cin >> type;
    
    if (type==1) {

      int i, x;
      cin >> i >> x;
      i--;

      // segment木の値を変えるだけ
      seg1.set(i,x);
      seg2.set(i,x);
      
    } else if (type==2) {

      int l, r;
      cin >> l >> r;
      l--;

      // 両者の区間集約値が一致していたら "Yes" で、そうでなければ "No"
      if (seg1.prod(l,r)==seg2.prod(l,r)) cout << "Yes" << endl;
      else cout << "No" << endl;

    }

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}