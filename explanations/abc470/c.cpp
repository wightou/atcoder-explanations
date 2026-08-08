#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  /////////////////// 前入力 ///////////////////
  
  int n, q;
  cin >> n >> q;

  /////////////////// 前処理 ///////////////////

  // 実際の配列
  vector<int> a(n,0);

  // xorの値
  int result = 0;

  // 今1以上の値が入っているところ一覧
  set<int> st;

  // 削除作業用一時スペース
  vector<int> del;

  /////////////////// ループ ///////////////////

  // クエリ数だけループ
  for (int loop=0; loop<q; loop++) {
    
    // クエリタイプを受け取る
    int type;
    cin >> type;
    
    if (type==1) {

      // タイプ1の場合

      int x;
      cin >> x;
      x--;

      // resultから、元のa[i]の影響を消す
      result ^= a.at(x);

      // a[i]を1増やす
      a.at(x)++;
      if (a.at(x)==1) st.emplace(x);

      // resultに、新しいa[i]の影響を入れる
      result ^= a.at(x);
           
    } else if (type==2) {

      // stに入っているもの全てから1減らす
      for (int x : st) {

        // resultから、元のa[i]の影響を消す
        result ^= a.at(x);

        // a[i]を1減らし、0になったら削除予定に入れる
        a.at(x)--;
        if (a.at(x)==0) del.emplace_back(x);

        // resultに、新しいa[i]の影響を入れる
        result ^= a.at(x);
      }

      // 削除予定に入っているものを、全て実際に削除する
      for (int x : del) {
        st.erase(x);
      }
      del.clear();

    }

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}