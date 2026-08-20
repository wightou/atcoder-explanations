#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  string s;
  cin >> n >> s;

  //////////////// 出力変数定義 ////////////////

  int result = 0;
  string result2 = "";

  //////////////////// 処理 ////////////////////

  // 文字列の最短周期を見る
  for (int l=1; l<=n; l++) {

    // 候補文字数がそもそも全文字数の約数でなければスキップ
    if (n%l) continue;

    // l文字差の位置同士が全て同じ文字の組であるか確認
    bool flag = true;
    for (int i=0; i<n-l; i++) {
      if (s.at(i)!=s.at(i+l)) flag = false;
    }

    // 最初に見つけたのが最短周期
    // その周期の繰り返し数が答えのKの値
    // 以後操作列を探すため、1周期分のみにし、その文字列で全員集合する手順を構築する
    if (flag) {
      result = n/l;
      s.erase(l);
      n = l;
      break;
    }

  }

  // 最前を行くfをn-1に、最後尾を行くbを0に配置
  int f = n-1;
  int b = 0;

  // fに牛歩をさせて、bが追いつくまで頑張る
  // 人同士の追い抜きがないので、fにbが追いつけば全員集合している
  while (f>b) {
    f++;
    char c = s.at(f%n);
    result2 += c;
    b++;
    while (s.at(b%n)!=c) b++;
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;
  cout << ssize(result2) << endl;
  cout << result2 << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}