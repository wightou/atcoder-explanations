#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int INF = 1001001001;

// 遅延segment木用のデータ一式
struct Data{
  long long val = 0;
  int num = 0;
};
Data op(Data a, Data b) {
  a.val += b.val;
  a.num += b.num;
  return a;
}
Data e() {
  return Data();
}
Data mping(long long f, Data x) {
  x.val += f*x.num;
  return x;
}
long long com(long long f, long long g) {
  return f+g;
}
long long id() {
  return 0;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m, q;
  cin >> n >> m >> q;

  vector<int> l(n), r(n);
  for (int i=0; i<n; i++) {
    cin >> l.at(i) >> r.at(i);
    l.at(i)--;
    r.at(i)--;
  }

  vector<int> a(q), b(q), c(q), d(q);
  for (int i=0; i<q; i++) {
    cin >> a.at(i) >> b.at(i) >> c.at(i) >> d.at(i);
    a.at(i)--;
    b.at(i)--;
    c.at(i)--;
    d.at(i)--;
  }

  //////////////// 出力変数定義 ////////////////

  vector<long long> result(q);

  //////////////////// 処理 ////////////////////

  // 後の二次元累積和で必要になる位置を記録しておく
  map<pair<int,int>,long long> mp;
  for (int i=0; i<q; i++) {
    mp[{a.at(i),c.at(i)}] = 0;
    mp[{a.at(i),d.at(i)+1}] = 0;
    mp[{b.at(i)+1,c.at(i)}] = 0;
    mp[{b.at(i)+1,d.at(i)+1}] = 0;
  }

  // 番兵を入れておく
  mp[{INF,INF}] = 0;

  // 遅延segment木を用意、区間和と区間加算
  vector<Data> tmp(1,Data());
  tmp.at(0).num = 1;
  tmp.assign(m,tmp.at(0));
  lazy_segtree<Data,op,e,long long,mping,com,id> seg(tmp);

  // イテレータで前から継続で見れるようにしておく
  auto it = mp.begin();

  // 遅延segment木を利用して、二次元累積和に必要なところを埋める
  for (int i=0; ; i++) {
    while ((*it).first.first==i) {
      (*it).second = seg.prod(0,(*it).first.second).val;
      it++;
    }
    if (i==n) break;
    seg.apply(l.at(i),r.at(i)+1,1);
  }

  // クエリ順に、二次元累積和で答えを出す
  for (int i=0; i<q; i++) {
    result.at(i) = mp[{a.at(i),c.at(i)}]-mp[{a.at(i),d.at(i)+1}]-mp[{b.at(i)+1,c.at(i)}]+mp[{b.at(i)+1,d.at(i)+1}];
  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}