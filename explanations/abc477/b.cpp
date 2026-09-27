#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, d;
  cin >> n >> d;

  vector<int> x(n);
  for (int i=0; i<n; i++) {
    cin >> x.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  vector<int> result;

  //////////////////// 処理 ////////////////////

  // 番号が若い順に全員調査
  for (int i=0; i<n; i++) {

    // ちゃんと「一線を画す」になっているか
    bool flag = true;

    // 自分以外を全員、相手として調査
    for (int j=0; j<n; j++) {

      // 自分はスキップ
      if (i==j) continue;

      // 相手までの距離がd未満だったらfalseにする
      if (abs(x.at(i)-x.at(j))<d) flag = false;

    }

    // flagがtrueのままだったら、結果一覧に追加する
    if (flag) result.emplace_back(i+1);

  }

  //////////////////// 出力 ////////////////////

  cout << ssize(result) << endl;
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