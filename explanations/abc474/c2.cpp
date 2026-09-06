#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, q;
  cin >> n >> q;

  vector<int> p(n);
  for (int i=0; i<n; i++) {
    cin >> p.at(i);
    p.at(i)--;
  }
  vector<int> a(q);
  for (int i=0; i<q; i++) {
    cin >> a.at(i);
    a.at(i)--;
  }

  //////////////// 出力変数定義 ////////////////

  vector<int> result(n);

  //////////////////// 処理 ////////////////////

  // 今それぞれ何番目にいるかを管理する配列
  vector<int> id(n);

  // 逆写像でもともといる位置を記録
  for (int i=0; i<n; i++) {
    id.at(p.at(i)) = i;
  }

  // クエリ分を処理
  // 抜けた分を詰めずに、N+1番目以降に並んでもらうことにする
  for (int i=0; i<q; i++) {
    id.at(a.at(i)) = n+i;
  }

  // すべての数の「位置、数」をソート
  vector<pair<int,int>> vec(n);
  for (int i=0; i<n; i++) {
    vec.at(i) = {id.at(i),i};
  }
  sort(vec.begin(),vec.end());

  // ソート順を見て答えを整理
  for (int i=0; i<n; i++) {
    result.at(i) = vec.at(i).second+1;
  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i);
    if (i!=result.size()-1) {
      cout << " ";
    }
  }
  cout << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}