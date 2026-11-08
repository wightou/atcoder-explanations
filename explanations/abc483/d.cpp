#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 各行動から今後通常通り動作した場合の、相対位置の最大値と最小値
  vector<long long> mx_r(n+1,0);
  vector<long long> mn_r(n+1,0);
  for (int i=n-1; i>=0; i--) {
    mx_r.at(i) = max(0LL,a.at(i)+mx_r.at(i+1));
    mn_r.at(i) = min(0LL,a.at(i)+mn_r.at(i+1));
  }

  // 何も反転しない場合の座標数を入れておく
  result = mx_r.at(0)-mn_r.at(0)+1;

  // シミュレーション用の、現在座標、過去の最大座標、過去の最小座標
  long long pos = 0;
  long long mx_l = 0;
  long long mn_l = 0;

  // 前から順に、「そこを反転した場合の、全体での最大座標と最小座標」を求め、最大値を探す
  for (int i=0; i<n; i++) {
    long long mx = max(mx_l,pos-a.at(i)+mx_r.at(i+1));
    long long mn = min(mn_l,pos-a.at(i)+mn_r.at(i+1));
    result = max(result,mx-mn+1);
    pos += a.at(i);
    mx_l = max(mx_l,pos);
    mn_l = min(mn_l,pos);
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}