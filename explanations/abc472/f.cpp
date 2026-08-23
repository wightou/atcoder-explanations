#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// ベクトル演算ライブラリ
struct Point {
  long long x = 0;
  long long y = 0;
  long long norm2() const {
    return (x*x+y*y);
  }
  long double norm() const {
    return sqrtl(norm2());
  }
  Point operator-() const {
    return {-x,-y};
  }
  Point operator+(const Point& other) const {
    return {x+other.x,y+other.y};
  }
  Point operator-(const Point& other) const {
    return {x-other.x,y-other.y};
  }
  Point operator*(long long other) const {
    return {x*other,y*other};
  }
  Point operator/(long long other) const {
    assert(other!=0);
    return {x/other,y/other};
  }
  void operator+=(const Point& other) {
    x += other.x;
    y += other.y;
  }
  void operator-=(const Point& other) {
    x -= other.x;
    y -= other.y;
  }
  void operator*=(long long other) {
    x *= other;
    y *= other;
  }
  void operator/=(long long other) {
    assert(other!=0);
    x /= other;
    y /= other;
  }
  bool operator==(const Point& other) const {
    return (x==other.x&&y==other.y);
  }
  bool operator!=(const Point& other) const {
    return (x!=other.x||y!=other.y);
  }
  bool operator<(const Point& other) const {
    return (x<other.x||(x==other.x&&y<other.y));
  }
};
bool is_zero(const Point& a) {return (a.x==0&&a.y==0);}
long long dot(const Point& a, const Point& b) {return a.x*b.x+a.y*b.y;}
long long dot(const Point& a, const Point& b, const Point& c) {return dot(b-a, c-a);}
bool is_front(const Point& a, const Point& b, const Point& c) {return (dot(a,b,c)>0);}
bool is_perpendicular(const Point& a, const Point& b, const Point& c) {return (dot(a,b,c)==0);}
bool is_back(const Point& a, const Point& b, const Point& c) {return (dot(a,b,c)<0);}
long long cross(const Point& a, const Point& b) {return a.x*b.y-a.y*b.x;}
long long cross(const Point& a, const Point& b, const Point& c) {return cross(b-a, c-a);}
bool is_left(const Point& a, const Point& b, const Point& c) {return (cross(a,b,c)>0);}
bool is_straight(const Point& a, const Point& b, const Point& c) {return (cross(a,b,c)==0);}
bool is_right(const Point& a, const Point& b, const Point& c) {return (cross(a,b,c)<0);}
bool is_on_segment(const Point& a, const Point& b, const Point& c) {return (is_straight(a,b,c)&&!is_front(c,a,b));}
bool is_parallel(const Point& a, const Point& b) {return (cross(a,b)==0);}
bool is_parallel(const Point& a, const Point& b, const Point& c, const Point& d) {return (cross(b-a,d-c)==0);}
bool is_opposite_side(const Point& a, const Point& b, const Point& c, const Point& d) {return ((is_left(a,b,c)&&is_right(a,b,d))||(is_left(a,b,d)&&is_right(a,b,c)));}
bool is_strict_intersect(const Point& a, const Point& b, const Point& c, const Point& d) {return (is_opposite_side(a,b,c,d)&&is_opposite_side(c,d,a,b));}
bool is_intersect(const Point& a, const Point& b, const Point& c, const Point& d) {return (is_strict_intersect(a,b,c,d)||is_on_segment(a,b,c)||is_on_segment(a,b,d)||is_on_segment(c,d,a)||is_on_segment(c,d,b));}
long double distance(const Point& a, const Point& b, const Point& c) {return abs(cross(a,b,c))/(b-a).norm();}

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int n, q;
  cin >> n >> q;

  vector<Point> p(n);
  for (int i=0; i<n; i++) {
    cin >> p.at(i).x >> p.at(i).y;
  }

  /////////////////// 前処理 ///////////////////

  // 面積密度2のときの、{n-1,0,1,...,i} の密度の積分値つまり面積（2倍は整数化のため）
  // ベクトル演算を用いることで実際に積分せずに出す
  vector<long long> sum_2m(n,0);

  // 面積密度2のときの、{n-1,0,1,...,i} の密度×位置の積分値（6倍整数化のため）
  // ベクトル演算を用いることで実際に積分せずに出す
  vector<Point> sum_6p(n,{0,0});

  // 上記の2つのデータを累積和的に構築
  for (int i=1; i<n; i++) {

    // {n-1,i-1,i} の面積の2倍
    long long m = cross(p.at(n-1),p.at(i-1),p.at(i));

    // 密度の積分の累積和を更新
    sum_2m.at(i) = sum_2m.at(i-1)+m;

    // 密度×位置の積分の累積和を更新
    sum_6p.at(i) = sum_6p.at(i-1)+(p.at(n-1)+p.at(i-1)+p.at(i))*m;

  }

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int u, v;
    cin >> u >> v;
    u--;
    v--;

    //////////////// 出力変数定義 ////////////////

    double result1 = 0;
    double result2 = 0;

    //////////////////// 処理 ////////////////////

    // {n-1,v,u} の符号付き面積
    long long m = cross(p.at(n-1),p.at(v),p.at(u));

    // {u,u+1,...,v-1,v} の符号付き面積
    long long mx2 = (sum_2m.at(v)-sum_2m.at(u))+m;

    // {u,u+1,...,v-1,v} の密度×位置の積分値
    Point px6 = (sum_6p.at(v)-sum_6p.at(u))+(p.at(n-1)+p.at(v)+p.at(u))*m;

    // u>vのときは逆サイドの負の値になってしまっているので、全体を足すことで意図した値に直す
    if (u>v) {
      mx2 = sum_2m.at(n-1)+mx2;
      px6 = sum_6p.at(n-1)+px6;
    }

    // 重心を求める
    result1 = px6.x/(3.0*mx2);
    result2 = px6.y/(3.0*mx2);

    //////////////////// 出力 ////////////////////

    cout << fixed << setprecision(10) << result1 << " ";
    cout << fixed << setprecision(10) << result2 << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}