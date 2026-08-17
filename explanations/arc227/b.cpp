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

  priority_queue<int,vector<int>,greater<int>> tmp;
  priority_queue<int> que;

  for (int i : a) {
    tmp.emplace(i);
  }
  tmp.emplace(n);

  for (int i=0; i<n; i++) {
    while(tmp.top()==i) {
      que.emplace(i);
      tmp.pop();
    }
    if (que.empty()) {
      result.clear();
      break;
    }
    result.emplace_back(que.top());
    que.pop();
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