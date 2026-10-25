#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int INF = 1001001001;

// 偏角の整数対応（-π<θ<=πを-4m^2<x<=4m^2の整数に対応させる）
long long calc (long long x, long long y, long long m) {
  assert(x!=0||y!=0);
  bool sign_y = true;
  if (y<0) {sign_y = false; y *= -1;}
  bool sign_x = true;
  if (x<0) {sign_x = false; x *= -1;}
  bool sign_d = true;
  if (x<y) {sign_d = false; swap(x,y);}
  long long result = m*y/x*m + m*y%x*m/x;
  if (!sign_d) result = 2*m*m - result;
  if (!sign_x) result = 4*m*m - result;
  if (!sign_y) result *= -1;
  return result;
}

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////



  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int n;
    cin >> n;

    vector<int> x(n), y(n);
    for (int i=0; i<n; i++) {
      cin >> x.at(i) >> y.at(i);
    }

    //////////////// 出力変数定義 ////////////////

    int result = 0;

    //////////////////// 処理 ////////////////////

    // すべての点の偏角を整数に対応させる
    // 値の範囲は -4*INF^2<値<=4*INF^2 で、偏角ちょうどπ違いは4m^2違いに相当
    vector<long long> vec(n);
    for (int i=0; i<n; i++) vec.at(i) = calc(x.at(i),y.at(i),INF);

    // 偏角ソートする
    sort(vec.begin(),vec.end());

    // 尺取り法で、差が4m^2の区間の最大個数を、値最小を含む側と含まない側で個別にやる
    for (int l=0, r=0; r<n; r++) {
      while (vec.at(r)-vec.at(l)>4LL*INF*INF) l++;
      result = max(result,r-l+1);
      if (l>0) {
        result = max(result,n-r+l);
      }
    }

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}