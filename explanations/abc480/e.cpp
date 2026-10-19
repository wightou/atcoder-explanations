#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr long long INF = 1001001001001001001LL;
constexpr int INFint = 1001001001;

// オーバーフロー防止機能付き掛け算
long long prod (long long x, long long y) {
  if (2*INF/x<y) return INF;
  if (x*y>INF) return INF;
  return x*y;
}

// 二分探索
// 初項a0、項数lの数列で、総積の値がlim以下になるような公差の最大値を求める
long long bin_search(long long inside, long long outside, long long lim, int a0, int l) {
  while (abs(inside-outside)>1) {
    long long mid = inside+(outside-inside)/2;
    long long p = a0;
    long long a = a0;
    for (int i=0; i<l-1; i++) {
      a += mid;
      p = prod(p,a);
    } 
    if (p<=lim) {
      inside = mid;
    } else {
      outside = mid;
    }
  }
  return inside;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  long long n;
  cin >> n;

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 長さ3から19まで試す
  for (int l=3; l<20; l++) {

    // 公差調査の上限初期値
    int count = INFint;

    // 初項1から、公差が1でいける限り試すループ
    for (int a0=1; ; a0++) {

      // 初項a0、項数lの数列で、総積の値がn以下になるような公差の最大値を二分探索で求める
      count = bin_search(0,count+1,n,a0,l);

      // 公差が1でも無理なら、これ以上初項を上げても意味がないので打ち切り
      if (count==0) break;

      // 正の公差がOKなら、その個数を結果に加える
      result += count;

    }

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}