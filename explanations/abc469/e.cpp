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
// pair<vector<Point>,vector<Point>> convex_hull(const vector<Point>& points) {
//   vector<Point> u, d;
//   for (Point p : points) {
//     while (u.size()>=2&&!is_right(u[u.size()-2],u[u.size()-1],p)) u.pop_back();
//     u.emplace_back(p);
//     while (d.size()>=2&&!is_left(d[d.size()-2],d[d.size()-1],p)) d.pop_back();
//     d.emplace_back(p);
//   }
//   return {u,d};
// }

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, k;
  string s;
  cin >> n >> k >> s;

  //////////////// 出力変数定義 ////////////////

  double result = 0;

  //////////////////// 処理 ////////////////////

  // （これまでの試合数、勝利数）という座標に点を打っていく、最初の (0,0) も含めて n+1 点
  vector<Point> tmp(1,{0,0});
  for (int i=0; i<n; i++) {
    tmp.emplace_back(tmp.back());
    tmp.back().x++;
    if (s.at(i)=='o') tmp.back().y++;
  }

  // 下側の凸包を作る用の配列
  vector<Point> d;

  // tmp上[l1,r]でy座標差kを保つ尺取法をしながら、d上でl2を動かすツーポインタ法を併走
  for (int l1=0, l2=0, r=0; r<=n; r++) {

    // r番目の点よりもy座標がk以上低い点だけの凸包を作る（凸包ライブラリの一部を使用）
    // 該当する点がなければ空
    while (tmp.at(r).y-tmp.at(l1).y>=k) {
      while (d.size()>=2&&!is_left(d[d.size()-2],d[d.size()-1],tmp.at(l1))) d.pop_back();
      d.emplace_back(tmp.at(l1));
      l1++;
    }

    // dの中身が空なら、次のrへ
    if (d.empty()) continue;

    // 凸包を更新した結果短くなっている可能性があるので、その場合はl2をdの最後尾に戻す
    l2 = min(l2,(int)ssize(d)-1);

    // d上でl2を走らせて、y座標にk以上の差がある範囲内を探索
    // l2 が示す点とrが示す点の2点を結ぶ直線の傾きが最大になる点を探す
    while (l2<ssize(d)-1) {
      Point p1 = tmp.at(r)-d.at(l2);
      Point p2 = tmp.at(r)-d.at(l2+1);
      if (p1.y*p2.x<p1.x*p2.y) l2++;
      else break;
    }

    // l2 が示す点とrが示す点の2点を結ぶ直線の傾きが最大記録だったら記録
    Point p = tmp.at(r)-d.at(l2);
    result = max(result,1.0*p.y/p.x);

  }

  //////////////////// 出力 ////////////////////

  cout << fixed << setprecision(10) << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}