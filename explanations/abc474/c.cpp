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

  // もう並んだかを管理する配列
  vector<bool> flags(n,false);

  // （逆再生で見て）次に並べる位置
  int id = n-1;

  // 後から移動した分を逆再生で見て、初めて出てきたやつから順に後ろから並べる
  for (int i=q-1; i>=0; i--) {
    if (flags.at(a.at(i))) continue;
    flags.at(a.at(i)) = true;
    result.at(id) = a.at(i)+1;
    id--;
  }

  // 最初の順列も同じように処理する
  for (int i=n-1; i>=0; i--) {
    if (flags.at(p.at(i))) continue;
    flags.at(p.at(i)) = true;
    result.at(id) = p.at(i)+1;
    id--;
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