#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  long long d;
  cin >> n >> d;

  //////////////// 出力変数定義 ////////////////

  double result = 0.0;

  //////////////////// 処理 ////////////////////

  // DPテーブル
  // mp[i]は、出目の積とdの最大公約数がiになる確率
  map<long long,double> mp;
  mp[1] = 1.0;

  // サイコロをn回振る
  for (int i=0; i<n; i++) {
  map<long long,double> next;
    for (auto [val,prob] : mp) {
      for (int j=1; j<=6; j++) {
        next[gcd(val*j,d)] += prob/6;
      }
    }
    swap(mp,next);
  }

  // 最大公約数がdであれば、dの倍数ということなので、それが答え
  result = mp[d];

  //////////////////// 出力 ////////////////////

  cout << fixed << setprecision(10) << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}