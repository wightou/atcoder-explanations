#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;

// n!までの階乗を用意
vector<long long> factorial;
void make_factorial(int n) {
  factorial.resize(n+1);
  factorial.at(0) = 1;
  for (int i=1; i<=n; i++) {
    factorial.at(i) = factorial.at(i-1)*i%mod;
  }
}

// マージソートを用いながら、vecのうちk個取った積でnを割った商と、
// そのパターン数、昇順限定のスコア合計をkごとに列挙する関数
// ただし、nの約数のみ考える
vector<vector<tuple<long long,int,int>>> calc(const vector<long long>& vec) {
  int len = ssize(vec);
  long long n = vec.back();
  vector<vector<tuple<long long,int,int>>> result(len+1);
  result.at(0).emplace_back(n,1,0);
  for (long long num : vec) {
    for (int i=len; i>0; i--) {
      vector<tuple<long long,int,int>> to_add;
      for (auto [remain,count,sum] : result.at(i-1)) {
        if (remain%num==0) to_add.emplace_back(remain/num,count,(sum+count*(num%mod))%mod);
      }
      vector<tuple<long long,int,int>> tmp;
      int a = 0;
      int b = 0;
      int a_lim = ssize(result.at(i));
      int b_lim = ssize(to_add);
      while (a<a_lim||b<b_lim) {
        if (b==b_lim||(a<a_lim&&get<0>(result.at(i).at(a))<get<0>(to_add.at(b)))) {
          tmp.emplace_back(result.at(i).at(a));
          a++;
        } else if (a==a_lim||(b<b_lim&&get<0>(result.at(i).at(a))>get<0>(to_add.at(b)))) {
          tmp.emplace_back(to_add.at(b));
          b++;
        } else {
          tmp.emplace_back(get<0>(to_add.at(b)),(get<1>(result.at(i).at(a))+get<1>(to_add.at(b)))%mod,(get<2>(result.at(i).at(a))+get<2>(to_add.at(b)))%mod);
          a++;
          b++;
        }
      }
      swap(result.at(i),tmp);
    }
  }
  return result;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  long long n;
  cin >> n;

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 100の階乗まであれば足りる
  make_factorial(100);

  // nの約数を全列挙する
  vector<long long> d;
  for (long long i=1; i*i<=n; i++) {
    if (n%i==0) {
       d.emplace_back(i);
      if (i*i!=n) d.emplace_back(n/i);
    }
  }
  sort(d.begin(),d.end());

  // vecのうちk個取った積でnを割った商と、その昇順限定スコア合計をkごとに列挙する
  vector<vector<tuple<long long,int,int>>> vec = calc(d);

  // 商が1になっているものについて、スコアを加算
  // 昇順だけじゃないものも含めると、使った約数の個数の階乗倍になる
  for (int i=0; i<ssize(vec); i++) {
    auto& v = vec.at(i);
    if (v.empty()) break;
    if (get<0>(v.front())==1) result += get<2>(v.front())*factorial.at(i)%mod;
  }

  result %= mod;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}