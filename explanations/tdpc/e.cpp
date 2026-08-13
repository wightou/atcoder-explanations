#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 1000000007;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  string k;
  int d;
  cin >> d >> k;

  int n = k.size();

  // kに1を足す（C++では大変）
  {
    int digit = ssize(k)-1;
    while (digit>=0&&k.at(digit)=='9') {
      k.at(digit) = '0';
      digit--;
    }
    if (digit==-1) {
      k.at(0) = '1';
      k += '0';
      n++;
    } else {
      k.at(digit)++;
    }
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // DPテーブル
  // free[i][j]は、i桁目より後ろ（制約なし）について、和をdで割った余りがjになるものの数
  vector<vector<int>> free(n,vector<int>(d,0));

  // 最下位の桁の値は明らか
  free.at(n-1).at(0) = 1;

  // 下の桁から順にDPを実行
  for (int i=n-1; i>0; i--) {
    for (int r=0; r<d; r++) {
      for (int j=0; j<10; j++) {
        free.at(i-1).at((r+j)%d) += free.at(i).at(r);
        free.at(i-1).at((r+j)%d) %= mod;
      }
    }
  }

  // 注目しているところより上の桁和
  int digit_sum = 0;

  // 上から順に1桁ずつ見ていくループ
  for (int i=0; i<n; i++) {

    // その桁の値より小さい数をループ
    for (int j=0; j<k.at(i)-'0'; j++) {

      // 残りの桁がいくつであればdの倍数にできるかを求める
      int tmp = -(digit_sum+j);
      tmp %= d;
      if (tmp<0) tmp += d;

      // 「その桁より上の桁が全てkと同じで、その桁がkより小さい」個数を結果に足す
      result += free.at(i).at(tmp);
      result %= mod;

    }

    // 次へ行く前に、桁和を更新
    digit_sum += k.at(i)-'0';

  }

  // 0の分を引いておく
  result--;
  if (result<0) result+=mod;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}