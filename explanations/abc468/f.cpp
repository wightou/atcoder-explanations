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

  vector<int> p(n);
  for (int i=0; i<n; i++) {
    cin >> p.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // それまでで最大のやつは、全部xの記録更新に使う
  // 使ったやつは、後の処理のためにn+1に書き換えておく
  {
    int mx = -1;
    for (int& i : p) {
      if (i>mx) {
        result++;
        mx = i;
        i = n+1;
      }
    }
  }

  // 最後に n+1 を付け足す
  // これで、最長増加列の最後がどこかの n+1 になっているはず
  p.emplace_back(n+1);

  // 残ったうち最長増加部分列（最後の n+1 を除く）を y の更新に使う
  // その残りは x に食わせる（最大更新が起こらない）
  result += ssize(lis(p))-1;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}