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

    int n, k;
    cin >> n >> k;

    //////////////// 出力変数定義 ////////////////

    string result = "";

    //////////////////// 処理 ////////////////////

    // 以下の条件を全て満たすことが必要十分条件
    // - nが奇数
    // - kが偶数
    // - n-1<=k<=(n^2-1)/2

    // 条件を満たさない場合は "No" として次へ
    if (n%2==0||k%2==1||k<n-1||2*k>n*n-1) {
      cout << "No" << endl;
      continue;
    }

    // 最終手順で右移動をn-1回するので、その分を予約しておく
    k -= n-1;

    // 左端の残り段数
    int remain = n-1;

    // 右移動回数の残りが1行分未満になるまで左右蛇行で埋める
    while (k>=n-1) {
      result += string(n-1,'R');
      result += 'D';
      result += string(n-1,'L');
      result += 'D';
      k -= n-1;
      remain -= 2;
    }

    // 余った端数分も途中まで行って折り返す
    if (k>0) {
      result += string(k,'R');
      result += 'D';
      result += string(k,'L');
      result += 'D';
      remain -=2;
    }

    // 左下隅まで移動
    result += string(remain,'D');

    // 余った部分を上下蛇行（右移動はn-1回）
    for (int i=0; 2*i<n-1; i++) {
      if (k>0&&2*i==k) remain += 2;
      result += 'R';
      result += string(remain,'U');
      result += 'R';
      result += string(remain,'D');

    }

    //////////////////// 出力 ////////////////////

    cout << "Yes" << endl;
    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}