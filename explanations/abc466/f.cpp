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
    long long x;
    cin >> n >> x;

    vector<long long> a(n);
    for (int i=0; i<n; i++) {
      cin >> a.at(i);
    }

    //////////////// 出力変数定義 ////////////////

    long long result = 0;

    //////////////////// 処理 ////////////////////

    // 余りとして、「0以上x未満がy個ずつある」を(x,y)で表して管理するpriority_queue
    // xの値が大きい順に取り出せるようにしておく
    priority_queue<pair<long long,long long>> que;

    // 「1以上x以下」を入れたいが無理なので、
    // 「0以上x+1未満が1つずつ」「0以上1未満が-1つずつ」で入れる
    // 「0以上1未満が-1つずつ」は番兵を兼ねる（1はaの中身より大きくなることがない）
    que.emplace(x+1,1);
    que.emplace(1,-1);

    // aの中身を前から順に見る
    for (long long i : a) {

      // 投入予定の「0以上i未満」の区間の個数
      long long full = 0;

      // 範囲上限がiを超えているものがなくなるまで続けるループ
      while (que.top().first>i) {

        // 先頭および、それと同じ範囲のものをまとめて取り出す
        long long len = que.top().first;
        long long num = 0;
        while (que.top().first==len) {
          num += que.top().second;
          que.pop();
        }

        // 「0以上i未満」の区間が何個できるか求めて加えておく
        full += (len/i)*num;

        // 端数分の区間ができるので、それをpriority_queueに戻す
        long long r = len%i;
        if (r>0) que.emplace(r,num);

      }

      // 「0以上i未満」の区間をまとめて投入
      que.emplace(i,full);

    }

    // 全ての区間に0が1つずついるはずなので、区間数の合計が答え
    while (!que.empty()) {
      result += que.top().second;
      que.pop();
    }

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}