#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// 最長増加部分列
vector<int> lis(const vector<int>& original) {
  int n = original.size();
  if (n==0) return {};
  vector<int> len(n);
  vector<int> vec;
  vec.reserve(n);
  for (int i=0; i<n; i++) {
    len[i] = distance(vec.begin(),lower_bound(vec.begin(),vec.end(),original[i]));
    if (len[i]==(int)vec.size()) vec.emplace_back(original[i]);
    else vec[len[i]] = original[i];
  }
  vector<int> result(vec.size());
  int idx = vec.size()-1;
  for (int i=n-1; i>=0; i--) {
    if (len[i]==idx) {
      result[idx] = original[i];
      idx--;
    }
  }
  return result;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> x(n), r(n);
  for (int i=0; i<n; i++) {
    cin >> x.at(i) >> r.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 左端が大きい順でソートする
  // 同じなら右端が大きい順にしておくことで、左端が同じものの同時採用を防ぐ
  vector<pair<int,int>> tmp(n);
  for (int i=0; i<n; i++) {
    tmp.at(i) = {x.at(i)-r.at(i),x.at(i)+r.at(i)};
  }
  sort(tmp.rbegin(),tmp.rend());

  // ターゲットに共存できるのは、右端が狭義に昇順になっている組だけ
  // つまり最長増加部分列の長さが答え
  vector<int> vec(n,0);
  for (int i=0; i<n; i++) {
    vec.at(i) = tmp.at(i).second;
  }

  result = ssize(lis(vec));

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}