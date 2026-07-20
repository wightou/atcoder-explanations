#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// ベクトル演算
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

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////



  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    Point p, q, r, s;
    cin >> p.x >> p.y >> q.x >> q.y >> r.x >> r.y >> s.x >> s.y;

    //////////////// 出力変数定義 ////////////////

    string result = "Yes";

    //////////////////// 処理 ////////////////////

    // 「PQとRSが平行でない」「PQ//RSの等脚台形（退化して一直線でも可）」なら、条件を満たす
    if (is_parallel(p,q,r,s)&&((r-q).norm2()!=(s-p).norm2()||(r-p).norm2()!=(s-q).norm2())) result = "No";

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}