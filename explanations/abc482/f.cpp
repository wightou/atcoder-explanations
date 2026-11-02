#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// 素因数分解
vector<pair<long long,int>> prime_factorization(long long n) {
  assert(n>0);
  vector<pair<long long,int>> primes;
  long long p = 2;
  while (p*p<=n) {
    int counter = 0;
    while (n%p==0) {
      counter++;
      n /= p;
    }
    if (counter>0) primes.emplace_back(p,counter);
    if (p&1) p += 2;
    else p++;
  }
  if (n>1) primes.emplace_back(n,1);
  return primes;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  long long n;
  int m;
  cin >> n >> m;

  if (m==1) {
    cout << 1 << endl;
    return 0;
  }

  //////////////// 出力変数定義 ////////////////

  vector<int> result(m,-1);

  //////////////////// 処理 ////////////////////

  // 1のときの答えは1（正整数なので）を入れておく
  result.at(1) = 1;

  // Nを素因数分解する
  vector<pair<long long,int>> primes = prime_factorization(n);

  // 今回調査予定の要素の一覧
  // 初回調査の1だけ入れて初期化する
  vector<int> vec(1,1);

  // Nの指数ごとに調査するループ
  for (int i=1; !vec.empty(); i++) {

    // 次回のループで調査予定の要素一覧
    vector<int> next;

    // 素因数別に処理する
    for (auto [p,r] : primes) {

      // pはあらかじめmでの剰余をとっておく
      p %= m;

      // 作業用配列（範囲for文の中で要素追加ができないため、後で追加するものを一時保管する）
      vector<int> tmp;

      // 調査予定にあるものすべて、pの1乗からr乗までをかけて新しい数を作れないか調べる
      // 新しい数を作れたら、i乗で作れたことを記録し、tmpに記録しておく
      for (long long k : vec) {
        for (int l=0; l<r; l++) {
          k *= p;
          k %= m;
          if (result.at(k)==-1) {
            result.at(k) = i;
            tmp.emplace_back(k);
          } 
        }
      }

      // tmpにあるものを今回（より大きい素数向け）と次回（より小さい、または同じ素数向け）の調査予定に追加
      for (int k : tmp) {
        vec.emplace_back(k);
        next.emplace_back(k);
      }

    }

    // 次回調査分を今回分に移動
    swap(vec,next);

  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}