#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }
  vector<int> b(n-1);
  for (int i=0; i<n-1; i++) {
    cin >> b.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 先頭に1すら足さない場合の、各値に（M回未満で）何回足すことになるか求める配列
  vector<int> d(n,0);

  // dの偶数番目と奇数番目を分けて入れておくvector
  vector<int> odd(0);
  odd.reserve(n/2+1);
  vector<int> even(1,0);
  even.reserve(n/2+1);

  // dの情報を埋めながら、oddとevenを作成
  for (int i=1; i<n; i++) {
    d.at(i) = (0LL+b.at(i-1)-a.at(i)-a.at(i-1)-d.at(i-1))%m;
    if (d.at(i)<0) d.at(i) += m;
    if (i%2) odd.emplace_back(d.at(i));
    else even.emplace_back(d.at(i));
  }

  // resultに、とりあえずdの合計を入れておく
  result = accumulate(d.begin(),d.end(),0LL);

  // oddとevenをソートしておく
  sort(odd.begin(),odd.end());
  sort(even.begin(),even.end());

  // 節約できる最大値
  long long dec = 0;

  // 先頭をいくつか足す場合について調べる
  // 全部1回以上足してるのは調べる意味がないので、どれか1つを「1回も足さない」にするやつだけ調べる
  // odd側の要素kを0に変えるなら、回数変化は
  // - odd側のk未満の個数だけ +M
  // - even側のm-k以上の個数だけ -M
  // - odd側の個数だけ -k
  // - even側の個数だけ +k
  // になる、これを二分探索で個数を調べる（ツーインデックスでやればもっと高速）
  for (int i=0; i<ssize(odd); i++) {
    long long num = even.end()-lower_bound(even.begin(),even.end(),m-odd.at(i));
    dec = max(dec,(num-i)*m+odd.at(i)*(ssize(odd)-ssize(even)));
  }
  for (int i=0; i<ssize(even); i++) {
    long long num = odd.end()-lower_bound(odd.begin(),odd.end(),m-even.at(i));
    dec = max(dec,(num-i)*m+even.at(i)*(ssize(even)-ssize(odd)));
  }

  // 節約できる最大値を実際に引けば答え
  result -= dec;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}