#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;

// 累乗剰余
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

// 階乗剰余
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

// AtCoder Libraryのconvolutionのラッパー
vector<long long> convolution_acl(const vector<long long>& a_org, const vector<long long>& b_org, int lim = (1<<23)-1) {
  if (a_org.empty()||b_org.empty()||lim<0) return {};
  vector<long long> a(a_org.begin(),a_org.begin()+min((int)ssize(a_org),lim+1));
  vector<long long> b(b_org.begin(),b_org.begin()+min((int)ssize(b_org),lim+1));
  vector<long long> result = convolution(a,b);
  if (ssize(result)>lim+1) result.resize(lim+1);
  return result;
}

// 逆数
// 途中計算では、全て符号反転状態で保持している
vector<long long> fps_inv(const vector<long long>& a, int lim) {
  assert(mod==998244353);
  assert(lim>=0);
  assert(!a.empty());
  assert(a[0]!=0);
  vector <long long> inv = {mod-power_mod(a[0],-1)};
  for (int n=1; n<=lim; n*=2) {
    vector<long long> tmp = convolution_acl(a,inv,min(2*n-1,lim));
    tmp[0] = (tmp[0]+2)%mod;
    inv = convolution_acl(tmp,inv,min(2*n-1,lim));
  }
  inv.resize(lim+1);
  for (long long& i : inv) if (i>0) i = mod-i;
  return inv;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 階乗データを用意
  make_factorial(n);

  // 1+x+2x^2+6x^3+24x^4+……の逆数をx^Nまで求める
  // 階乗データをそのまま利用できる
  vector<long long> tmp = fps_inv(factorial,n);
  
  result = -tmp.at(n);
  if (result<0) result += mod;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}