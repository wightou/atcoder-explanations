#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, s;
  long long l;
  cin >> n >> s >> l;
  s--;

  vector<long long> a(n-1);
  for (int i=0; i<n-1; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 累積和で、各都市の位置を出しておく
  vector<long long> sum(n);
  partial_sum(a.begin(),a.end(),sum.begin()+1);

  // 尺取法で、左に行ってから右に行くパターンを出す
  // sから左に行って、tでターンして、gまで行く
  for (int t=s,g=n-1; t>=0; t--) {

    // 左へ行くだけでオーバーの場合、ゴールがスタートを超えられない場合は探索終了
    if (sum.at(s)-sum.at(t)>l||g<s) break;

    // ゴールまでの距離がオーバーしている場合、ゴールを近づける
    while (sum.at(s)+sum.at(g)-2*sum.at(t)>l) g--;

    // 到達地点数の最大値記録を更新
    result = max(result,max(s,g)-t+1);

  }

  // 尺取法で、右に行ってから左に行くパターンを出す（左右逆なだけ）
  // sから右に行って、tでターンして、gまで行く
  for (int t=s,g=0; t<n; t++) {
    if (sum.at(t)-sum.at(s)>l) break;
    while (2*sum.at(t)-sum.at(s)-sum.at(g)>l) g++;
    result = max(result,t-min(s,g)+1);
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}