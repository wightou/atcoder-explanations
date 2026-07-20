#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

int mod = 998244353;

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

// 素数判定
bool is_prime (long long n) {
  if (n<2) return false;
  if (n%2==0&&n>2) return false;
  for (long long i=3; i*i<=n; i+=2) {
    if (n%i==0) return false;
  }
  return true;
}

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////



  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int n;
    cin >> n;

    vector<int> a(n);
    for (int i=0; i<n; i++) {
      cin >> a.at(i);
    }

    //////////////// 出力変数定義 ////////////////

    long long result = 0;

    //////////////////// 処理 ////////////////////

    // nが合成数の場合は答えは明らかに0
    // nが素数の場合のみ処理する（合成数の場合に処理しても、計算量が増えるだけではある）
    if (is_prime(n)) {

      // 剰余類環の法をnに再設定し、(n/2)!%nを作り直す
      mod = n;
      factorial.clear();
      make_factorial(n/2);

      // ((n/2)!)^n%nを求めて、resultに入れておく
      // これが符号以外は答えそのもの、あとは符号だけ調整すればよい
      result = power_mod(factorial.at(n/2),n);

      // aはソートした状態で考えてよい
      sort(a.begin(),a.end());

      // ここまでに、nで割った余りがiであるものがいくつ出たか管理するfenwick木
      fenwick_tree<int> f(n);

      // aのデータを前からチェックするループ
      for (int i : a) {

        // iが大きい値である場合があるので、%nした値も持っておく
        int imod = i%mod;

        // 割った余りがimodになっているものが既出の場合は、答えは0
        if (f.sum(imod,imod+1)!=0) {
          result = 0;
          break;
        }

        // （今回の値-以前の値）%nの値がn/2より大きいなら、-1倍
        // それをfenwick木で以前の分をまとめて処理する
        // nが2のときは、そもそも-1倍で不変なので、深く考えなくてよい
        int counter = 0;
        if (imod>=n/2) counter = f.sum(0,n)-f.sum(imod-n/2,imod+1);
        else counter = f.sum(imod+1,imod+n/2+1);
        if (counter%2) result *= -1;
        f.add(imod,1);

      }

    }

    // resultが負の数になっていた場合の補正
    if (result<0) result += mod;

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}