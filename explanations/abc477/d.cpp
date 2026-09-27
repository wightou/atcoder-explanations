#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, q;
  cin >> n >> q;

  // bはクエリ1用、cはクエリ2用
  vector<int> a(q), b(q);
  string c(q,'.');
  for (int i=0; i<q; i++) {
    cin >> a.at(i);
    if (a.at(i)==1) {
      cin >> b.at(i);
      b.at(i)--;
    } else {
      cin >> c.at(i);
    }
  }

  //////////////// 出力変数定義 ////////////////

  string result = string(n,'.');

  //////////////////// 処理 ////////////////////

  // 未着色で、タイルがある/ないマスの一覧
  set<int> closed;
  set<int> open;

  // 最初は全てのマスが、未着色のタイルなし
  for (int i=0; i<n; i++) open.emplace(i);

  // クエリ1だけシミュレーションして、最終状態のタイル有無分類を行う
  for (int i=0; i<q; i++) {
    if (a.at(i)==2) continue;

    //1ならopenとclosedの間を移動する
    if (closed.contains(b.at(i))) {
      closed.erase(b.at(i));
      open.emplace(b.at(i));
    } else if (open.contains(b.at(i))) {
      open.erase(b.at(i));
      closed.emplace(b.at(i));
    }

  }

  // 今度は逆再生シミュレーションをする
  for (int i=q-1; i>=0; i--) {

    // 逆再生で色塗りが初めて行われた色が、最後に塗られた色
    // openに入っているものを全部「塗って、処理済みに」をする
    if (a.at(i)==2) {
      for (int id : open) result.at(id) = c.at(i);
      open.clear();
      continue;
    }

    // 1ならopenとclosedの間を移動する
    if (closed.contains(b.at(i))) {
      closed.erase(b.at(i));
      open.emplace(b.at(i));
    } else if (open.contains(b.at(i))) {
      open.erase(b.at(i));
      closed.emplace(b.at(i));
    }

  }

  // まだ塗られていないものは、最初の'a'のままのはず
  for (int i : open) result.at(i) = 'a';
  for (int i : closed) result.at(i) = 'a';
  
  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}