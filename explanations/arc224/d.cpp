#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// 二項係数
long long comb(long long n, int r) {
  if (r<0) return 0;
  if (r>n) return 0;
  long long result = 1;
  for (long long i=n; i>n-r; i--) {
    result *= i;
    result /= (n+1-i);
  }
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

    int n, k;
    cin >> n >> k;

    //////////////// 出力変数定義 ////////////////

    long long result = 0;

    //////////////////// 処理 ////////////////////

    // 大きな数ほど、使う個数が少なくて済むようにする
    // 「i個使う全パターンを割り当てていく」をiが小さい方から見る
    for (int i=0; (i<=n&&k>0); i++) {

      // その個数の残りパターン
      // iが大きいとオーバーフローするが、その前に決着するはず
      long long rest = comb(n,i);

      // そのパターン1つを、未割り当てである値の最大値に割り当てる
      while (rest>0&&k>0) {
        result += i*ssize(to_string(k));
        rest--;
        k--;
      }

    }

    // まだ残っていたら、不可能ということなので出力を-1にする
    if (k>0) result = -1;

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}