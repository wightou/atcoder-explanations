#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n = 5;

  vector<string> s(n);
  for (int i=0; i<n; i++) {
    cin >> s.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 5;

  //////////////////// 処理 ////////////////////

  // 上からi行目を見るループ
  for (int i=0; i<n; i++) {

    // カウンターを用意して、その行に何個 '.' があるか数える
    int counter = 0;
    for (int j=0; j<n; j++) {
      if (s.at(i).at(j)=='.') counter++;
    }

    // 最小記録を更新
    result = min(result,counter);

  }

  // 左からi列目を見るループ
  for (int i=0; i<n; i++) {
    int counter = 0;
    for (int j=0; j<n; j++) {
      if (s.at(j).at(i)=='.') counter++;
    }
    result = min(result,counter);
  }
  
  // 左上から右下への斜めを見る
  {
    int counter = 0;
    for (int j=0; j<n; j++) {
      if (s.at(j).at(j)=='.') counter++;
    }
    result = min(result,counter);
  }

  // 右上から左下への斜めを見る
  {
    int counter = 0;
    for (int j=0; j<n; j++) {
      if (s.at(j).at(n-1-j)=='.') counter++;
    }
    result = min(result,counter);
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}