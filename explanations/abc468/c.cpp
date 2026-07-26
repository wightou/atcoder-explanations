#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// 階乗
vector<long long> factorial;
void make_factorial(int n) {
  assert(n>=0);
  factorial.assign(n+1,1);
  for (int i=1; i<=n; i++) {
    factorial[i] = factorial[i-1]*i;
  }
}

// それより辞書順で小さい順列の個数を数える関数
int calc(const vector<int>& vec) {

  // とりあえずnの値を取得
  int n = ssize(vec);

  // 結果を入れる変数
  int result = 0;

  // 各数が出現済かどうか（[0]はダミーデータ）
  vector<bool> flags(n+1,true);

  // 前から見ていく
  for (int i=0; i<n; i++) {

    // その桁が初めてvecと差異があり、辞書順で前になっている個数を足しこんでいく
    // より前の桁で使用済のものはスキップする
    for (int j=1; j<vec.at(i); j++) {
      if (!flags.at(j)) continue;
      result += factorial.at(n-1-i);
    }

    // 数の使用済データを更新
    flags.at(vec.at(i)) = false;

  }

  // 結果を返す
  return result;

}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> p(n);
  for (int i=0; i<n; i++) {
    cin >> p.at(i);
  }

  vector<int> q(n);
  for (int i=0; i<n; i++) {
    cin >> q.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 階乗のデータ作成
  make_factorial(n);

  // 「qより前の個数」-「pより前の個数」-1（←pそのものの分）が答え
  // ただし、負の数だった場合は0にしておく
  result = max(0,calc(q)-calc(p)-1);

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}