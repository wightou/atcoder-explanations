#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, k;
  cin >> n >> k;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 累積和で出てきた値一覧
  set<int> st;

  // 空状態での0だけ入れておく
  st.emplace(0);

  // 累積和、保管する意味はないのでただのintでよい
  int sum = 0;

  // 前から順に見る
  for (int i : a) {

    // 累積和更新
    sum += i;
    sum %= k;

    // もう出た数かをチェック
    if (st.contains(sum)) {

      // もう出た数だった場合、ここを右端とするような、条件を満たす区間がある
      // その区間を採用し、既出数一覧を空状態での0だけに戻し、累積和もリセット
      result++;
      st.clear();
      st.emplace(0);
      sum = 0;

    } else {

      // まだ出てないなら、追加しておく
      st.emplace(sum);

    }

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}