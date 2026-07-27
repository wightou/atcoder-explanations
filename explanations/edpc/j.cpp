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

  double result = 0.0;

  //////////////////// 処理 ////////////////////

  // 寿司が1個/2個/3個の皿の数を数える
  vector<int> counts(4,0);
  for (int i : a) {
    counts.at(i)++;
  }
  int& n1 = counts.at(1);
  int& n2 = counts.at(2);
  int& n3 = counts.at(3);
  
  // DPテーブル
  // 中身のサイズが固定でないので、とりあえず「3貫ある皿の枚数ごと」だけサイズを決めておく
  vector<vector<vector<double>>> dp(n3+1);

  // 3貫ある皿の枚数が少ない方からループ
  for (int i3=0; i3<=n3; i3++) {

    // 3貫ある皿がi3枚だと、2貫の皿は最大でn3+n2-i3枚ある可能性がある
    dp.at(i3).resize(n3+n2-i3+1);

    // 2貫ある皿の枚数が少ない方からループ
    for (int i2=0; i2<=n3+n2-i3; i2++) {
      
      // 3貫ある皿がi3枚、2貫ある皿がi2枚だと、1貫の皿は最大でn3+n2+n1-i3-i2枚ある可能性がある
      dp.at(i3).at(i2).assign(n3+n2+n1-i3-i2+1,0.0);

      // 1貫ある皿の枚数が少ない方からループ
      for (int i1=0; i1<=n3+n2+n1-i3-i2; i1++) {

        // 全部0のときは、既に終わっているので期待値0のまま
        // ゼロ除算の回避のため、処理をスキップする
        if (i1==0&&i2==0&&i3==0) continue;

        // 1貫ある皿がi1枚、2貫ある皿がi2枚、3貫ある皿がi3枚である場合の期待値を既定の式で計算
        dp.at(i3).at(i2).at(i1) += 1.0*n;
        if (i1>0) dp.at(i3).at(i2).at(i1) += dp.at(i3).at(i2).at(i1-1)*i1;
        if (i2>0) dp.at(i3).at(i2).at(i1) += dp.at(i3).at(i2-1).at(i1+1)*i2;
        if (i3>0) dp.at(i3).at(i2).at(i1) += dp.at(i3-1).at(i2+1).at(i1)*i3;
        dp.at(i3).at(i2).at(i1) /= (i1+i2+i3);

      }

    }

  }

  // 1貫ある皿がn1枚、2貫ある皿がn2枚、3貫ある皿がn3枚である場合の期待値
  result = dp.at(n3).at(n2).at(n1);

  //////////////////// 出力 ////////////////////

  cout << fixed << setprecision(10) << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}