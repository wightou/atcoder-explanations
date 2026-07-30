#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr long long INF = 1001001001001001001LL;

// 区間最大値と範囲加算のlazy segment木用の関数5つ
long long op(long long a, long long b) {
  return max(a,b);
}

long long e() {
  return -INF;
}

long long mp(long long f, long long x) {
  return x+f;
}

long long comp(long long f, long long g) {
  return f+g;
}

long long id() {
  return 0;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;

  vector<int> l(m), r(m), a(m);
  for (int i=0; i<m; i++) {
    cin >> l.at(i) >> r.at(i) >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 範囲をrの値でバケツソート
  vector<vector<pair<int,int>>> vec(n+1);
  for (int i=0; i<m; i++) {
    vec.at(r.at(i)).emplace_back(l.at(i),a.at(i));
  }

  // DPテーブルを区間最大値と範囲加算のlazy segment木でもつ
  // seg[j]は、得点に絡むうち最も右にある1がj番目である場合の最大得点（1-indexed）
  lazy_segtree<long long,op,e,long long,mp,comp,id> seg(n+1);
  
  // 全部0にする場合は、0点
  seg.set(0,0);

  // 「rがi以下の区間のみ考える」のiを1つずつ広げていく
  for (int i=1; i<=n; i++) {

    // 書きやすくするために参照を持っておく
    auto& v = vec.at(i);

    // 注目地点より左で終わるうちの最大値を求め、注目地点に書き込む
    seg.set(i,seg.prod(0,i));

    // rがiである区間全部、該当区間に得点加算する
    for (int j=0; j<ssize(v); j++) {
      seg.apply(v.at(j).first,i+1,v.at(j).second);
    }

  }

  // 全区間を追加した状態での、全体の最大値が答え
  result = seg.all_prod();

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}