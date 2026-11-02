#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  /////////////////// 前入力 ///////////////////
  
  int h, w, n, q;
  cin >> h >> w >> n >> q;

  vector<int> r(n), c(n);
  for (int i=0; i<n; i++) {
    cin >> r.at(i) >> c.at(i);
  }

  /////////////////// 前処理 ///////////////////

  // rとcの組でデータを扱うことはないので、個別にソートしてしまう
  sort(r.begin(),r.end());
  sort(c.begin(),c.end());

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {
    
    int type;
    cin >> type;
    
    if (type==1) {

      int u, d;
      cin >> u >> d;

      // 二分探索で、uの右側のイテレータとdの左側のイテレータの距離を測る
      cout << upper_bound(r.begin(),r.end(),d)-lower_bound(r.begin(),r.end(),u) << endl;
      
    } else if (type==2) {

      int l, right;  // 問題でrという変数名がかぶっているので、それを回避
      cin >> l >> right;

      // 二分探索で、rightの右側のイテレータとlの左側のイテレータの距離を測る
      cout << upper_bound(c.begin(),c.end(),right)-lower_bound(c.begin(),c.end(),l) << endl;

    }

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}