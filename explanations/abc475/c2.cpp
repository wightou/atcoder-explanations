#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, s;
  long long l;
  cin >> n >> s >> l;
  s--;

  vector<long long> a(n-1);
  for (int i=0; i<n-1; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 累積和で、各都市の位置を出しておく
  vector<long long> sum(n);
  partial_sum(a.begin(),a.end(),sum.begin()+1);

  // 左端と右端を決めて全探索
  for (int i=0; i<=s; i++) {
    for (int j=s; j<n; j++) {

      // 左側。右側の距離
      long long l1 = sum.at(s)-sum.at(i);
      long long l2 = sum.at(j)-sum.at(s);

      // 近い方へ先に行く前提で、距離l以内で到達できるなら、到達地点数の最大値更新
      if (l1+l2+min(l1,l2)<=l) result = max(result,j-i+1);

    }
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}