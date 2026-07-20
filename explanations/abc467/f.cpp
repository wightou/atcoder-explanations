#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// セグ木用関数
// first は最後の手紙を出し終えるまで、secondは最後の手紙の返事が来るまで
pair<long long,long long> op (pair<long long,long long> x, pair<long long,long long> y) {
  pair<long long,long long> z;
  z.first = x.first+y.first;
  z.second = max(x.second,x.first+y.second);
  return z;
}

pair<long long,long long> e() {
  return {0,0};
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, q;
  cin >> n >> q;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }
  vector<int> b(n);
  for (int i=0; i<n; i++) {
    cin >> b.at(i);
  }

  // クエリを先読みする
  vector<int> type(q), p(q), x(q);
  for (int i=0; i<q; i++) {
    cin >> type.at(i) >> p.at(i) >> x.at(i);
    p.at(i)--;
  } 

  //////////////// 出力変数定義 ////////////////

  vector<long long> result(q);

  //////////////////// 処理 ////////////////////

  // 初期値+クエリ2の値で、bが大きい順に並べる
  vector<pair<int,int>> tmp(n);
  for (int i=0; i<n; i++) {
    tmp.at(i) = {b.at(i),i};
  }
  for (int i=0; i<q; i++) {
    if (type.at(i)==2) tmp.emplace_back(x.at(i),n+i);
  }
  sort(tmp.rbegin(),tmp.rend());

  // <nでは、その番号の宛先のデータのセグ木上の現在位置
  // >=nでは、位置-n番目のクエリでデータを移転する先
  vector<int> pos(n+q,-1);
  for (int i=0; i<ssize(tmp); i++) {
    pos.at(tmp.at(i).second) = i;
  }

  // 「全部書くまでの時間、全部返事が来るまでの時間」を管理するセグ木
  // bの値が小さい奴ほど右に入れる
  segtree<pair<long long,long long>,op,e> seg(ssize(tmp));

  // 初期データを全部セグ木の対応位置に入れる
  for (int i=0; i<n; i++) {
    seg.set(pos.at(i),{a.at(i),a.at(i)+b.at(i)});
  }

  // クエリを1つずつ処理する
  for (int i=0; i<q; i++) {

    // 書きやすさのため、aやb上のどこを見ているか取得しておく
    int p0 = p.at(i);

    if (type.at(i)==1) {

      // タイプ1の場合、aが変わるだけなので、同じ位置で書き換える
      a.at(p0) = x.at(i);
      seg.set(pos.at(p0),{a.at(p0),a.at(p0)+b.at(p0)});

    } else {

      // タイプ2の場合、bが変わるので、値が変わるだけでなく、pos.at(n+i) を見て位置を変更する
      b.at(p0) = x.at(i);
      seg.set(pos.at(p0),e());
      pos.at(p0) = pos.at(n+i);
      seg.set(pos.at(p0),{a.at(p0),a.at(p0)+b.at(p0)});

    }

    // 全ての積のsecondが答え
    result.at(i) = seg.all_prod().second;

  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}