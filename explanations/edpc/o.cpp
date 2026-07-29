#include <bits/stdc++.h>
using namespace std;

constexpr int mod = 1000000007;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;
  
  vector<vector<int>> a(n,vector<int>(n));
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      cin >> a.at(i).at(j);
    }
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // DPテーブル
  // dp.at(i)は、例えばn=5でi=13なら、二進数で01101なので、
  // 男性の最初3人（0人目1人目2人目）と女性の0人目2人目3人目のマッチングパターン数
  vector<int> dp(1<<n,0);

  // 0人vs0人のマッチングパターンは1通り
  dp.at(0) = 1;

  // bitDPをやる
  for (unsigned int i=1; i<(1<<n); i++) {

    // 番号が最後である男性とどの女性をペアにするかでループ
    for (int j=0; j<n; j++) {

      // その女性がiのビットにいない場合はとばす
      if ((i&(1<<j))==0) continue;

      // 相性が悪い場合もとばす
      // popcount(i)-1で番号が最後である男性の番号
      if (a.at(popcount(i)-1).at(j)==0) continue;

      // その女性とマッチングさせるパターン数を足す
      dp.at(i) += dp.at(i^(1<<j));
      dp.at(i) %= mod;

    }

  }

  // 答えは全男性と全女性のマッチング数
  result = dp.back();


  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}