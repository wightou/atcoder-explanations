#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// m+1番目を常に取れるようにする構造体
struct Data {

  // 左に残す個数
  int m;

  // 左側のデータ、delは本来消えているはずのやつ
  priority_queue<string> left;
  priority_queue<string> left_del;

  // 右側のデータ、delは本来消えているはずのやつ
  priority_queue<string,vector<string>,greater<string>> right;
  priority_queue<string,vector<string>,greater<string>> right_del;

  // 初期化
  Data (int m_) {
    m = m_;
  }

  // 左側の先頭が削除済要素だったら消す
  void delete_left() {
    while (!empty(left_del)&&left_del.top()==left.top()) {
      left_del.pop();
      left.pop();
    }
  }

  // 右側の先頭が削除済要素だったら消す
  void delete_right() {
    while (!empty(right_del)&&right_del.top()==right.top()) {
      right_del.pop();
      right.pop();
    }
  }

  // 左右のバランスを取る
  void balance() {
    while (ssize(left)-ssize(left_del)>m) {
      right.emplace(left.top());
      left.pop();
      delete_left();
    }
    while (ssize(left)-ssize(left_del)<m) {
      left.emplace(right.top());
      right.pop();
      delete_right();
    }
  }

  // 文字列挿入
  void emplace(const string& str) {
    if (!left.empty()&&str<=left.top()) {
      left.emplace(str);
    } else {
      right.emplace(str);
    }
  }

  // 文字列削除
  void erase(const string& str) {
    if (!left.empty()&&str<=left.top()) {
      left_del.emplace(str);
      delete_left();
    } else {
      right_del.emplace(str);
      delete_right();
    }
  }

  // m+1番目の値を返す
  string get() {
    balance();
    return right.top();
  }

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

  // 辞書順m+1番目を返してくれる構造体
  Data d(m);

  // 正誤判定文字列にする（正解で'0'、不正解で'1'）
  // そのまま構造体につっこむ
  for (int i=0; i<n; i++) {
    for (int j=0; j<k; j++) {
      if (s.at(i).at(j)==t.at(j)) s.at(i).at(j) = '0';
      else s.at(i).at(j) = '1';
    }
    d.emplace(s.at(i));
  }

  // 全問不正解の人を番兵として追加しておく
  // m+1番目がはみ出さない意味と、全問不正解がm位判定で通過しないようにする意味がある
  d.emplace(string(k,('1')));

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
    d.erase(s.at(p));

    // 正誤判定を反転
    s.at(p).at(j) ^= '1'^'0';

    // 新しいデータを加える
    d.emplace(s.at(p));

    // m+1番目の人よりも辞書順で真に前なら予選突破
    if (s.at(p)<d.get()) result = "Yes";

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}