#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 1000000007;

// 行列積
vector<vector<long long>> prod (vector<vector<long long>> x, vector<vector<long long>> y) {
  int n1 = x.size();
  int n2 = x.at(0).size();
  assert((int)y.size() == n2);
  int n3 = y.at(0).size();
  vector<vector<long long>> z(n1,vector<long long>(n3,0));
  for (int i=0; i<n1; i++) {
    for (int j=0; j<n3; j++) {
      for (int k=0; k<n2; k++) {
        z.at(i).at(j) += x.at(i).at(k)*y.at(k).at(j);
        z.at(i).at(j) %= mod;
      }
    }
  }
  return z;
}

// n次の単位行列
vector<vector<long long>> e(int n) {
  vector<vector<long long>> result(n,vector<long long>(n,0));
  for (int i=0; i<n; i++) {
    result.at(i).at(i) = 1;
  }
  return result;
}

// 正方行列の累乗
vector<vector<long long>> doubling(vector<vector<long long>> a, long long p) {
  if (p==0) {
    return e(a.size());
  } else if (p%2==0) {;
    vector<vector<long long>> tmp = doubling(a,p/2);
    return prod(tmp,tmp);
  } else {
    vector<vector<long long>> tmp = doubling(a,p-1);
    return prod(tmp,a);
  }
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  long long k;
  cin >> n >> k;

  vector<vector<long long>> a(n,vector<long long>(n));
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      cin >> a.at(i).at(j);
    }
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // aのk乗を計算する
  auto mat = doubling(a,k);

  // すべての成分の和が答え
  for (auto vec : mat) {
    result += accumulate(vec.begin(),vec.end(),0LL);
  }
  result %= mod;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}