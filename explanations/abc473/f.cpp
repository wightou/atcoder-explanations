#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int INF = 1001001001;

// 区間最小値と範囲加算ができる遅延segment木用の関数
int op(int a, int b) {
  return min(a,b);
}
int e() {
  return INF;
}
int mp(int f, int x) {
  return f+x;
}
int com(int f, int g) {
  return f+g;
}
int id() {
  return 0;
}

/////////////////// メイン ///////////////////

int main () {
  
  /////////////////// 前入力 ///////////////////
  
  int n, q;
  string s;
  cin >> n >> s >> q;

  /////////////////// 前処理 ///////////////////

  // 区間最小値と範囲加算ができる遅延segment木
  // 「より左」型累積和にしたいので、長さをn+1にする
  lazy_segtree<int,op,e,int,mp,com,id> seg(n+1);

  // 0番は0としておく
  seg.set(0,0);

  // 累積和計算用
  int sum = 0;

  // 'A'は+1、'B'は-1として「より左」の累積和を作る
  for (int i=0; i<n; i++) {
    if (s.at(i)=='A') sum++;
    else sum--;
    seg.set(i+1,sum);
  }

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {
    
    int type;
    cin >> type;
    
    if (type==1) {

      int x;
      char c;
      cin >> x >> c;
      x--;

      // もし、書き換える必要がないなら何もせずスキップ
      if (s.at(x)==c) continue;
      
      // 書き換えが行われた場合、累積和を範囲加算で更新する
      // 'A'→'B'なら-2、逆なら+2
      s.at(x) = c;
      if (c=='A') seg.apply(x+1,n+1,2);
      else seg.apply(x+1,n+1,-2);
      
    } else if (type==2) {

      int l, r;
      cin >> l >> r;
      l--;
      r--;

      // 該当範囲（l直前からr直後まで）のなかで、l直前が最小値（タイでも可）だったら問題ない
      if (seg.prod(l,r+2)==seg.get(l)) cout << "Yes" << endl;
      else cout << "No" << endl;

    }

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}