#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;

  vector<int> r(m), c(m);
  for (int i=0; i<m; i++) {
    cin >> r.at(i) >> c.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 今コマがある場所を、行ごとに「そこの何列目にあるか」を記録
  vector<int> vec_r(n+1,-1);

  // 今コマがある場所を、列ごとに「そこの何行目にあるか」記録
  vector<int> vec_c(n+1,-1);

  // 操作を1つずつ行う
  for (int i=0; i<m; i++) {

    // r行目にあるものの列番号を確認し、対応する列のデータから削除→r行目も削除
    if (vec_r.at(r.at(i))!=-1) {
      vec_c.at(vec_r.at(r.at(i))) = -1;
      vec_r.at(r.at(i)) = -1;
    }

    // c列目にあるものの行番号を確認し、対応する行のデータから削除→c列目も削除
    if (vec_c.at(c.at(i))!=-1) {
      vec_r.at(vec_c.at(c.at(i))) = -1;
      vec_c.at(c.at(i)) = -1;
    }

    // 行と列それぞれ、新たに置くコマを記録
    vec_c.at(c.at(i)) = r.at(i);
    vec_r.at(r.at(i)) = c.at(i);

  }

  // 行か列かどちらかを見て、全体にあるコマ数を求める
  for (int i : vec_r) {
    if (i!=-1) result++;
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}