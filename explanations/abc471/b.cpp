#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<string> s(n);
  for (int i=0; i<n; i++) {
    cin >> s.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 回答数を管理するmap
  map<string,int> counters;

  // 全ての回答についてループ
  for (string& str : s) {

    // 全ての文字について、大文字だったら小文字に変更してしまう
    for (char& c : str) {
      if (isupper(c)) c += 'a'-'A';
    }

    // 回答の内容（小文字化したもの）にカウント
    counters[str]++;

  }

  // 改めてmapの中身を全部見て、最大票数のものを探す
  for (auto [key,val] : counters) {
    result = max(result,val);
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}