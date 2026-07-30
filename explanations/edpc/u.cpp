#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

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

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // bitで表された集合を、全員同じグループにする場合の得点を求める
  vector<long long> dp_full(1<<n,0);
  for (int b=0; b<n; b++) {
    for (int j=0; j<(1<<b); j++) {
      int i = j|(1<<b);
      dp_full.at(i) = dp_full.at(j);
      for (int k=0; k<b; k++) {
        if (j&(1<<k)) dp_full.at(i) += a.at(b).at(k);
      }
    }
  }

  // bitで表された集合を、任意個数のグループにする場合の得点を求める
  // kの更新を工夫することで、O(4^N)からO(3^N)に高速化
  vector<long long> dp_part(1<<n,0);
  for (int b=0; b<n; b++) {
    for (int j=0; j<(1<<b); j++) {
      int i = j|(1<<b);
      dp_part.at(i) = dp_full.at(i);
      for (int k=j; k>0; k=((k-1)&j)) {  // ここのkの更新がカギ
        dp_part.at(i) = max(dp_part.at(i),dp_full.at(i&(~k))+dp_part.at(k));
      }
    }
  }
  
  // DPテーブル最後尾が答え
  result = dp_part.back();

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}