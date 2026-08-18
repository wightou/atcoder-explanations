#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  /////////////////// 前入力 ///////////////////
  
  int q, v;
  cin >> q >> v;

  /////////////////// 前処理 ///////////////////

  // 時刻0でのバッテリー容量が多い順を管理する優先度付きキュー
  priority_queue<int> que;

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    int type;
    cin >> type;
    
    if (type==1) {

      int t, w;
      cin >> t >> w;

      // 「時刻0に残量w-tのものを挿してあったのが今見つかった」と読み替えてqueに入れる
      que.emplace(w-t);
      
    } else if (type==2) {

      int t;
      cin >> t;

      if (que.empty()) {

        // 空なら、排出できるものがないので -1
        cout << -1 << endl;

      } else {

        // 空でないなら、最も初期残量が高いもの排出（V以上ならVに丸める）
        cout << min(v,que.top()+t) << endl;
        que.pop();

      }

    }

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}