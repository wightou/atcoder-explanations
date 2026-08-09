#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> s(n), t(n);
  for (int i=0; i<n; i++) {
    cin >> s.at(i) >> t.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 1;

  //////////////////// 処理 ////////////////////

  // (s,t) をsが早い順にソート
  vector<pair<int,int>> vec;
  for (int i=0; i<n; i++) {
    vec.emplace_back(s.at(i),t.at(i));
  }
  sort(vec.begin(),vec.end());

  // 2人の拘束終了時刻
  // -1 なら現在フリー
  int a = -1;
  int b = -1;

  // sが早い順に処理する
  for (auto [l,r] : vec) {

    // すでに仕事が終わっていたらフリー状態に戻す
    if (a<l) a = -1;
    if (b<l) b = -1;

    // フリーな人の数を数える
    int count = 0;
    if (a==-1) count++;
    if (b==-1) count++;

    // 割り当て可能人数を結果にかける
    result *= count;
    result %= mod;

    // 既に実現不可能なら、これ以上計算しても無意味なのでループを終了する
    if (result==0) break;

    // aが暇ならaに、そうでなければbに割り当てる
    if (a==-1) a = r;
    else b = r;

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}