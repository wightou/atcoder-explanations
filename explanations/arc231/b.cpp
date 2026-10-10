#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////



  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int a, b, c;
    cin >> a >> b >> c;

    //////////////// 出力変数定義 ////////////////
    
    bool flag = true;
    vector<int> result1;
    vector<int> result2;

    //////////////////// 処理 ////////////////////

    // 1024以下の数が作れるかどうかのチェック用配列
    vector<bool> vec(1025,false);

    // Xの方の、絶対に入れなきゃいけない数を入れる
    for (int i=0; i<a; i++) {
      result1.emplace_back(i);
    }

    // Yの方の、絶対に入れなきゃいけない数を入れる
    // Xにある数との組を全探索して、チェック用配列に反映
    for (int j=0; j<b; j++) {
      result2.emplace_back(j);
      for (int i : result1) {
        vec.at(i^j) = true;
      }
    }

    // Xの方に1024を入れて、Yの方に1024から1024+c-1を入れる
    // これにより、Zにc未満が全て作れることを担保できる
    // この1024族と小さい方の値のxorは1024未満にならないのでチェック用配列には反映しない
    result1.emplace_back(1024);
    for (int k=0; k<c; k++) {
      result2.emplace_back(k+1024);
      vec.at(k) = true;
    }

    // mexが本当にcになっているか確認
    int mn = 0;
    while (vec.at(mn)) mn++;
    if (mn!=c) flag = false;

    //////////////////// 出力 ////////////////////

    if (flag) {
      cout << "Yes" << endl;
      cout << ssize(result1) << " ";
      for (size_t i=0; i<result1.size(); i++) {
        cout << result1.at(i);
        if (i!=result1.size()-1) {
          cout << " ";
        }
      }
      cout << endl;
      cout << ssize(result2) << " ";
      for (size_t i=0; i<result2.size(); i++) {
        cout << result2.at(i);
        if (i!=result2.size()-1) {
          cout << " ";
        }
      }
      cout << endl;
    } else {
      cout << "No" << endl;
    } 
  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}