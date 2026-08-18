#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// ソート用比較関数
// $2$ つの文字の連結で辞書順が遅くなる方の、前にいる側が「小さい」とする
// これにより、「小さい」順にならべると、連結時の辞書順最大になる
bool compare(const string& a, const string& b) {
  return a+b>b+a;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, k;
  cin >> n >> k;

  vector<string> s(n);
  for (int i=0; i<n; i++) {
    cin >> s.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  // 2パターンのどちらが最善か事前に判断するのが難しい
  // よって、両方の結果を用意してみて、より良い方を解答する
  string result1 = "";
  string result2 = "";

  //////////////////// 処理 ////////////////////

  // 文字列が長い方から取り出すpriority_queueに「文字数、文字列」で全部入れる
  // タイブレークは文字列の辞書順で、これは長さが同じなら数値の大きさ順でもある
  priority_queue<pair<int,string>> tmp;
  for (string& str : s) {
    tmp.emplace(ssize(str),str);
  }

  // 数値が全部0だったら答えは0、を先に処理する
  {
    bool flag = false;
    for (const string& str : s) {
      if (stoll(str)>0) flag = true; 
    }
    if (!flag) {
      cout << 0 << endl;
      return 0;
    }
  }

  // 長い方から K-1 個を採用
  // 最後の1個はまだ何を採用するか決めない
  vector<string> vec1;
  while (ssize(vec1)<k-1) {
    vec1.emplace_back(tmp.top().second);
    tmp.pop();
  }

  // 全部文字列前後順でソートして、2つに複製しておく
  sort(vec1.begin(),vec1.end(),compare);
  vector<string> vec2 = vec1;

  // 片方には、長い方から K 番目を採用
  vec1.emplace_back(tmp.top().second);

  // 片方には、長い方から K 番目以下で最も数値として大きいものを採用
  string mx_str = "";
  long long mx = -1;
  while (!tmp.empty()) {
    long long num = stoll(tmp.top().second);
    if (mx<num) {
      mx = num;
      mx_str = tmp.top().second;
    }
    tmp.pop();
  }
  vec2.emplace_back(mx_str);

  // 両者、最後以外はソート済なので、最後分だけ挿入ソートする
  for (int i=k-1; i>0; i--) {
    if (compare(vec1.at(i),vec1.at(i-1))) swap(vec1.at(i-1),vec1.at(i));
    else break;
  }
  for (int i=k-1; i>0; i--) {
    if (compare(vec2.at(i),vec2.at(i-1))) swap(vec2.at(i-1),vec2.at(i));
    else break;
  }

  // それぞれ、実際に答えの文字列を作ってみる
  // 先頭には数値が0でないものが来ているはず
  //（または、前者が全て0だった場合、そちらが短くまたは辞書順で後者になるはず）
  for (int i=0; i<k; i++) {
    if (i==0) result1 += to_string(stoll(vec1.at(0)));
    else result1 += vec1.at(i);
  }
  for (int i=0; i<k; i++) {
    if (i==0) result2 += to_string(stoll(vec2.at(0)));
    else result2 += vec2.at(i);
  }

  //////////////////// 出力 ////////////////////

  // 長い方（タイブレーク辞書順で大きい方）を答える
  if (result1.size()>result2.size()||result1.size()==result2.size()&&result1>result2) cout << result1 << endl;
  else cout << result2 << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}