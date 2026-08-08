#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  /////////////////// 前入力 ///////////////////
  
  int n, q;
  cin >> n >> q;

  // p[0]にダミーデータを置いて、1-indexedで受け取る
  vector<int> p(n+1,0);
  for (int i=1; i<=n; i++) {
    cin >> p.at(i);
  }

  /////////////////// 前処理 ///////////////////

  // inv[0]にダミーデータを置いて、逆写像を作る
  vector<int> inv(n+1,0);
  for (int i=1; i<=n; i++) {
    inv.at(p.at(i)) = i;
  }

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {
    
    int type;
    cin >> type;
    
    if (type==1) {

      int x, y;
      cin >> x >> y;

      // 本データと逆写像それぞれ、該当箇所を入れ替える
      swap(inv.at(p.at(x)),inv.at(p.at(y)));
      swap(p.at(x),p.at(y));
      
    } else if (type==2) {

      // 本データと逆写像を入れ替える
      swap(p,inv);

    }

  }

  /////////////////// 後処理 ///////////////////

  for (size_t i=1; i<p.size(); i++) {
    cout << p.at(i);
    if (i!=p.size()-1) {
      cout << " ";
    }
  }
  cout << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}