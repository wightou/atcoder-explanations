#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int INF = 1001001001;

// 座標圧縮
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

// 座標圧縮内蔵Fenwick木
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
  cin >> n;

  vector<long long> s(n);
  for (int i=0; i<n; i++) {
    cin >> s.at(i);
  }
  vector<long long> t(n);
  for (int i=0; i<n; i++) {
    cin >> t.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 隙間にも全てダミーの人がいると考える（ダミーの人同士は入れ替わらない）
  // 各人のブロックについて、「先頭の人の初期位置、先頭の人の行先位置、人数」を入れる
  vector<tuple<int,int,int>> vec(n);

  // とりあえず、ダミーじゃない人は1人1ブロックでデータを入れる
  for (int i=0; i<n; i++) {
    vec.at(i) = {s.at(i),t.at(i),1};
  }
  
  // ダミーの人を、連続する人たちはまとめて1ブロックにしてデータを入れる

  // 初期位置と行先位置の、飛ばされるべき値一覧
  vector<long long> s0 = s;
  vector<long long> t0 = t;
  sort(s0.begin(),s0.end());
  sort(t0.begin(),t0.end());

  // 全体の最大値より右は考えなくてよい
  int s_idx = max(s0.back(),t0.back());
  int t_idx = s_idx;

  // 「これ以上考える必要がない」になるまでループ
  while (true) {

    // 次が飛ばすべき値だったらそれを処理
    while (!s0.empty()&&s0.back()==s_idx) {
      s0.pop_back();
      s_idx--;
    }
    while (!t0.empty()&&t0.back()==t_idx) {
      t0.pop_back();
      t_idx--;
    }

    // 両方から何もなくなっていたら、それ以上継続しなくていい
    if (s0.empty()&&t0.empty()) break;

    // sとtそれぞれ、そこからそれだけダミーの人が並ぶことになるか
    int s_diff = s_idx+1;
    if (!s0.empty()) s_diff -= s0.back()+1;
    int t_diff = t_idx+1;
    if (!t0.empty()) t_diff -= t0.back()+1;

    // 少ない方にあわせて、ダミーの人のデータを切り出して入れる
    int diff = min(s_diff,t_diff);
    s_idx -= diff;
    t_idx -= diff;
    vec.emplace_back(s_idx+1,t_idx+1,diff);

  }

  // 本当の人、ダミーの人を全部まとめて初期位置の値でソート
  sort(vec.begin(),vec.end());

  // 行先位置に対応する座標圧縮内蔵Fenwick木を用意
  vector<long long> tmp(ssize(vec));
  for (int i=0; i<ssize(vec); i++) tmp.at(i) = get<1>(vec.at(i));
  CompressedFenwickTree<long long,op,inv,e> f(tmp);

  // 人数分の重み付きで、転倒数を求める
  for (auto [_,pos,num] : vec) {
    result += f.sum(pos,INF)*num;
    f.add(pos,num);
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}