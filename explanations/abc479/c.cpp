#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, w;
  cin >> n >> w;

  vector<int> a(n), b(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i) >> b.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // おかずの選び方をbit全探索
  for (int i=0; i<(1<<n); i++) {

    // おいしさの合計
    long long sum_a = 0;

    // 体積の合計
    long long sum_b = 0;

    // 選んだおかずのおいしさと体積を合計
    for (int j=0; j<n; j++) {
      if (i&(1<<j)) {
        sum_a += a.at(j);
        sum_b += b.at(j);
      }
    }

    // もし体積が制限オーバーしていたら打ち切って次へ
    if (sum_b>w) continue;

    // おいしさ最高記録を更新
    result = max(result,sum_a);

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}