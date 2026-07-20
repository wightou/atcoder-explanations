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
    s.at(i)--;
    t.at(i)--;
  }
  
  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 最後に退出する人の時刻を調べておく
  int t_max = *max_element(t.begin(),t.end());
  vector<int> p(t_max+2,0);

  // 各人がいつスタートの犯行なら加担できるか、の階差数列
  for (int i=0; i<n; i++) {
    if (t.at(i)-s.at(i)<d) continue;
    p.at(s.at(i))++;
    p.at(t.at(i)+1-d)--;
  }

  // 階差数列からもとの数列を復元
  for (int i=0; i<t_max+1; i++) {
    p.at(i+1) += p.at(i);
  }

  // 各時間ごとに、そのときに犯罪スタートできる犯人の組の数を求めて足す
  for (int num : p) {
    result += 1LL*num*(num-1)/2;
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}