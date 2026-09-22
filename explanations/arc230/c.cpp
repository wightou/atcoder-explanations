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

// 二項係数剰余
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

// 配列から合成パターン数を求める関数
int calc(const vector<int>& c) {

  // 合成待ちの情報保管
  vector<tuple<int,int,long long>> vec;

  // 1つずつ見る
  for (int i : c) {

    // 次に入れるデータの「深さ、区間長、パターン数」
    int d = i;
    int s = 1;
    long long num = 1;

    // vecの後ろに合成可能なものがある限り合成する
    while (!vec.empty()) {
      const auto& [d0,s0,num0] = vec.back();

      // 深さが違えば合成不可
      if (d0!=d) break;

      // 合成すると深さが1浅くなり、サイズは足し算
      // パターン数は、両方の積に加え、どちらにどの数を割り振るかのパターン数も掛ける
      d--;
      s += s0;
      num = num*num0%mod*comb(s-2,s0-1)%mod;

      // 合成完了したものを消す
      vec.pop_back();

    }

    // 合成できなくなったので、末尾に追加
    vec.emplace_back(d,s,num);

  }
  
  // もし最終的に深さ0の1要素だけになっていなかったら、失敗なので0パターン
  if (ssize(vec)!=1) return 0;
  const auto& [d_,s_,num_] = vec.at(0);
  if (d_!=0) return 0;

  // 深さ0の1要素だけになっていたら、そのパターン数が答え
  return num_;

}

// 2^(-k) を2つで 2^(-k+1) にする処理を繰り返したとして、
// 最終的に 2^0 にするのに2のマイナス何乗が不足しているか調査する
vector<int> make_lack(const vector<int>& c) {

  // 任意値に後から決められる両端を除いて、値として何がいくつあるか確認
  vector<int> counters(*max_element(c.begin(),c.end())+1,0);
  for (int i=1; i<ssize(c)-1; i++) {
    counters.at(c.at(i))++;
  }

  // 結果用
  vector<int> result;

  // 指数の絶対値が大きい方から、2^(-1)まで順に調査
  while (ssize(counters)>1) {

    // 次に処理するものの個数を分離
    int tmp = counters.back();
    counters.pop_back();

    // 奇数個だったら不足を1個追加して、2個ずつ合体
    if (tmp%2) {
      result.emplace_back(ssize(counters));
      tmp++;
    }
    counters.back() += tmp/2;

  }

  // 不足分が3個以上あったら、不可能なので空配列を返す
  // 不足分が0個の場合は、もともと空配列が返るので処理の必要なし
  // 残りが 2^0 が 1 個だけになっていなかったら、不可能なので空配列を返す
  if (counters.at(0)!=1) return {};
  if (ssize(result)>2) return {};

  // 不可能判定にひっかからなかったら、求めた結果を返す
  return result;

}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  // cを両サイドに1つ余白を残して受け取る
  vector<int> c(n+1,0);
  for (int i=1; i<n; i++) {
    cin >> c.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 階乗の準備
  make_factorial(n+1);

  // 不足分を求める
  vector<int> tmp = make_lack(c);

  if (ssize(tmp)==1) {

    // 不足分が1つだった場合、それより1大きい数を両サイドに指定してパターン数を求める
    c.at(0) = tmp.at(0)+1;
    c.at(n) = tmp.at(0)+1;
    result += calc(c);

  } else if (ssize(tmp)==2) {

    // 不足分が2つだった場合、それらを両サイドに指定してパターン数を求めて和を取る
    c.at(0) = tmp.at(0);
    c.at(n) = tmp.at(1);
    result += calc(c);
    c.at(0) = tmp.at(1);
    c.at(n) = tmp.at(0);
    result += calc(c);
    result %= mod;

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}