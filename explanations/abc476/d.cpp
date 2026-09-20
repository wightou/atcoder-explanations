#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  long long k, x, y;
  cin >> n >> m >> k >> x >> y;


  vector<long long> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }
  vector<long long> b(m);
  for (int i=0; i<m; i++) {
    cin >> b.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // デザートとドリンクそれぞれを安い順に並べる
  sort(a.begin(),a.end());
  sort(b.begin(),b.end());

  // ドリンクだけを安い順に買っていった場合、何番目まで買えるか求める
  // 何番目かは1-indexedにしておく
  long long sum_k = 0;
  int lim = m;
  for (int j=0; j<m; j++) {
    sum_k += (b.at(j)+k-1)/k;
    if (sum_k>y) {
      lim = j;
      break;
    }
  }

  // デザートとドリンクそれぞれ、累積和にしておく
  vector<long long> sum_a(n+1);
  partial_sum(a.begin(),a.end(),sum_a.begin()+1);
  vector<long long> sum_b(m+1);
  partial_sum(b.begin(),b.end(),sum_b.begin()+1);

  // ツーインデックス法で、所持金が足りる範囲で変える個数を調べる
  // ドリンク上限は、先に求めたlimまでとする
  for (int i=n, j=0; j<=lim; j++) {
    while (i>0&&sum_a.at(i)+sum_b.at(j)>x+y*k) i--;
    result = max(result,i+j);
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}