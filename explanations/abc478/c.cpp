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

  string result = "No";

  //////////////////// 処理 ////////////////////

  // コピーして全体をソートする
  vector<int> b = a;
  sort(b.begin(),b.end());

  // ソートに含めなければならない、最も左の位置を調べる
  int l = 0;
  while (l<n&&a.at(l)==b.at(l)) l++;

  // ソートに含めなければならない、最も右の位置を調べる
  int r = n-1;
  while (r>=0&&a.at(r)==b.at(r)) r--;

  // その幅r-l+1がk以下なら、条件を満たして範囲を選べる
  if (r-l+1<=k) result = "Yes";

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}