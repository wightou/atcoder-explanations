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

  vector<pair<int,int>> vec;

  // デザートとドリンクを混合で「値段、kドル札要求枚数」で昇順ソート
  for (int i : a) vec.emplace_back(i,0);
  for (int i : b) vec.emplace_back(i,(i+k-1)/k);
  sort(vec.begin(),vec.end());

  // 所持金と、kドル札の枚数
  long long money = x+y*k;
  long long num_k = y;

  // 安い順に、（デザート支払いは後回しにする前提で）買えるなら買う
  for (auto [cost,need_k] : vec) {
    if (money>=cost&&num_k>=need_k) {
      result++;
      money -= cost;
      num_k -= need_k;
    }
  }
  

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}