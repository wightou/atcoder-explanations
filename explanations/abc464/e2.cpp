// 左と上も可変な場合に対応したアルゴリズムで解く
// （本当にそうするには、入力部分とque.emplace()を書き直す必要あり）

#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int LEFT = 0;
constexpr int UP = 0;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int h, w, q;
  cin >> h >> w >> q;

  vector<int> r(q), c(q);
  vector<char> x(q);
  for (int i=0; i<q; i++) {
    cin >> r.at(i) >> c.at(i) >> x.at(i);
    r.at(i)--;  // 0-indexed
    c.at(i)--;
  }

  //////////////// 出力変数定義 ////////////////

  // 全てのマスを'A'で埋めて初期化しておく
  vector<string> result(h,string(w,'A'));

  //////////////////// 処理 ////////////////////

  // 上書き作業を「上端、下端、左端、右端、スタンプ」の小さい順で、上端が早い順に取り出せるようにしておく
  // （今回は上端が0固定なのであまり意味はないが……）
  priority_queue<tuple<int,int,int,int,int>,vector<tuple<int,int,int,int,int>>,greater<tuple<int,int,int,int,int>>> que;
  for (int i=0; i<q; i++) {
    que.emplace(UP,r.at(i),LEFT,c.at(i),i);
  }

  // 番兵を入れておく
  que.emplace(h,h,w,w,-1);

  // segment木もどき
  // それぞれにpriority_queueを持たせて、その中身は「スタンプ、下端」の大きい順、prodはなし
  int len = 1;
  while (len<w) len <<= 1;
  vector<priority_queue<pair<int,int>>> seg(2*len);

  // 1行ずつ作っていく
  for (int i=0; i<h; i++) {

    // その行から始まる上書きを該当範囲に追加する
    // 原理は遅延segment木のapplyの仕方と同じ
    while (get<0>(que.top())==i) {
      auto [up,down,left,right,stamp] = que.top();
      que.pop();
      left += len;
      right += len+1;
      while (left!=right) {
        if (left&1) {
          seg.at(left).emplace(stamp,down);
          left++;
        }
        if (right&1) {
          right--;
          seg.at(right).emplace(stamp,down);
        }
        left >>= 1;
        right >>= 1;
      }
    }

    // その行の内容を作る
    // 原理は遅延segment木の一点更新の仕方と同じ
    for (int j=0; j<w; j++) {
      int latest = -1;
      for (int idx = j+len; idx>0; idx>>=1) {
        if (!seg.at(idx).empty()) latest = max(latest,seg.at(idx).top().first);
      }
      if (latest>=0) result.at(i).at(j) = x.at(latest);
    }

    // その行以前で終わる上書きを除去
    for (auto& pq : seg) {
      while (!pq.empty()&&pq.top().second<=i) pq.pop();
    }

  }


  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}