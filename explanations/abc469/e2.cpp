#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr long long INF = 1001001001001001001LL;

// [0,1] をこれだけの個数に区切って考える
constexpr long long G = 10000000;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, k;
  string s;
  cin >> n >> k >> s;

  //////////////// 出力変数定義 ////////////////

  double result = 0;

  //////////////////// 処理 ////////////////////

  // 各試合数までの'o'の個数を数える
  vector<long long> base(n+1,0);
  for (int i=0; i<n; i++) {
    base.at(i+1) = base.at(i);
    if (s.at(i)=='o') base.at(i+1)++;
  }

  // 二分探索する
  long long inside = -1;
  long long outside = G+1;
  while (abs(inside-outside)>1) {

    long long mid = inside+(outside-inside)/2;

    bool judge = false;

    // tmpをG倍して傾きmid=目標勝率*Gを引いたデータにする
    // （試合数,勝利数*G-(目標勝率*G)*試合数）という点をプロットするイメージ
    // 2点間の傾きが0以上なら、その目標勝率は達成されている
    vector<long long> tmp = base;
    for (int i=0; i<=n; i++) {
      tmp.at(i) *= G;
      tmp.at(i) -= i*mid;
    }

    // 尺取法で「rの示す位置とbaseの値がk以上違う」範囲に逆転値があるか調べる
    // 逆転値すなわちrの示す位置が該当範囲の値を超える組が1つでもあれば、そのmidは取れる
    long long mn = INF;
    for (int l=0, r=0; r<=n; r++) {
      while (base.at(r)-base.at(l)>=k) {
        mn = min(mn,tmp.at(l));
        l++;
      }
      if (tmp.at(r)>=mn) judge = true;
    }

    if (judge) {
      inside = mid;
    } else {
      outside = mid;
    }

  }

  // 答えはinsideをGで割った値
  result = 1.0*inside/G;

  //////////////////// 出力 ////////////////////

  cout << fixed << setprecision(7) << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}