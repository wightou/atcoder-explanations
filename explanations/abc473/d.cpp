#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// 再帰でパターンを列挙する関数
// 引数は「残りで必要な和、残りで使える最大数-1、現在調査中の個数リスト、これまでに見つけた答え一覧
void calc (int k, int id, vector<int>& vec, vector<vector<int>>& result) {

  // もしあとは1しか使えなかったら、全部1で埋めて答え一覧に追加
  if (id==0) {
    vec.at(id) = k;
    result.emplace_back(vec);
    return;
  }

  // まだ2以上も使えるなら、最大数の個数全パターンについて調査
  for (int i=0; i<=k/(id+1); i++) {
    vec.at(id) = i;
    calc (k-(id+1)*i,id-1,vec,result);
  }

}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, k;
  cin >> n >> k;

  //////////////// 出力変数定義 ////////////////

  vector<vector<int>> result;

  //////////////////// 処理 ////////////////////

  // 再帰で使う用の長さnのvector
  vector<int> vec(n,0);

  // 必要な和k、使える最大数nで探索をかける
  calc (k,n-1,vec,result);

  // 辞書順にソート
  sort(result.begin(),result.end());

  //////////////////// 出力 ////////////////////

  for (size_t j=0; j<result.size(); j++) {
    for (size_t i=0; i<result.at(j).size(); i++) {
      cout << result.at(j).at(i);
      if (i!=result.at(j).size()-1) {
        cout << " ";
      }
    }
    cout << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}