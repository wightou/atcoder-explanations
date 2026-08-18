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

  vector<int> result;

  //////////////////// 処理 ////////////////////

  // 全ての数をカウンティングする
  vector<int> counters(n,0);
  for (int i : a) {
    counters.at(i)++;
  }
  
  // i以下の未使用の数を管理する stack（のつもりの deque）
  deque<int> que;

  // 前から順に値を決めていく
  for (int i=0; i<n; i++) {

    // i という値があれば、que に全部入れる
    for (int j=0; j<counters.at(i); j++) {
      que.emplace_back(i);
    }

    // i 以下の未使用の数がなければ構築不可能
    if (que.empty()) {
      result.clear();
      break;
    }

    // i 以下の未使用の数で最大のものを採用する
    result.emplace_back(que.back());
    que.pop_back();

  }

  //////////////////// 出力 ////////////////////

  if (result.empty()) {
    cout << "No" << endl;
  } else {
    cout << "Yes" << endl;
    for (size_t i=0; i<result.size(); i++) {
      cout << result.at(i);
      if (i!=result.size()-1) {
        cout << " ";
      }
    }
    cout << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}