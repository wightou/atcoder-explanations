#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

struct Data{
  int b = 0;
  int w = 0;
  long long wb = 0;
  Data(char c) {
    if (c=='B') b++;
    else if (c=='W') w++;
  }
};

Data op(Data x, Data y) {
  Data z('.');
  z.b = x.b+y.b;
  z.w = x.w+y.w;
  z.wb = x.wb+y.wb+1LL*x.w*y.b;
  return z;
}

Data e() {
  return Data('.');
}

/////////////////// メイン ///////////////////

int main () {
  
  /////////////////// 前入力 ///////////////////
  
  int n, q;
  cin >> n >> q;

  /////////////////// 前処理 ///////////////////

  // WとBと.からなる文字列で、「WのBの組で、Bの方が右にあるものが何個あるか」を求めるsegment木
  // ある行を最後に触った時刻をB、ある列を最後に触った時刻をW、どちらでもないところは.とする
  // 最初に2n回事前に追加クエリがあるので、2n+q回分用意しておく
  segtree<Data,op,e> seg(2*n+q);

  // 最後に各行各列を操作した時刻を記録するタイムスタンプ
  vector<int> r_stamp(n,-1);
  vector<int> c_stamp(n,-1);

  // 時刻
  int time = 0;

  // 各行を全部触ってから、各列を全部触る
  // タイムスタンプを全部正常データにしておきながら、きちんと全部白に戻す
  for (int i=0; i<n; i++) {
    seg.set(time,Data('B'));
    r_stamp.at(i) = time;
    time++;
  }
  for (int i=0; i<n; i++) {
    seg.set(time,Data('W'));
    c_stamp.at(i) = time;
    time++;
  }

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {
    
    int type;
    cin >> type;
    
    if (type==1) {

      // クエリタイプ1の場合
      int r;
      cin >> r;
      r--;

      // 最後にその行を触った時刻を更新
      seg.set(r_stamp.at(r),Data('.'));
      seg.set(time,Data('B'));
      r_stamp.at(r) = time;
      time++;
      
    } else if (type==2) {

      // クエリタイプ2の場合
      int c;
      cin >> c;
      c--;

      // 最後にその列を触った時刻を更新
      seg.set(c_stamp.at(c),Data('.'));
      seg.set(time,Data('W'));
      c_stamp.at(c) = time;
      time++;

    }

    // segment木全体のprodが答え
    cout << seg.all_prod().wb << endl;
    
  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}