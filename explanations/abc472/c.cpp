#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  long long k;
  cin >> n >> m >> k;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////



  //////////////////// 処理 ////////////////////

  // m-1日前からの合計
  long long sum = 0;

  // 1日ずつ調べる
  for (int i=0; i<n; i++) {

    // m日前に食べた分を引く
    if (i>=m) sum -= a.at(i-m);

    // 今日を食べると規定オーバーな場合は、なかったことにする
    if (sum+a.at(i)>k) a.at(i) = 0;

    // 今日の分を足す
    sum += a.at(i); 

  }

  //////////////////// 出力 ////////////////////

  // 正の数だったらYes、0だったらNoを答える
  for (size_t i=0; i<a.size(); i++) {
    if (a.at(i)>0) cout << "Yes" << endl;
    else cout << "No" << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}