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

  vector<int> result(3);

  //////////////////// 処理 ////////////////////

  // 1回ごとに買い物額を見るループ
  for (int i : a) {

    // とりあえず、1000円以上分は普通に払ってしまう
    i %= 1000;

    // ちょうどだったら処理は終わりなので次へ
    if (i==0) continue;

    // おつりの額にする
    i = 1000-i;

    // 一の位、十の位、百の位、それぞれを取り出して、該当変数に加える
    result.at(0) += i%10;
    i /= 10;
    result.at(1) += i%10;
    i /= 10;
    result.at(2) += i%10;

  }

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