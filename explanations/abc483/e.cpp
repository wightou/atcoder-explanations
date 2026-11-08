#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;

  vector<string> s(n);
  for (int i=0; i<n; i++) {
    cin >> s.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 作業用map
  unordered_map<string,int> mp;

  // 0か所を'.'で潰したときの同一文字列ペア数を数える
  long long a0 = 0;
  {
    for (string str : s) {
      a0 += mp[str];
      mp[str]++;
    }
    mp.clear();
  }

  // 1か所を'.'で潰したときの同一文字列ペア数を数える
  long long a1 = 0;
  for (int i=0; i<m; i++) {
    for (string str : s) {
      str.at(i) = '.';
      a1 += mp[str];
      mp[str]++;
    }
    mp.clear();
  }

  // 2か所を'.'で潰したときの同一文字列ペア数を数える
  long long a2 = 0;
  for (int i=0; i<m; i++) {
    for (int j=i+1; j<m; j++) {
      for (string str : s) {
        str.at(i) = '.';
        str.at(j) = '.';
        a2 += mp[str];
        mp[str]++;
      }
      mp.clear();
    }
  }

  // 1ヶ所違うペアの個数を求める
  // 0か所のペアがm回ずつ誤ってカウントされているのを引く
  a1 -= a0*m;

  // 2ヶ所違うペアの個数を求める
  // 1か所のペアがm-1回ずつ、0か所のペアがm*(m-1)/2回ずつ誤ってカウントされているのを引く
  a2 -= a0*m*(m-1)/2+a1*(m-1);

  // 0ヶ所、1ヶ所、2ヶ所の合計が答え
  result = a0+a1+a2;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}