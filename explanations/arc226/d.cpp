#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int INF = 1001001001;

// queueの前からn個のうち、先頭の最小値の位置
// 全部空なら -1 を返す
int queue_min(const vector<deque<int>>& a, int n) {
  int mn = INF;
  int pos = -1;
  for (int i=0; i<n; i++) {
    if (a.at(i).empty()) continue;
    if (mn>a.at(i).front()) {
      mn = a.at(i).front();
      pos = i;
    }
  }
  return pos;
}

// queueの前からn個でマージソート実施
vector<pair<int,int>> queue_sort(vector<deque<int>>& a, int n) {

  // 今回の操作列
  vector<pair<int,int>> result;

  // キュー1の2つが逆順だったら入れ替える
  if (ssize(a.at(0))==2&&a.at(0).at(0)>a.at(0).at(1)) {
    result.emplace_back(1,1);
    swap(a.at(0).at(0),a.at(0).at(1));
  }

  // マージ結果を入れておき、後でswapする
  deque<int> next;

  // 最小値位置を取得し、nextに移動していく
  while (true) {
    int pos = queue_min(a,n);
    if (pos==-1) break;
    result.emplace_back(pos+1,n);
    next.emplace_back(a.at(pos).front());
    a.at(pos).pop_front();
  }

  // 正規のマージ位置に戻す
  swap(a.at(n-1),next);

  return result;

}

/////////////////// メイン ///////////////////

int main () {
  
  /////////////////// 前入力 ///////////////////
  
  int q;
  cin >> q;
  q *= 2;

  /////////////////// 前処理 ///////////////////

  // 5本のキューの管理
  vector<deque<int>> a(5);

  // タイプ1のクエリが来た回数
  int q1_count = 0;

  /////////////////// ループ ///////////////////

  // クエリ数だけループ
  for (int loop=0; loop<q; loop++) {
    
    int type;
    cin >> type;

    if (type==-1) return 0;
    
    if (type==1) {

      // タイプ1の場合

      int x;
      cin >> x;
      a.at(0).emplace_back(x);

      // 操作列格納用
      vector<pair<int,int>> vec;

      // タイプ1の回数が 2*7^(i-1) の倍数だったら、前から i+1 個をマージソートにかける
      q1_count++;
      if (q1_count%686==0) vec = queue_sort(a,5);
      else if (q1_count%98==0) vec = queue_sort(a,4);
      else if (q1_count%14==0) vec = queue_sort(a,3);
      else if (q1_count%2==0) vec = queue_sort(a,2);

      // 操作列を出力
      cout << ssize(vec) << endl;
      for (size_t i=0; i<vec.size(); i++) {
        cout << vec.at(i).first << " " << vec.at(i).second << '\n';
      }
      cout << flush;
      
    } else if (type==2) {

      // タイプ2の場合

      // 最小値位置取得
      int pos = queue_min(a,5);

      // 実際に削除
      a.at(pos).pop_front();

      // 出力
      cout << pos+1 << endl;

    }

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}