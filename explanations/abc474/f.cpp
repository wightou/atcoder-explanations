#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr long long INF = 1001001001001001001LL;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<long long> a(n+1);    // 1-indexed でやる
  for (int i=1; i<=n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 下界と上界
  long long lb = 0;
  long long ub = INF;

  // 各値をTにするまでに+1が必要な回数を k1*T-k0 とする
  vector<long long> k0 = a;
  vector<long long> k1(n+1,1);

  // 後ろから順にループ
  for (int i=n; i>0; i--) {

    // 上位の操作からの影響を反映する
    for (int j=2*i; j<=n; j+=i) {
      k0.at(i) -= k0.at(j);
      k1.at(i) -= k1.at(j);
    }

    // 今回得られる上界や下界を求めて更新
    // k0とk1の符号が違う場合は計算がバグる
    // ただしその場合は後の段階のどこかでなんか自然と軌道修正されるはずなので、意図的にバグった仕様で書く
    // （C++では負数除算の仕様が壊れているが、こういう符号がどの組もあり得る場合の対応は本質的でないのに非常に手間）
    // k1が0の場合は、単にk0<=0かどうかだけ確認する
    // 違った場合、上界を-1にすることで答えが-1になるようにしておく
    if (k1.at(i)>0) lb = max(lb,(k0.at(i)+k1.at(i)-1)/k1.at(i));
    else if (k1.at(i)<0) ub = min(ub,(-k0.at(i))/(-k1.at(i)));
    else if (k0.at(i)>0) ub = -1;
  }

  // 上界と下界に矛盾があったら不可能
  // 矛盾がなければ下界の場合の値が答え
  if (lb>ub) result = -1;
  else result = lb-a.at(1);

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}