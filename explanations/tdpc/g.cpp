#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  string s;
  long long k;
  cin >> s >> k;

  int n = ssize(s);

  //////////////// 出力変数定義 ////////////////

  string result = "";

  //////////////////// 処理 ////////////////////

  // 各文字の登場位置の逆順
  vector<vector<int>> pos(26);

  // 各位置の文字から始まる文字列を何パターン作れるか
  vector<long long> dp(n,0);

  // 現状作れる文字列数、最初は空文字の1つだけ
  long long sum = 1;

  // 後ろから1文字ずつ追加する
  for (int i=n-1; i>=0; i--) {

    // その文字開始で作れるのは、その後ろの任意文字列数と同じだけ
    dp.at(i) = sum;

    // その文字が何文字目のアルファベットか
    int c = s.at(i)-'a';

    // 差分更新で現状作れる文字列数を更新
    // kを超える値を管理する意味はないので、オーバーフロー対策で全部k+1にしてしまう
    if (!pos.at(c).empty()) sum -= dp.at(pos.at(c).back());
    sum += dp.at(i);
    if (sum>k) sum = k+1;


    pos.at(c).emplace_back(i);

  }

  // 作れる文字列数が足りない場合、Eelと出力して早期終了
  if (sum-1<k) {
    cout << "Eel" << endl;
    return 0;
  }

  // 残り作らなきゃいけない文字列数
  long long count = k;

  // 必要数作るまでループ
  while (count>0) {

    // アルファベット順に全部見る
    for (int c=0; c<26; c++) {

      // 空だったらスキップ
      if (pos.at(c).empty()) continue;

      // そのアルファベットで始まるものを全部追加しても足りないなら、残り個数を減らして次へ
      if (count-dp.at(pos.at(c).back())>0) {
        count -= dp.at(pos.at(c).back());
        continue;
      }

      // 次の採用文字の位置
      int p = pos.at(c).back();

      // 採用文字を結果に追加
      result += s.at(p);

      // その文字で終わる分を引く
      count -= 1;

      // すべての位置情報から、p以前を削除
      for (vector<int>& vec : pos) {
        while(!vec.empty()&&vec.back()<=p) vec.pop_back(); 
      } 

      // 改めてaから再調査
      break;

    }

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}