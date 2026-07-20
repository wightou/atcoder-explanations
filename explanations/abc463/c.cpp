#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> h(n), l(n);
  for (int i=0; i<n; i++) {
    cin >> h.at(i) >> l.at(i);
  }

  int q;
  cin >> q;

  vector<int> t(q);
  for (int i=0; i<q; i++) {
    cin >> t.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  vector<int> result(q);

  //////////////////// 処理 ////////////////////

  // 全クエリを、時刻が後ろのものから順にしておく
  vector<pair<int,int>> query(q);
  for (int i=0; i<q; i++) {
    query.at(i) = {t.at(i),i};
  }
  sort(query.rbegin(),query.rend());

  // 「退出時刻、身長」を、時刻が後ろのものから順にしておく
  vector<pair<int,int>> vec(n);
  for (int i=0; i<n; i++) {
    vec.at(i) = {l.at(i),h.at(i)};
  }
  sort(vec.rbegin(),vec.rend());

  // 何人目の高橋君を見ているか
  int id = 0;

  // 現在の最高身長
  int mx = 0;

  // 時刻を逆再生しながら、クエリに答えていく
  for (int j=0; j<q; j++) {

    // とりあえずクエリを取り出して書きやすくする
    auto& [time,i] = query.at(j);

    // そのクエリの時刻に未退出の高橋君を全員入室させる
    while (id<n&&vec.at(id).first>time) {
      mx = max(mx,vec.at(id).second);
      id++;
    }

    // その時点の最高身長が、そのクエリの答え
    result.at(i) = mx;

  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}