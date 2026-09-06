#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////



  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int n;
    cin >> n;

    vector<int> a(n), b(n);
    for (int i=0; i<n; i++) {
      cin >> a.at(i) >> b.at(i);
    }

    //////////////// 出力変数定義 ////////////////

    long long result = 0;

    //////////////////// 処理 ////////////////////

    // ダブって1個何かを買う場合の最小コスト
    int mn = *min_element(a.begin(),a.end());

    // クーポン不使用の場合の追加コストを小さい順に並べておく
    vector<int> vec(n);
    for (int i=0; i<n; i++) {
      vec.at(i) = a.at(i)-b.at(i);
    }
    sort(vec.begin(),vec.end());

    // とりあえず、全部クーポンありでの値段を加算しておく
    result = accumulate(b.begin(),b.end(),0LL);

    // 残りの買わなきゃいけない範囲
    int l = 0;
    int r = n-1;

    // 追加コストがクーポン2枚追加より安い限り、2つを抱き合わせ購入する
    while (r>=l+1&&vec.at(l)<=2*mn) {
      result += vec.at(l);
      l++;
      r--;
    }

    // 残り1個なら、そのものを買うかクーポンを1個追加か、安い方を行う
    // 残り2個以上あるなら、クーポンをその分追加する
    if (r==l) {
      result += min(vec.at(l),mn);
    } else if (r>l) {
      result += 1LL*(r-l+1)*mn;
    }

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}