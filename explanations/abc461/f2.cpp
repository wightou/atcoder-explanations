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

// 単調減少する、積がnになる2以上の要素からなる数列を作る深さ優先探索
void dfs(const vector<long long>& d, vector<long long>& vec, long long sum, long long value, int next, long long& result) {

  // 値が1の場合は終端とする
  if (value==1LL) {

    // 1を含まない場合と1を含む場合をそれぞれresultに足す
    int num = vec.size();
    result += sum*(factorial.at(num+1)+factorial.at(num));
    result += factorial.at(num+1);
    result %= mod;

  } else {

    // 1を含まない場合、割れる数を探す
    // 最後に割った数の前から始めるが、valueが小さければ二分探索で一気に飛ばす高速化をする
    for (int i=min(next,(int)(lower_bound(d.begin(),d.end(),value)-d.begin())); i>=0; i--) {
      if (value%d.at(i)==0) {
        vec.emplace_back(d.at(i));
        dfs(d,vec,(sum+d.at(i))%mod,value/d.at(i),i-1,result);
        vec.pop_back();
      }
    }

  }

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

  // nの1以外の約数を全列挙する
  vector<long long> d;
  for (long long i=1; i*i<=n; i++) {
    if (n%i==0) {
      if (i!=1) d.emplace_back(i);
      if (i*i!=n) d.emplace_back(n/i);
    }
  }
  sort(d.begin(),d.end());

  // 深さ優先探索で計算する
  vector<long long> vec;
  dfs(d,vec,0,n,ssize(d)-1,result);

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}