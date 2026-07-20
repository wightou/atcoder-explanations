#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }
  vector<int> b(n-1);
  for (int i=0; i<n-1; i++) {
    cin >> b.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 先頭に1すら足さない場合の、各値に（M回未満で）何回足すことになるか求める配列
  vector<int> d(n,0);

  // dの情報を埋めていく
  for (int i=1; i<n; i++) {
    d.at(i) = (0LL+b.at(i-1)-a.at(i)-a.at(i-1)-d.at(i-1))%m;
    if (d.at(i)<0) d.at(i) += m;
  }

  // resultに、とりあえずdの合計を入れておく
  result = accumulate(d.begin(),d.end(),0LL);

  // C問題の場合、先頭に1足した場合は全部の0/1が反転するので、n-result がその場合の結果
  // 2つの値の小さい方が答え
  result = min(result,n-result);

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}