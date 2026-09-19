#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;

long long power_mod(long long a, long long p) {
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

// AtCoder Libraryのconvolutionのラッパー
vector<long long> convolution_acl(const vector<long long>& a_org, const vector<long long>& b_org, int lim = (1<<23)-1) {
  if (a_org.empty()||b_org.empty()||lim<0) return {};
  vector<long long> a(a_org.begin(),a_org.begin()+min((int)ssize(a_org),lim+1));
  vector<long long> b(b_org.begin(),b_org.begin()+min((int)ssize(b_org),lim+1));
  vector<long long> result = convolution(a,b);
  if (ssize(result)>lim+1) result.resize(lim+1);
  return result;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m, l;
  cin >> n >> m >> l;

  vector<int> a(m);
  for (int i=0; i<m; i++) {
    cin >> a.at(i);
  }
  vector<int> b(l);
  for (int i=0; i<l; i++) {
    cin >> b.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 2*n以上の2の累乗を探す
  int len = 1;
  while (len<2*n) len *= 2;

  // サイコロの母関数
  int p = power_mod(m,-1);
  vector<long long> fa(len,0);
  for (int i : a) fa.at(i) = p;

  // 滞在可能な床の管理
  // ゴールマス以降は、滞在不可能とみなす
  vector<bool> flags(len,true);
  for (int i : b) flags.at(i) = false;
  for (int i=n; i<len; i++) flags.at(i) = false;

  // DPテーブル、値はそのマスに止まる確率
  deque<long long> dp(len,0);
  dp.at(0) = 1;

  // 分割統治法でDPを高速に進める
  // 落とし穴だったら0に、ゴールマス以降ならゴール率に加算してから0にする
  for (int i=1; i<2*n; i++) {
    int w = i&-i;
    vector<long long> left = vector(dp.begin()+i-w,dp.begin()+i);
    vector<long long> transition = convolution_acl(left,fa,2*w);
    for (int j=0; j<w; j++) {
      dp.at(i+j) += transition.at(w+j);
      dp.at(i+j) %= mod;
    }
    if (!flags.at(i)) {
      if (i>=n) result += dp.at(i);
      result %= mod;
      dp.at(i) = 0;
    }
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}