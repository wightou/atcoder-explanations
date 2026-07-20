#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// k進法変換（0のときに空を返すように加工）
vector<long long> to_base (long long n,long long k) {
  assert(k>1);
  if (n==0) return {};
  assert(n>0);
  vector<long long> result;
  while (n>0) {
    result.emplace_back(n%k);
    n /= k;
  }
  return result;
}

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////



  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    long long x, y, k;
    cin >> x >> y >> k;

    //////////////// 出力変数定義 ////////////////

    int result = 0;

    //////////////////// 処理 ////////////////////

    // xとyをk進法変換する
    // その後、正順にするためにreverse
    vector<long long> x_vec = to_base(x,k);
    reverse(x_vec.begin(),x_vec.end());
    vector<long long> y_vec = to_base(y,k);
    reverse(y_vec.begin(),y_vec.end());

    // xとyと数字列が上から何個一致しているかを見る
    int d = 0;
    while (ssize(x_vec)>d&&ssize(y_vec)>d&&x_vec.at(d)==y_vec.at(d)) d++;

    // xの不一致部分を削除して、yの不一致部分を追加するのにかかる手数
    result = ssize(x_vec)+ssize(y_vec)-2*d;

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}