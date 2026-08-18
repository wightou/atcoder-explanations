#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// 1の位置を全て拾い出す関数
vector<int> pick_one (const string& str) {
  vector<int> result;
  for (int i=0; i<ssize(str); i++) {
    if (str.at(i)=='1') result.emplace_back(i);
  }
  return result;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  string a, b, c;
  cin >> n >> a >> b >> c;

  //////////////// 出力変数定義 ////////////////

  long long result = 0;
  string result2 = string(2*n,'0');

  //////////////////// 処理 ////////////////////

  // 各文字列から、1の位置を全て拾い出す
  vector<int> vec_a = pick_one(a);
  vector<int> vec_b = pick_one(b);
  vector<int> vec_c = pick_one(c);

  // 各文字列i番目の1を見ていくループ
  for (int i=0; i<n; i++) {

    // 各文字列中の位置の3つの数の、最大値、最小値、真ん中を取得
    int mx = max({vec_a.at(i),vec_b.at(i),vec_c.at(i)});
    int mn = min({vec_a.at(i),vec_b.at(i),vec_c.at(i)});
    int mid = vec_a.at(i)+vec_b.at(i)+vec_c.at(i)-mn-mx;

    // 最大値と最小値を真ん中に寄せ集める
    result += mx-mn;
    result2.at(mid) = '1';
    
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;
  cout << result2 << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}