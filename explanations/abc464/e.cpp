#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int h, w, q;
  cin >> h >> w >> q;

  vector<int> r(q), c(q);
  vector<char> x(q);
  for (int i=0; i<q; i++) {
    cin >> r.at(i) >> c.at(i) >> x.at(i);
    r.at(i)--;  // 0-indexed
    c.at(i)--;
  }

  //////////////// 出力変数定義 ////////////////

  // 全てのマスを'A'で埋めて初期化しておく
  vector<string> result(h,string(w,'A'));

  //////////////////// 処理 ////////////////////

  // 各マス最後に書き換えられた時刻を記録する二次元配列を-1で初期化しておく
  vector<vector<int>> vec(h+1,vector<int>(w+1,-1));

  // 時刻が早い方から順に、右下の位置だけ時刻を書き込んでおく
  for (int i=0; i<q; i++) {
    vec.at(r.at(i)).at(c.at(i)) = i;
  }

  // 右下から二次元累積最大値を取ることで、全体が正しく最後に書き換えられた時刻になる
  // ついでに、その時刻を見て、0以上だったらその時刻に対応する文字をresultに書き込んでいく
  for (int i=h-1; i>=0; i--) {
    for (int j=w-1; j>=0; j--) {
      vec.at(i).at(j) = max({vec.at(i).at(j),vec.at(i).at(j+1),vec.at(i+1).at(j)});
      if (vec.at(i).at(j)!=-1) result.at(i).at(j) = x.at(vec.at(i).at(j));
    }
  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}