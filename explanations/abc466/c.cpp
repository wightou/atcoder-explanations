#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// ジャッジへの問い合わせ
bool ask(int l, int r) {

  // 問い合わせ違反は、実際に問い合わせをせずにtrueを返す
  // l==rのケースは実際にtrueとして扱う
  if (l>=r) return true;

  // 問い合わせ
  cout << "? " << l << " " << r << endl;
  string tmp;
  cin >> tmp;

  // boolに直して返り値とする
  if (tmp=="Yes") return true;
  else return false;

}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 尺取り法で、rの値ごとに対応できるlがいくつあるかを数える
  for (int l=1,r=1; r<=n; r++) {
    while (!ask(l,r)) l++;
    result += r-l;
  }

  //////////////////// 出力 ////////////////////

  cout << "! " << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}