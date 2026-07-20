#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, k, m;
  cin >> n >> k >> m;

  vector<int> c(n), v(n);
  for (int i=0; i<n; i++) {
    cin >> c.at(i) >> v.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 宝石の価値が高い順に「価値、色」を取り出すpriority_queue
  priority_queue<pair<int,int>> que;
  for (int i=0; i<n; i++) {
    que.emplace(v.at(i),c.at(i));
  }

  // 一度でも選んだことがある色の一覧
  set<int> color_set;

  // 色の重複が何回許されるか
  int allowed = k-m;

  // 選んだ宝石の個数
  int counter = 0;

  // 選んだ個数がk個になるまでループ
  while (counter<k) {

    // 残りのうち価値が高いものを取り出す
    auto [value,color] = que.top();
    que.pop();

    // 新出の色か、既出だけど色重複が許される場合は採用する
    if (!color_set.contains(color)||allowed>0) {
      result += value;
      if (color_set.contains(color)) allowed--;
      else color_set.emplace(color);
      counter++;
    }

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}