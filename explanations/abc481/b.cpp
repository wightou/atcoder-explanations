#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, k;
  cin >> n >> k;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 最初の点が条件を満たすか確認する
  if (a.at(0)==k) result++;

  // 1つずつ、区間途中でkを通るか確認する
  // 左端は含まず、右端は含むようにすることで、重複カウントを避ける
  for (int i=1; i<n; i++) {
    if (a.at(i-1)<k&&a.at(i)>=k) result++;
    if (a.at(i-1)>k&&a.at(i)<=k) result++;
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}