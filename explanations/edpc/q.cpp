#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// 最大値を求めるsegment木の関数
long long op(long long a, long long b) {
  return max(a,b);
}
long long e() {
  return 0;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> h(n);
  for (int i=0; i<n; i++) {
    cin >> h.at(i);
    h.at(i)--;
  }
  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 最大値を求めるsegment木
  // 位置 i には、高さ i の花を最後に残す場合の最大価値を入れる
  segtree<long long,op,e> seg(n);

  // 前から順に見るループ
  for (int i=0; i<n; i++) {

    // その花を最後に残すものにする場合の最大値
    long long value = seg.prod(0,h.at(i))+a.at(i);

    // segment木に入れる
    seg.set(h.at(i),value);

  }

  // 全体の最大値が答え
  result = seg.all_prod();

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}