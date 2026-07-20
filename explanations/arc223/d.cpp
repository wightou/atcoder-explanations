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

// 階乗とその逆数
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

// 二項係数とその逆数
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

long long comb_inv(int n, int r) {
  assert(r>=0);
  assert(r<=n);
  assert(n<mod);
  long long result = fact_inv[n];
  result *= factorial[r];
  result %= mod;
  result *= factorial[n-r];
  result %= mod;
  return result;
}

// 一般化カタラン数
// カタラン数の (0,0) から (N,N) までではなく、一般的な座標間の移動パターン数
long long generalized_catalan(int x1, int y1, int x2, int y2) {
  long long result = comb(x2-x1+y2-y1,y2-y1)-comb(x2-x1+y2-y1,x2-y1+1);
  result %= mod;
  if (result<0) result += mod;
  return result;
}

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////

  make_factorial(10010010);

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int a, b, k;
    cin >> a >> b >> k;

    //////////////// 出力変数定義 ////////////////

    long long result = 0;

    //////////////////// 処理 ////////////////////

    // 全てのAとBの並べ方を求めておく
    // 処理の中で、最後まで引ききれるパターン数にする
    long long count = comb(a+b,a);

    // i回目を見る
    for (int i=0; k+2*i<a+b; i++) {

      // i回目が終わった後に山札が残っているのに終了するようなAとBの並び順パターン数
      long long num = 0;
      if (i<=a&&i+k<=b) num = generalized_catalan(0,-k+1,i,i)*comb(a+b-k-2*i,a-i)%mod;

      // パターン数×その場合の最終枚数を加えていく（分母はあとでまとめて処理する）
      result += num*(k+i)%mod;

      // パターン数合計から引いておく
      count -= num;
      count %= mod;

    }

    // 最後まで引ききれるパターン数×その場合の最終枚数を足す
    result += count*b;
    result %= mod;

    // 全てのAとBの並べ方で割れば答え
    result *= comb_inv(a+b,a);
    result %= mod;
    if (result<0) result += mod;

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}