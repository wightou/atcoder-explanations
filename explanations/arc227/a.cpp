#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

vector<int> calc (const string& str) {
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

  vector<int> vec_a = calc(a);
  vector<int> vec_b = calc(b);
  vector<int> vec_c = calc(c);

  for (int i=0; i<n; i++) {
    int mx = max({vec_a.at(i),vec_b.at(i),vec_c.at(i)});
    int mn = min({vec_a.at(i),vec_b.at(i),vec_c.at(i)});
    int mid = vec_a.at(i)+vec_b.at(i)+vec_c.at(i)-mn-mx;

    result += mx-mn;
    result2.at(mid) = '1';
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;
  cout << result2 << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}