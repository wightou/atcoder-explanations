#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// 後ろから順に、再帰の深さ優先探索で（貪欲法まじりの）ナップサック問題を解く
// 再帰1回は、容量w以下を荷物番号id以下で埋めるときの最大価値を求める
long long calc (const vector<pair<long long,long long>>& vec, long long w, int id) {

  // 1つも選べない場合の最大価値は0
  if (id==-1) return 0;

  // 結果を入れるための変数
  long long result = 0;

  // もし、残り許容量が次の荷物の2倍-1より少ないときだけ、その荷物を見送る選択がある（貪欲法）
  if (vec.at(id).first*2-1>w) result = max(result,calc(vec,w,id-1));

  // もし、残り許容量が次の荷物以上ならば、その荷物を選ぶ選択がある
  if (vec.at(id).first<=w) result = max(result,calc(vec,w-vec.at(id).first,id-1)+vec.at(id).second);

  return result;

}

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////



  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int n;
    long long w;
    cin >> n >> w;

    vector<pair<long long,long long>> v(n);
    for (int i=0; i<n; i++) {
      cin >> v.at(i).first >> v.at(i).second;
    }

    //////////////// 出力変数定義 ////////////////

    long long result = 0;

    //////////////////// 処理 ////////////////////

    // 深さ優先探索で求める
    result = calc(v,w,n-1);

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}