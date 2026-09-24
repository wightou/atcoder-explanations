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
  vector<int> b(n);
  for (int i=0; i<n; i++) {
    cin >> b.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 斜めライン＼上の総和
  vector<long long> down(2*n-1,0);

  // 斜めライン／上の総和
  vector<long long> up(2*n-1,0);

  // 各マスの人口を求めて、上の総和に足しこむ
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      long long c = 1LL*a.at(i)*b.at(j)%m;
      down.at(i-j+n-1) += c;
      up.at(i+j) += c;
    }
  }

  // 累積和（down版）
  vector<long long> sum0_down(2*n-1);
  partial_sum(down.begin(),down.end(),sum0_down.begin());

  // 距離の絶対値をかけての総和
  vector<long long> sum1_down(2*n-1);
  sum1_down.at(2*n-2) = accumulate(sum0_down.begin(),sum0_down.end()-1,0LL); 
  for (int i=2*n-3; i>=0; i--) {
    sum1_down.at(i) = sum1_down.at(i+1)+sum0_down.at(2*n-2)-2*sum0_down.at(i);
  }

  // 累積和（up版）
  vector<long long> sum0_up(2*n-1);
  partial_sum(up.begin(),up.end(),sum0_up.begin());

  // 距離の絶対値をかけての総和
  vector<long long> sum1_up(2*n-1);
  sum1_up.at(2*n-2) = accumulate(sum0_up.begin(),sum0_up.end()-1,0LL); 
  for (int i=2*n-3; i>=0; i--) {
    sum1_up.at(i) = sum1_up.at(i+1)+sum0_up.at(2*n-2)-2*sum0_up.at(i);
  }

  // 指示通りに、xorを取る
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      result ^= (sum1_down.at(i-j+n-1)+sum1_up.at(i+j))/2+i*n+j;
    }
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}