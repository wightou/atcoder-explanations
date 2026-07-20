#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;

  vector<int> a(n), d(n), b(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i) >> d.at(i) >> b.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  vector<int> result(m);

  //////////////////// 処理 ////////////////////

  // 「時刻、鳥の番号」をpriority_queueに入れる（vectorに入れてソートでもよい）
  priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> que;

  // 各色の鳥が何羽いるかと、1種類以上いる色の数
  vector<int> counters(n+1,0);
  int kind = 0;

  // 番兵を入れておく
  que.emplace(m+1,-1);

  // 初期状態で、1羽ずつ見て、各色の鳥の数を記録する
  // 0から増えるときだけ、kindを1増やす
  for (int i=0; i<n; i++) {
    if (counters.at(a.at(i))==0) kind++;
    counters.at(a.at(i))++;
    que.emplace(d.at(i),i);
  }

  // 1日ごと処理するループ
  for (int j=0; j<m; j++) {

    // priority_queueを見て、その時刻に色変する鳥がいたら処理するループ
    while (que.top().first==j+1) {

      // 元の色の鳥の数を1減らし、新しい色の鳥の数を1増やす
      // 0に減ったり0から増えたら、kindもそれに従って処理
      int k = que.top().second;
      que.pop();
      counters.at(a.at(k))--;
      if (counters.at(a.at(k))==0) kind--;
      if (counters.at(b.at(k))==0) kind++;
      counters.at(b.at(k))++;
    }

    // 終わったら、結果を記録
    result.at(j) = kind;

  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}