#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  vector<int> result(n);

  //////////////////// 処理 ////////////////////

  // 両端の場合は、逆サイドに感染させるまでの時間が答え
  result.at(0) = (a.at(n-1)-a.at(0))/2;
  result.at(n-1) = (a.at(n-1)-a.at(0))/2;

  // それ以外の場合は、以下の2つのうち速い方が答え
  // - 自分が最も左の人に感染させ、1つ左の人が最も右の人に感染させる
  // - 自分が最も右の人に感染させ、1つ右の人が最も左の人に感染させる
  for (int i=1; i<n-1; i++) {
    result.at(i) = min(max(a.at(n-1)-a.at(i),a.at(i+1)-a.at(0)),max(a.at(n-1)-a.at(i-1),a.at(i)-a.at(0)))/2;
  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}