#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;

// 繰り返し2乗法による a^p の計算
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

// 二項係数 nCr の逆数
long long comb_inv(int n, int r) {
  if (r<0) return 0;
  if (r>n) return 0;
  long long tmp = fact_inv[n];
  tmp *= factorial[r];
  tmp %= mod;
  tmp *= factorial[n-r];
  tmp %= mod;
  return tmp;
}

// マージソートを用いながら、vecのうちk個取った合計をkごとに列挙する関数
vector<vector<long long>> make_candidate(const vector<long long> vec) {
  int n = ssize(vec);
  vector<vector<long long>> result(n+1);
  result.at(0).emplace_back(0);
  for (long long num : vec) {
    for (int i=n; i>0; i--) {
      vector<long long> tmp;
      int a = 0;
      int b = 0;
      int a_lim = ssize(result.at(i-1));
      int b_lim = ssize(result.at(i));
      while (a<a_lim||b<b_lim) {
        if (b==b_lim||(a<a_lim&&result.at(i-1).at(a)+num<result.at(i).at(b))) {
          tmp.emplace_back(result.at(i-1).at(a)+num);
          a++;
        } else {
          tmp.emplace_back(result.at(i).at(b));
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

  int n;
  long long x;
  cin >> n >> x;

  vector<long long> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 階乗とその逆数を n! まで用意しておく
  make_factorial(n);

  // aを半分に分け、それぞれ、k個取った合計をkごとに列挙する
  vector<long long> b, c;
  for (int i=0; i<n/2; i++) {
    b.emplace_back(a.at(i));
  }
  for (int i=n/2; i<n; i++) {
    c.emplace_back(a.at(i));
  }
  vector<vector<long long>> vv1 = make_candidate(b);
  vector<vector<long long>> vv2 = make_candidate(c);

  // vv1の方の各vectorの累積和をもっておく用の場所
  vector<vector<long long>> sum1(ssize(vv1));

  // aの合計を取得しておく
  long long sum_all = accumulate(a.begin(),a.end(),0LL)%mod;

  // vv1のi個とったデータを1つずつ見る
  for (int i=0; i<ssize(vv1); i++) {

    // 書きやすさのために、vv1のi個取ったデータの長さを取得しておく
    int n1 = ssize(vv1.at(i));

    // vv1のi個取ったデータの累積和を作る
    // ただし、そのまま作ると long long 型からすらあふれるので、この時点でmodしておく
    sum1.at(i).resize(n1+1,0);
    for (int j=0; j<n1; j++) {
      sum1.at(i).at(j+1) = sum1.at(i).at(j) + vv1.at(i).at(j);
      sum1.at(i).at(j+1) %= mod;
    }

    // vv1のj個とったデータを1つずつ見る
    for (int j=0; j<ssize(vv2); j++) {

      // i+jがnのとき、次に取るものがなくゼロ除算が発生する
      // もともと意味がない計算なので、スキップしてしまう
      if (i+j==n) continue;

      // vv1.at(i)上を右から走るツーポインタ法用のインデックス
      int id = n1;

      // vv2.at(j)を左から見る（範囲for文で回すため、インデックスは略）
      for (long long k : vv2.at(j)) {

        // vv1.at(i)の値がx-k以上である最後のデータまで、idを左に動かす
        while (id>0&&vv1.at(i).at(id-1)>=x-k) id--;

        // 結果に、以下をやった計算結果を加える
        // - sum_all-k-vv1.at(i).at(id) の総和を取る
        //   - 個数倍と累積和を使って高速処理
        //   - k が大きすぎるので、オーバーフローに注意
        // - それを comb(n,i+j) で割る
        //   - 最初から逆数で求める
        // - n-i-jで割る
        //   - 逆数を求めるのに計算量をかけたくないので (n-i-j-1)!/(n-i-j)! をかける
        result += ((sum_all-k%mod)*id-sum1.at(i).at(id))%mod*comb_inv(n,i+j)%mod*factorial.at(n-i-j-1)%mod*fact_inv.at(n-i-j)%mod;
        result %= mod;

      }

    }

  }

  // 剰余類環の変な仕様のせいで負の数になっている場合への対策
  if (result<0) result += mod;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}