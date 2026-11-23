#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// ライブラリ https://github.com/wightou/library-public

// 尺取法（headベース）
// vector<int> shakutori_detail<judge,add,remove>(int n, State& state, Args&&... args)
//   各headに対し、条件を満たす最小の左端result[head]を返す（区間は[result[head],head+1)）
// long long shakutori<judge,add,remove>(int n, State& state, Args&&... args)
//   上記の左端配列を集計して返す
// 計算量：O(N×(各操作の計算量の最大値))（judge、add、removeがO(1)ならO(N)）
// ※ stateは空区間を表す状態に初期化して渡す
// ※ add(head,state,args...)は右端の要素headを追加する
// ※ judge(tail,state,args...)は現在の区間が条件を満たすときtrueを返す（stateは変更しない）
// ※ remove(tail,state,args...)は左端の要素tailを削除する
// ※ 各headで、条件を満たす左端は連続し、その最小値はheadに関して単調非減少であること
// ※ 条件を満たす非空区間が存在しない場合はresult[head]=head+1。空区間ではremoveを呼ばない
// ※ args...は3関数のstate以降の引数に渡す
// ※ 実行終了後のstateは通常、最後の有効区間を表す（空区間に戻るとは限らない）
bool judge(int, const vector<int>& counters, const string&) {
  return (counters.at(0)==0||counters.at(1)==0||counters.at(2)==0);
}
void add_head(int head, vector<int>& counters, const string& s) {
  counters.at(s.at(head)-'A')++;
}
void remove_tail(int tail, vector<int>& counters, const string& s) {
  counters.at(s.at(tail)-'A')--;
}
template<auto judge = judge, auto add = add_head, auto remove = remove_tail, class State, class... Args>
vector<int> shakutori_detail(int n, State& state, Args&&... args) {
  assert(n>=0);
  vector<int> result(n);
  for (int head=0, tail=0; head<n; head++) {
    add(head,state,args...);
    while (tail<=head&&!judge(tail,state,args...)) {
      remove(tail,state,args...);
      tail++;
    }
    result[head] = tail;
  }
  return result;
}
template<auto judge = judge, auto add = add_head, auto remove = remove_tail, class State, class... Args>
long long shakutori(int n, State& state, Args&&... args) {
  long long result = 0;
  vector<int> data = shakutori_detail<judge,add,remove>(n, state, args...);
  for (int head=0; head<n; head++) {
    // result += head+1-data[head]; // 条件を満たす非空区間の総数
    result += data[head];        // 条件を満たさない非空区間の総数
    // result = max(result,(long long)head+1-data[head]); // 条件を満たす区間の最大長
  }
  return result;
}

/////////////////// メイン ///////////////////

int main () {

  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  //////////////////// 入力 ////////////////////

  int n;
  string s;
  cin >> n >> s;

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 先頭から見ていって、初めて3種類そろう位置を求める
  // [0,l)がその区間になるようにしておく
  int l = 0;
  {
    set<char> st = {'A','B','C'};
    while (l<n&&ssize(st)>0) {
      st.erase(s.at(l));
      l++;
    }
  }

  // 末尾から見ていって、初めて3種類そろう位置を求める
  // [r,N)がその区間になるようにしておく
  int r = n;
  {
    set<char> st = {'A','B','C'};
    while (r>0&&ssize(st)>0) {
      r--;
      st.erase(s.at(r));
    }
  }

  // [l,r)の範囲を切り出す
  string t = s.substr(l,r-l);

  // 「全種入っていない区間数」の尺取り法をして外側を数える
  vector<int> state(3,0);
  result = shakutori(ssize(t),state,t);

  //////////////////// 出力 ////////////////////

  cout << result << '\n';

  //////////////////// 終了 ////////////////////

  return 0;

}