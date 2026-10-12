#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, w;
  cin >> n >> w;

  vector<int> a(n), b(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i) >> b.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 各選び方のおいしさの合計を入れる配列
  vector<long long> sum_a(1<<n,0);

  // 各選び方の体積の合計を入れる配列
  vector<long long> sum_b(1<<n,0);

  // 最後に選ぶおかずが何か、でループを分けてデータを作る
  for (int i=0; i<n; i++) {
    int width = (1<<i);
    for (int j=0; j<width; j++) {
      sum_a.at(j+width) = sum_a.at(j)+a.at(i);
      sum_b.at(j+width) = sum_b.at(j)+b.at(i);
    }
  }

  // 体積が制限内のもので、おいしさ最大のものを探す
  for (int i=0; i<(1<<n); i++) {
    if (sum_b.at(i)<=w) result = max(result,sum_a.at(i));
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}