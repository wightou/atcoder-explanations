#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;

// 繰り返し二乗法
long long power_mod(long long a, long long p) {
  if (p==0) return 1LL;
  a %= mod;
  if (a<0) a += mod;
  if (p<0) {
    assert(gcd(a,mod)==1);
    p = (p%(mod-1))+mod-1;
  }
  long long result = 1;
  long long b = a;
  while (p>0) {
    if (p&1LL) result = result*b%mod;
    b = b*b%mod;
    p >>= 1;
  }
  return result;
}

// 階乗とその逆元
vector<long long> factorial;
vector<long long> fact_inv;
void make_factorial(int n, bool make_inv = true) {
  assert(n>=0);
  if (make_inv) assert(n<mod);
  factorial.assign(n+1,1);
  for (int i=1; i<=n; i++) {
    factorial[i] = factorial[i-1]*i%mod;
  }
  if (!make_inv) return;
  fact_inv.assign(n+1,power_mod(factorial[n],-1));
  for (int i=n; i>0; i--) {
    fact_inv[i-1] = fact_inv[i]*i%mod;
  }
}

// 二項係数
long long comb(int n, int r) {
  if (r<0) return 0;
  if (r>n) return 0;
  long long result = factorial[n];
  result *= fact_inv[r];
  result %= mod;
  result *= fact_inv[n-r];
  result %= mod;
  return result;
}

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////

  // Nとしてありえる最大値まで、階乗とその逆数を事前計算しておく
  make_factorial(200200);

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int n, k;
    cin >> n >> k;

    vector<int> a(n);
    for (int i=0; i<n; i++) {
      cin >> a.at(i);
    }

    //////////////// 出力変数定義 ////////////////

    long long result = 1;

    //////////////////// 処理 ////////////////////

    // 前から順に見ていくループ
    for (int id=0; id<n; ) {

      // aの今見ている位置の値が「kの整数倍」「kの半整数倍」「それ以外」でわける
      if (a.at(id)%k==0) {

        // 「kの整数倍」の場合
        // kの整数倍が続く範囲全体の中を、好きに並べ替えられる
        // よって、整数倍が続く限り各数が何回登場するかチェックし、同じものがある順列として処理する
        // その答えをresultにかける
        int counter = 0;
        map<int,int> mp;
        while (id<n&&a.at(id)%k==0) {
          counter++;
          mp[a.at(id)]++;
          id++;
        }
        result *= factorial.at(counter);
        result %= mod;
        for (auto [key,val] : mp) {
          result *= fact_inv.at(val);
          result %= mod;
        }

      } else if (a.at(id)%k*2==k) {

        // 「kの半整数倍」の場合
        // kの半整数倍が続く範囲全体の中を、好きに並べ替えられる
        // よって、半整数倍が続く限り各数が何回登場するかチェックし、同じものがある順列として処理する
        // その答えをresultにかける
        int counter = 0;
        map<int,int> mp;
        while (id<n&&a.at(id)%k*2==k) {
          counter++;
          mp[a.at(id)]++;
          id++;
        }
        result *= factorial.at(counter);
        result %= mod;
        for (auto [key,val] : mp) {
          result *= fact_inv.at(val);
          result %= mod;
        }

      } else {

        // 「それ以外」の場合
        // 余りがそれと同じまたはそのマイナスである限り情報を集める
        // 余りがプラスマイナス異なるもの同士は自由に入れ替えられる
        // よって、プラスを置く位置の選び方をresultにかける

        int a1 = a.at(id)%k;
        int a2 = k-a1;
        int counter1 = 0;
        int counter2 = 0;
        while (id<n&&(a.at(id)%k==a1||a.at(id)%k==a2)) {
          if (a.at(id)%k==a1) counter1++;
          else counter2++;
          id++;
        }
        result *= comb(counter1+counter2,counter1);
        result %= mod;

      }

    }

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}