#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// 重み付きTrie木（二分木）
struct Node {
  int left = -1;
  int right = -1;
  long long end = 0;
  long long value = 0;
};
struct TrieBinary {
  char cl, cr;
  vector<Node> vec;
  TrieBinary(char cl_='0', char cr_='1') {assert(cl_<cr_); cl = cl_; cr = cr_; vec.resize(1);}
  int add_node() {vec.emplace_back(Node()); return ssize(vec)-1;}
  bool move_left(int& pos, bool make=false) {
    if (vec[pos].left==-1) {
      if (make) vec[pos].left = add_node();
      else return false;
    }
    pos = vec[pos].left;
    return true;
  }
  bool move_right(int& pos, bool make=false) {
    if (vec[pos].right==-1) {
      if (make) vec[pos].right = add_node();
      else return false;
    }
    pos = vec[pos].right;
    return true;
  }
  void add(const string& s, long long num) {
    int pos = 0;
    for (int i=0; i<ssize(s); i++) {
      vec[pos].value+=num;
      if (s.at(i)==cl) move_left(pos,true);
      else if (s.at(i)==cr) move_right(pos,true);
      else assert(false);
    }
    vec[pos].value+=num;
    vec[pos].end+=num;
  }
  long long sum(const string& s, bool tie_count=false) {
    long long result = 0;
    int pos = 0;
    bool flag = true;
    for (int i=0; i<ssize(s); i++) {
      result += vec[pos].end;
      if (s.at(i)==cl) {if (!move_left(pos)) {flag = false; break;}}
      else if (s.at(i)==cr) {
        if (vec[pos].left!=-1) result += vec[vec[pos].left].value;
        if (!move_right(pos)) {flag = false; break;}
      }
      else assert(false);
    }
    if (flag&&tie_count) result += vec[pos].end;
    return result;
  }
  long long sum_lower(const string& s) {return sum(s,false);}
  long long sum_leq(const string& s) {return sum(s,true);}
};

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int n, m, k;
  string t;
  cin >> n >> m >> k >> t;

  vector<string> s(n);
  for (int i=0; i<n; i++) {
    cin >> s.at(i);
  }

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////

  TrieBinary trie('0','1');

  // 正誤判定文字列にする（正解で'0'、不正解で'1'）
  // そのままTrie木につっこむ
  for (int i=0; i<n; i++) {
    for (int j=0; j<k; j++) {
      if (s.at(i).at(j)==t.at(j)) s.at(i).at(j) = '0';
      else s.at(i).at(j) = '1';
    }
    trie.add(s.at(i),1);
  }

  // 全問不正解の人を番兵として追加しておく
  // 全問不正解がm位判定で通過しないようにする意味がある
  trie.add(string(k,('1')),1);

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int p, j;
    cin >> p >> j;
    p--;
    j--;

    //////////////// 出力変数定義 ////////////////

    string result = "No";

    //////////////////// 処理 ////////////////////

    // 差分更新のため、元々入っていたデータを削除
    trie.add(s.at(p),-1);

    // 正誤判定を反転
    s.at(p).at(j) ^= '1'^'0';

    // 新しいデータを加える
    trie.add(s.at(p),1);

    // 自分より優秀または同じ成績の人が、m人以下なら予選突破
    if (trie.sum_leq(s.at(p))<=m) result = "Yes";

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}