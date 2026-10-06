#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;
constexpr int INF = 1001001001;

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

  long long result = 1;

  //////////////////// 処理 ////////////////////

  // 単調スタックに番兵を入れておく
  vector<pair<int,int>> vec(1,{INF,0});

  // 2番目以降のある数の親は、「最後に追加した自分より大きい数」以降に追加したものどれでもいい
  // その個数を単調スタックで求めてかけていく
  for (int i=1; i<n; i++) {
    while (vec.back().first<a.at(i)) vec.pop_back();
    result *= i-vec.back().second;
    result %= mod;
    vec.emplace_back(a.at(i),i);
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}