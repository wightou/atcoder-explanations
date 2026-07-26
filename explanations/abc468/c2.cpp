#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> p(n);
  for (int i=0; i<n; i++) {
    cin >> p.at(i);
  }

  vector<int> q(n);
  for (int i=0; i<n; i++) {
    cin >> q.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // pスタートで、q以上になるまで順列全探索
  // 進めた回数-1が答えなのと、答えが0になるパターンを、処理順で工夫することで対応
  while (next_permutation(p.begin(), p.end())) {
    if (p>=q) break;
    result++;
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}