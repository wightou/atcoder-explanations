#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, k, t;
  cin >> n >> k >> t;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // イベントを「時刻、種類」で記録するpriority_queue
  // 種類は、ボタンを押すなら1、ボタンの時間が切れるなら-1、時刻終了は0
  priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> que;

  // ボタンを押すイベントを入れておく
  // 時刻終了も入れて、処理の簡単さのため開始時刻の時間切れも入れておく
  for (int i : a) que.emplace(i,1);
  que.emplace(t,0);
  que.emplace(0,-1);

  // 有効ボタン数のカウンター
  // 最初に時刻0での時間切れで処理をスタートしたいので、1にしておく
  int counter = 1;

  // 時刻終了までループ
  while (true) {

    // 先頭のイベントを取り出す
    auto [time,value] = que.top();
    que.pop();

    // 時刻終了だったら調査終了
    if (value==0) break;

    if (value==1) {

      // ボタンを押す場合、カウンターを+1して、時間切れイベントを追加
      counter++;
      que.emplace(time+k,-1);

    } else if (value==-1) {

      // ボタンの時間切れの場合、カウンターを-1して、0になった場合は次のイベントまで高橋くんが押す
      counter--;
      if (counter==0) {
        int d = que.top().first-time;
        result += (d+k-1)/k;
      }

    }
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}