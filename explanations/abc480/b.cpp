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
  vector<string> t(m);
  for (int i=0; i<m; i++) {
    cin >> t.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  vector<int> result(n);

  //////////////////// 処理 ////////////////////

  // 名前ごとに、得票数を管理する
  // とりあえず全員0で初期化する
  map<string,int> vote;
  for (string& str : s) {
    vote[str] = 0;
  }

  // 投票で書かれた名前を全探索
  for (string& name1 : t) {

    // きっちり同じ名前の人がいるかどうか
    bool hit = false;

    // 1文字違いの候補一覧
    vector<string> candidates;

    // 立候補者を全探索
    for (string& name2 : s) {

      // 文字数が違う人だったら何もしない
      if (ssize(name1)!=ssize(name2)) continue;

      // 差異がある文字数を数える
      int counter = 0;
      for (int i=0; i<ssize(name1); i++) {
        if (name1.at(i)!=name2.at(i)) counter++;
      }

      // 差異が0だったら、hitをtrueにして脱出
      // 差異が1だったら、1文字違いの人に登録して続ける
      if (counter==0) {
        hit = true;
        break;
      }
      else if (counter==1) candidates.emplace_back(name2);

    }

    // 完全一致の人がいたらその人に1票
    // 完全一致がいなくて1文字違いが1人でもその人に1票
    if (hit) vote.at(name1)++;
    else if (ssize(candidates)==1) vote.at(candidates.at(0))++;

  }

  // 名前ごとのデータを見て、出力すべき順に整理する
  for (int i=0; i<n; i++) {
    result.at(i) = vote.at(s.at(i));
  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}