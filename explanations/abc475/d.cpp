#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// 同じ文字の位置関係を検出する関数
// 文字列を受け取って、同じ位置をまとめてbit計算して返す
vector<int> same_letters(const string& s) {

  // mapを用意
  map<char,int> mp;

  // その文字がi文字目にいることを2^iという値に対応させて、合計を取る
  for (int i=0; i<ssize(s); i++) mp[s.at(i)] += (1<<i);

  // キーを捨てて値だけ取り出し、ソート
  vector<int> vec;
  for (auto [c,i] : mp) vec.emplace_back(i);
  sort(vec.begin(),vec.end());

  return vec;

}

// エラトステネスの篩
vector<bool> eratosthenes (int n) {
  assert(n>=0);
  vector<bool> flags(n+1,true);
  flags[0] = false;
  if (n>0) flags[1] = false;
  for (int i=2; i*i<=n; i++) {
    if (!flags[i]) continue;
    for (int j=i*i; j<=n; j+=i) {
      flags[j] = false;
    }
  }
  return flags;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  string s;
  cin >> s;

  int n = ssize(s);

  //////////////// 出力変数定義 ////////////////

  int result = -1;

  //////////////////// 処理 ////////////////////

  vector<int> vec = same_letters(s);

  // 調査すべき範囲を調べる
  int d = 1;
  for (int i=0; i<n; i++) {
    d *= 10;
  }

  // エラトステネスの篩で、d以下の素数を全て洗い出す
  vector<bool> primes = eratosthenes(d);
  
  // 同じ長さの文字列になる範囲の素数全て、同じ文字の位置関係がsと一致していないか調べる
  for (int i=d/10; i<d; i++) {
    if (!primes.at(i)) continue;
    if (same_letters(to_string(i))==vec) {
      result = i;
      break;
    }
  }
  

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}