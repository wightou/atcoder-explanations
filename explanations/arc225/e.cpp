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
    a.at(i)--;
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 左へ進みたいものについて、区間 [a.at(i),i] の交錯数を求める
  fenwick_tree<int> fl(n);
  for (int i=0; i<n; i++) {
    if (a.at(i)<i) {

      // 狭義の交錯数の計算
      result += fl.sum(a.at(i)+1,i);
      fl.add(i,1);
      fl.add(a.at(i),-1);

      // Lが自身の目的地に乗る分
      result++;

    }
  }

  // 右へ進みたいものについて、区間 [i,a.at(i)] の交錯数を求める
  fenwick_tree<int> fr(n);
  for (int i=n-1; i>=0; i--) {
    if (a.at(i)>i) {

      // 狭義の交錯数の計算
      result += fr.sum(i+1,a.at(i));
      fr.add(i,1);
      fr.add(a.at(i),-1);

      // 最初からRが別のRの目的地に乗っている分
      if (a.at(a.at(i))>a.at(i)) result++;

    }
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}