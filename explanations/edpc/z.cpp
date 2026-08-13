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

// 凸包（コードをコピペして使った）
// vector<Point> convex_hull(const vector<Point>& points) {
//   vector<Point> u, d;
//   for (Point p : points) {
//     while (u.size()>=2&&!is_right(u[u.size()-2],u[u.size()-1],p)) u.pop_back();
//     u.emplace_back(p);
//     while (d.size()>=2&&!is_left(d[d.size()-2],d[d.size()-1],p)) d.pop_back();
//     d.emplace_back(p);
//   }
//   if (points.size()>2) for (size_t i=u.size()-2; i>0; i--) d.emplace_back(u[i]);
//   return d;
// }

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  long long c;
  cin >> n >> c;

  vector<long long> h(n);
  for (int i=0; i<n; i++) {
    cin >> h.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // DPテーブル
  vector<long long> dp(n);

  // 下側の凸包を作る用の配列
  vector<Point> d;

  // 下側の凸包を作りながらツーポインタ法で最小値探索する
  for (int l=0,r=0; r<n; r++) {

    // 2つ目以降の足場なら、最小値を探してDPテーブル更新
    if (r!=0) {
      while (l<ssize(d)-1&&d.at(l+1).y-d.at(l).y<2*h.at(r)*(d.at(l+1).x-d.at(l).x)) l++;
      dp.at(r) = d.at(l).y-2*d.at(l).x*h.at(r)+h.at(r)*h.at(r)+c;
    }

    // 二次元上に点を追加して凸包更新
    Point p = {h.at(r),dp.at(r)+h.at(r)*h.at(r)};
    while (d.size()>=2&&!is_left(d[d.size()-2],d[d.size()-1],p)) d.pop_back();
    d.emplace_back(p);
    l = min(l,(int)ssize(d)-1);

  }

  // DPテーブル末尾の値が答え
  result = dp.back();

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}