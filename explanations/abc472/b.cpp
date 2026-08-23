#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int INF = 1001001001;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> l(n);
  for (int i=0; i<n; i++) {
    cin >> l.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = INF;

  //////////////////// 処理 ////////////////////

  // 最初は、全部右側に入れておく
  int left = 0;
  int right = accumulate(l.begin(),l.end(),0);

  // 順番に1つずつ左に移動して、左右の差を全探索して最小値を探す
  for (int i : l) {
    left += i;
    right -= i;
    result = min(result,abs(left-right));
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}