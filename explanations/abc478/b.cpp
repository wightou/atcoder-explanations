#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, v;
  cin >> n >> v;

  vector<int> w(n);
  for (int i=0; i<n; i++) {
    cin >> w.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 3つの選び方全パターンを全探索
  for (int i=0; i<n; i++) {
    for (int j=i+1; j<n; j++) {
      for (int k=j+1; k<n; k++) {

        // 3つの番号の合計がvより大きかったらスキップ
        // 0-indexedだと、i+j+kが3つ下がっているので、v-3より大きいかどうかで判定
        if (i+j+k>v-3) break;

        // 価値合計が最高記録更新だったら更新
        result = max(result,w.at(i)+w.at(j)+w.at(k));

      }
    }
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}