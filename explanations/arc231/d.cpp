#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {

  // nとkを受け取る
  int n, k;
  cin >> n >> k;

  // 後攻を宣言
  cout << "Second" << endl;

  // 各山を実際に用意し、石に番号をつける
  // i番の山の石には、1番からN番のうちi番だけスキップ
  vector<set<int>> vec(n+1);
  for (int i=1; i<=n; i++) {
    for (int j=1; j<=n; j++) {
      if (i==j) continue;
      vec.at(i).emplace(j);
    }
  }

  // 入力受け取り用変数
  int i, j;

  // ゲームを進行する
  while (true) {

    // ジャッジの手を受け取る
    cin >> i >> j;
    if (i<=0) return 0;

    // 自分の応手
    vector<int> result;

    // ジャッジの手は番号の若い石から取ったと決めつける
    // 山iの石jを取られたら、山jの石iを取る
    for (int l=0; l<j; l++) {
      result.emplace_back(*vec.at(i).begin());
      vec.at(*vec.at(i).begin()).erase(i);
      vec.at(i).erase(vec.at(i).begin());
    }

    // こちらの手を出力
    cout << j << endl;
    for (size_t i=0; i<result.size(); i++) {
      cout << result.at(i);
      if (i!=result.size()-1) {
        cout << " ";
      }
    }
    cout << endl;

  }

  return 0;

}