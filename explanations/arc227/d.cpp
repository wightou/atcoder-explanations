#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// 「2つの位置と内容」をユニークな整数に対応付ける関数
int encode(const string& s, int i, int j) {
  return i*(i-1)*2+j*4+2*(s.at(i)=='1')+(s.at(j)=='1');
}

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int n, m, q;
  cin >> n >> m >> q;

  vector<string> s(n);
  for (int i=0; i<n; i++) {
    cin >> s.at(i);
  }

  /////////////////// 前処理 ///////////////////

  // 「2つの位置と内容」からユニークな整数に対応付けて、
  // それがsのどこかにいたかどうかを記録しておく配列
  vector<bool> checker(m*(m-1)*2);

  // sの全ての文字列内の全てを調査し、全ての「2つの位置と内容」について有無を調べる  
  for (const string& str : s) {
    for (int i=0; i<m; i++) {
      for (int j=0; j<i; j++) {
        checker.at(encode(str,i,j)) = true;
      }
    }
  }

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    string t;
    cin >> t;

    //////////////// 出力変数定義 ////////////////

    string result = "No";

    //////////////////// 処理 ////////////////////
    
    // 1文字しかない場合はコーナーケースとして愚直処理
    if (m<2) {

      // 新しい文字列ができる余地がないので、そのものが存在するときだけYes
      for (int i=0; i<n; i++) {
        if (s.at(i)==t) {
          result = "Yes";
          break;
        }
      }

    } else {

      // その文字列を作るのに必要な全ての「2つの位置と内容」がそろっているか確認
      bool flag = true;
      for (int i=0; i<m; i++) {
        for (int j=0; j<i; j++) {
          if(!checker.at(encode(t,i,j))) {
            flag = false;
            break;
          }
        }
        if (!flag) break;
      }

      // 全部そろっているならYes
      if (flag) result = "Yes";

    }

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}