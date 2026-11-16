#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  deque<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }
  deque<int> b(n);
  for (int i=0; i<n; i++) {
    cin >> b.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  string result = "No";

  //////////////////// 処理 ////////////////////

  // 先頭の数が同じである限り、それらを捨てる
  while (!a.empty()&&a.front()==b.front()) {
    a.pop_front();
    b.pop_front();
  }

  // 末尾の数が同じである限り、それらを捨てる
  while (!a.empty()&&a.back()==b.back()) {
    a.pop_back();
    b.pop_back();
  }

  // まず、空になっていたらそもそもOK
  if (a.empty()) {

    result = "Yes";

  } else {

    // 空でなかった場合の確認

    // aの先頭を末尾に回して一致できるか確認
    a.emplace_back(a.front());
    a.pop_front();
    if (a==b) result = "Yes";

    // 戻す
    a.emplace_front(a.back());
    a.pop_back();

    // aの末尾を先頭に回して一致できるか確認
    a.emplace_front(a.back());
    a.pop_back();
    if (a==b) result = "Yes";

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}