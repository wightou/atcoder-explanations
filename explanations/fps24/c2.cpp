#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// 繰り返し二乗法によるダブリング
// 畳み込みして、x^limより先を切る演算での累乗
vector<long long> power(vector<long long> a, long long p, int lim) {
  assert(p>=0);
  vector<long long> result = {1};
  vector<long long> b = a;
  while (p>0) {
    if (p&1LL) result = convolution(result,b);
    if (ssize(result)>lim+1) result.resize(lim+1); 
    p >>= 1;
    if (p>0) b = convolution(b,b);
    if (ssize(b)>lim+1) b.resize(lim+1); 
  }
  return result;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m, s;
  cin >> n >> m >> s;

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 0からmまでから1つ選ぶ行為の母関数
  vector<long long> a(m+1,1);

  // n乗し、長さが足りなければ補充
  vector<long long> a_dth = power(a,n,s);
  a_dth.resize(s+1,0);

  // x^sの係数が答え
  result = a_dth.at(s);

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}