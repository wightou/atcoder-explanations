#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

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

  vector<int> result;

  //////////////////// 処理 ////////////////////

  // 大きい方から3つを保持するvector
  vector<int> vec;

  // aを前から1つずつ見る
  for (int i=0; i<n; i++) {

    // vecに追加して逆順ソート
    vec.emplace_back(a.at(i));
    sort(vec.rbegin(),vec.rend());

    // 4つ以上あったら、4位転落したものを削除
    if (vec.size()>3) vec.pop_back();

    // 第3位を結果に入れる
    if (vec.size()==3) result.emplace_back(vec.back());

    
  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}