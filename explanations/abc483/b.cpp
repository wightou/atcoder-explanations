#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, q;
  cin >> n >> q;

  vector<int> a(q);
  for (int i=0; i<q; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  // n（下）から1（上）まで降順に入った配列として初期化する
  vector<int> result(n);
  iota(result.begin(),result.end(),1);
  reverse(result.begin(),result.end());

  //////////////////// 処理 ////////////////////

  // k枚ひっくり返すことを繰り返す
  for (int k : a) {

    // 持ち上げたものを逆順に、かつ表裏をひっくり返しながら上からカードを取る
    vector<int> tmp;
    for (int i=0; i<k; i++) {
      tmp.emplace_back(-result.back());
      result.pop_back();
    }

    // 取ったものをそのまま上に戻す
    for (int i : tmp) {
      result.emplace_back(i);
    }

  }

  // 出力用に上下を逆にする
  reverse(result.begin(),result.end());

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}