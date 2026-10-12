#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<long long> s(n);
  for (int i=0; i<n; i++) {
    cin >> s.at(i);
  }
  vector<long long> t(n);
  for (int i=0; i<n; i++) {
    cin >> t.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 各人、sが小さい方から何番目かを求める
  vector<int> s_order(n);
  vector<pair<int,int>> s0(n);
  for (int i=0; i<n; i++) s0.at(i) = {s.at(i),i};
  sort(s0.begin(),s0.end());
  for (int i=0; i<n; i++) s_order.at(s0.at(i).second) = i;

  // tも同様に求める
  vector<int> t_order(n);
  vector<pair<int,int>> t0(n);
  for (int i=0; i<n; i++) t0.at(i) = {t.at(i),i};
  sort(t0.begin(),t0.end());
  for (int i=0; i<n; i++) t_order.at(t0.at(i).second) = i;

  // 各人、swap以外で何回移動するかを求めて足す
  // swapで左右にそれぞれ何回移動するかはわからないが、差分だけならsとtの順位数の差でわかる
  for (int i=0; i<n; i++) {
    int diff = t_order.at(i)-s_order.at(i);
    result += abs(s.at(i)+diff-t.at(i));
  }

  // sとtの順位を見て、swap回数すなわち転倒数を調べて加える
  vector<int> tmp(n);
  for (int i=0; i<n; i++) tmp.at(s_order.at(i)) = t_order.at(i);
  fenwick_tree<int> f(n);
  for (int i=0; i<n; i++) {
    result += f.sum(tmp.at(i),n);
    f.add(tmp.at(i),1);
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}