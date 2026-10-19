#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int n, q;
  cin >> n >> q;

  vector<long long> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  /////////////////// 前処理 ///////////////////

  // aの累積和を「より左」で取る
  vector<long long> sum1(n+1);
  partial_sum(a.begin(),a.end(),sum1.begin()+1);

  // aの累積和の累積和を「より左」で取る（sum1の時点ですでに先頭に0が入っている）
  vector<long long> sum2(n+1);
  partial_sum(sum1.begin(),sum1.end(),sum2.begin());

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    long long l, r;
    cin >> l >> r;
    l--;

    //////////////// 出力変数定義 ////////////////

    long long result = 0;

    //////////////////// 処理 ////////////////////

    // R項目までの総和を出して、結果に足す

    // 第何群までがまるごと含まれるか考え、その範囲の和を足す
    long long tmp_r = ((long long)sqrt(8*r+1)-1)/2;
    result += sum2.at(tmp_r);

    // その次の群の第何項までか考え、その範囲の和を足す
    r -= tmp_r*(tmp_r+1)/2;
    result += sum1.at(r);

    // L-1項目までの総和を出して、結果から引く

    // 第何群までがまるごと含まれるか考え、その範囲の和を引く
    long long tmp_l = ((long long)sqrt(8*l+1)-1)/2;
    result -= sum2.at(tmp_l);

    // その次の群の第何項までか考え、その範囲の和を引く
    l -= tmp_l*(tmp_l+1)/2;
    result -= sum1.at(l);

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}