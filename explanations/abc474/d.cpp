#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }
  vector<int> b(n);
  for (int i=0; i<n; i++) {
    cin >> b.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  // 可能かどうか
  bool flag = false;

  // 可能だった場合の配列
  vector<long long> result(n);

  //////////////////// 処理 ////////////////////

  // aがbより大きいなら10^18、小さいなら1とする
  // 1回でも10^18を入れたら、条件を満たす
  for (int i=0; i<n; i++) {
    if (a.at(i)>b.at(i)) {
      result.at(i) = 1000000000000000000LL;
      flag = true;
    }
    else result.at(i) = 1;
  }

  //////////////////// 出力 ////////////////////

  if (flag) {
    cout << "Yes" << endl;
    for (size_t i=0; i<result.size(); i++) {
      cout << result.at(i);
      if (i!=result.size()-1) {
        cout << " ";
      }
    }
    cout << endl;
  }
  else cout << "No" << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}