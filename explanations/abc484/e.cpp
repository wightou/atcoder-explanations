#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

struct Compression {
  vector<long long> vec;
  Compression(const vector<long long>& vec_ = {}) {
    vec = vec_;
    sort(vec.begin(),vec.end());
    vec.erase(unique(vec.begin(),vec.end()),vec.end());
  }
  int lower(long long x) {
    int y = lower_bound(vec.begin(),vec.end(),x)-vec.begin();
    return y;
  }
  int upper(long long x) {
    int y = upper_bound(vec.begin(),vec.end(),x)-vec.begin();
    return y;
  }
  int encode(long long x) {
    int y = lower_bound(vec.begin(),vec.end(),x)-vec.begin();
    assert(y<ssize(vec)&&vec[y] == x);
    return y;
  }
  long long decode (int y) {
    assert(0<=y&&y<ssize(vec));
    return vec.at(y);
  }
  long long left (int y) {
    assert(1<=y&&y<ssize(vec));
    return vec.at(y)-vec.at(y-1);
  }
  long long right (int y) {
    return left(y+1);
  }
  int size() {
    return ssize(vec);
  }
};

long long op(long long a, long long b) {
  return a+b;
}
long long inv(long long a, long long b) {
  return b-a;
}
long long e() {
  return 0;
}
template<class T, T (*op)(T,T), T (*inv)(T,T), T (*e)()>
struct CompressedFenwickTree {
  int n;
  Compression comp;
  vector<T> values;
  CompressedFenwickTree(const vector<long long>& vec) {
    comp = Compression(vec);
    n = comp.size();
    values.assign(n,e());
  }
  void add(long long pos, T x) {
    int p = comp.encode(pos);
    assert(0<=p&&p<n);
    p++;
    while(p<=n) {
      values[p-1] = op(values[p-1],x);
      p += (p&-p);
    }
  }
  void set(long long pos, T x) {
    add(pos,inv(get(pos),x));
  }
  T sum(long long pos) {
    int p = comp.lower(pos);
    T s = e();
    while(p>0) {
      s = op(s,values[p-1]);
      p -= (p&-p);
    }
    return s;
  }
  T sum(long long pos_l, long long pos_r) {
    assert(pos_l <= pos_r);
    return inv(sum(pos_l),sum(pos_r));
  }
  T get(long long pos) {return sum(pos,pos+1);}
};

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  long long l, r;
  cin >> n >> l >> r;

  vector<long long> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 事前に累積和を取る
  vector<long long> sum(n+1,0);
  partial_sum(a.begin(),a.end(),sum.begin()+1);

  // 座標圧縮付きFenwick木を作る（普通に座標圧縮+ACLのFenwick木でもいい）
  CompressedFenwickTree<long long,op,inv,e> f(sum);

  // 以前出した累積和との差が適切な範囲に収まる個数を計上する
  for (long long i : sum) {
    result += f.sum(i-r,i-l+1);
    f.add(i,1);
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}