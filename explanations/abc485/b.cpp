#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// ライブラリ https://github.com/wightou/library-public

/////////////////// メイン ///////////////////

int main () {

  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;

  vector<string> s(n);
  for (int i=0; i<n; i++) {
    cin >> s.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  string result1 = "";
  int result2 = 0;

  //////////////////// 処理 ////////////////////

  // 各蛇口が正常である保証が取れたかどうか
  vector<bool> faucet(n,false);

  // 各排水管が正常である保証が取れたかどうか
  vector<bool> drain(m,false);

  // 各パターン、oだったらそのときの蛇口と排水管に正常と記録
  for (int i=0; i<n; i++) {
    for (int j=0; j<m; j++) {
      if (s.at(i).at(j)=='o') {
        faucet.at(i) = true;
        drain.at(j) = true;
      }
    }
  }

  // 各蛇口、各排水管を見て、正常確認が取れていないのを出力
  // 制約から、該当するものは1つになっているはず
  for (int i=0; i<n; i++) {
    if (!faucet.at(i)) {
      result1 = "Faucet";
      result2 = i+1;
    }
  }
  for (int j=0; j<m; j++) {
    if (!drain.at(j)) {
      result1 = "Drain";
      result2 = j+1;
    }
  }

  //////////////////// 出力 ////////////////////

  cout << result1 << " " << result2 << '\n';

  //////////////////// 終了 ////////////////////

  return 0;

}