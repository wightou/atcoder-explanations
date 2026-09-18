#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m, l;
  cin >> n >> m >> l;

  //////////////// 出力変数定義 ////////////////

  vector<int> result(m-l+1);

  //////////////////// 処理 ////////////////////

  // 母関数をとりあえず1で初期化
  vector<long long> a(n+1,0);
  a.at(0) = 1;
  
  // 1円硬貨から順に処理するループ
  for (int i=1; i<=m; i++) {

    // 1+x^i+x^(2i)+x^(3i)+…… を掛ける
    // NTTは逆に遅くなるので、無制限ナップサック問題の手法でO(N)でやる
    for (int j=i; j<=n; j++) {
      a.at(j) += a.at(j-i);
      a.at(j) %= mod;
    }

    // l-1円硬貨までは、掛けて終わり
    if (i<l) continue;

    // 結果を入れる、負の数対応もする
    result.at(i-l) = (a.at(n)+mod)%mod;

    // 1+x^(i-l+1)+x^(2(i-l+1))+x^(3(i-l+1))+…… で割る
    // 1-x^(i-l+1) を掛ければよい
    // NTTは逆に遅くなるので、0/1ナップサック問題の手法でO(N)でやる
    for (int j=n; j>=i-l+1; j--) {
      a.at(j) -= a.at(j-(i-l+1));
      a.at(j) %= mod;
    }

  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}