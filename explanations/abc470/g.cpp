#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int INF = 1001001001;

// 範囲代入と区間和ができるlazy segment木用の関数5つ
struct Data {
  long long sum = 0;
  int mn = INF;
  int num = 0;
};

Data op (Data a, Data b) {
  Data c;
  c.sum = a.sum+b.sum;
  c.mn = min(a.mn,b.mn);
  c.num = a.num+b.num;
  return c;
}

Data e() {
  return Data();
}

Data mp(int f, Data x) {
  x.mn = min(f,x.mn);
  if (f!=INF) x.sum = 1LL*f*x.num;
  return x;
}

int com(int f, int g) {
  return min(f,g);
}

int id() {
  return INF;
}

// 二分探索用の関数オブジェクト
struct Judge {
  int b;
  bool operator()(Data x) const {
    return x.mn > b;
  }
};

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 各数値がどこにあるのかを 1-indexed でバケツソート
  // 番兵としてすべてに 0 を入れておく
  vector<vector<int>> vec(n+1,vector(1,0));
  for (int i=0; i<n; i++) {
    vec.at(a.at(i)).emplace_back(i+1);
  }

  // 範囲代入と区間和ができるlazy segment木
  // seg[i] の葉は、i以下の各数の最終出現位置の最小値
  // sumはその区間の合計値、mnはその最小値、numはその範囲の個数
  lazy_segtree<Data,op,e,int,mp,com,id> seg(n+1);

  // 初期値セット
  int mn = INF;
  for (int i=0; i<=n; i++) {
    mn = min(mn,vec.at(i).back());
    Data c;
    c.sum = mn;
    c.mn = mn;
    c.num = 1;
    seg.set(i,c);
  }

  // 後ろから順に調べるループ
  for (int i=n; i>0; i--) {

    // そこで終わる区間のmexの和を加算
    result += seg.all_prod().sum;

    // aを1つ縮める
    // seg上の情報は、その値以降の値全てを最小値更新したい
    // 二分探索で更新すべき右端位置を求めて、区間代入
    int val = a.at(i-1);
    vec.at(val).pop_back();
    int pos = vec.at(val).back();
    int r = seg.max_right(val,Judge(pos));
    seg.apply(val,r,pos);

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}