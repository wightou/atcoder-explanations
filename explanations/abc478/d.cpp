#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, q;
  cin >> n >> q;

  vector<int> l(q), r(q), x(q);
  for (int i=0; i<q; i++) {
    cin >> l.at(i) >> r.at(i) >> x.at(i);
    l.at(i)--;
    r.at(i)--;
    x.at(i)--;
  }

  //////////////// 出力変数定義 ////////////////

  vector<int> result(n+1);

  //////////////////// 処理 ////////////////////

  // xの値ごとに、範囲を集計する
  // 「左端、右端」で小さい順にソートしておく
  vector<priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>> vec(q);
  for (int i=0; i<q; i++) {
    vec.at(x.at(i)).emplace(l.at(i),r.at(i));
  }

  // 階差数列を用意
  vector<int> tmp(n+1,0);

  // xの値ごとに処理するループ
  for (int i=0; i<q; i++) {

    // その値を入れる範囲を全て調査
    while (!vec.at(i).empty()) {

      // queから範囲を取り出す
      // 次の範囲とつながっている場合は合体する
      auto [left,right] = vec.at(i).top();
      vec.at(i).pop();
      while (!vec.at(i).empty()&&vec.at(i).top().first<=right+1) { 
        right = max(right,vec.at(i).top().second);
        vec.at(i).pop();
      }

      // 区間加算する
      tmp.at(left)++;
      tmp.at(right+1)--;

    }

  }

  // 階差数列から本来の数列を復元し、作業用に用意していた余計な末尾を削除する
  partial_sum(tmp.begin(),tmp.end(),result.begin());
  result.pop_back();

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i);
    if (i!=result.size()-1) {
      cout << " ";
    }
  }
  cout << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}