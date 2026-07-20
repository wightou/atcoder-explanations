#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, d;
  cin >> n >> d;

  vector<int> s(n), t(n);
  for (int i=0; i<n; i++) {
    cin >> s.at(i) >> t.at(i);
  }
  
  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 入館時刻を速い順に管理するpriority_queue
  priority_queue<int,vector<int>,greater<int>> que1;

  // 退館時刻-(d-1)を速い順に管理するpriority_queue
  // つまり、「これ以後スタートの犯罪には加担できない」時刻
  priority_queue<int,vector<int>,greater<int>> que2;

  // d時間以上いた人それぞれ、2つのpriority_queueにデータを入れる
  for (int i=0; i<n; i++) {
    if (s.at(i)+d<=t.at(i)) {
      que1.emplace(s.at(i));
      que2.emplace(t.at(i)-d+1);
    }
  }

  // 時刻を1ずつ進めるループ
  for (int time=0,counter=0; !que2.empty(); time++) {

    // 今来た人数を加算
    while (!que1.empty()&&que1.top()==time) {
      que1.pop();
      counter++;
    }

    // 帰り時間が近くて加担できない人を減算
    while (!que2.empty()&&que2.top()==time) {
      que2.pop();
      counter--;
    }

    // 今スタートの犯罪を行える組の数を求めて足す
    result += 1LL*counter*(counter-1)/2;

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}