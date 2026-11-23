#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// ライブラリ https://github.com/wightou/library-public

constexpr int INF = 1001001001;

/////////////////// メイン ///////////////////

int main () {

  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> c(n);
  for (int i=0; i<n; i++) {
    cin >> c.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  vector<int> result(n,0);

  //////////////////// 処理 ////////////////////

  // 左側にある色違いで最も近いものの番号（存在しなければ-INF）
  vector<int> l(n,-INF);
  for (int i=1; i<n; i++) {
    if (c.at(i-1)==c.at(i)) l.at(i) = l.at(i-1);
    else l.at(i) = i-1;
  }

  // 右側にある色違いで最も近いものの番号（存在しなければINF）
  vector<int> r(n,INF);
  for (int i=n-2; i>=0; i--) {
    if (c.at(i+1)==c.at(i)) r.at(i) = r.at(i+1);
    else r.at(i) = i+1;
  }

  // それぞれ、左右の最も近いのを比べて、近い距離が答えになる
  for (int i=0; i<n; i++) {
    result.at(i) = min(i-l.at(i),r.at(i)-i);
  }

  //////////////////// 出力 ////////////////////

  for (int i=0; i<ssize(result); i++) {
    cout << result.at(i);
    if (i!=ssize(result)-1) cout << ' ';
  }
  cout << '\n';

  //////////////////// 終了 ////////////////////

  return 0;

}