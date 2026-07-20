#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int h, w, k;
  cin >> h >> w >> k;

  vector<string> s(h);
  for (int i=0; i<h; i++) {
    cin >> s.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 二次元累積和を作る
  vector<vector<int>> vec(h+1,vector<int>(w+1,0));
  for (int i=0; i<h; i++) {
    for (int j=0; j<w; j++) {
      if (s.at(i).at(j)=='1') vec.at(i+1).at(j+1) = 1;
    }
  }
  for (int i=0; i<=h; i++) {
    for (int j=0; j<w; j++) {
      vec.at(i).at(j+1) += vec.at(i).at(j);
    }
  }
  for (int i=0; i<h; i++) {
    for (int j=0; j<=w; j++) {
      vec.at(i+1).at(j) += vec.at(i).at(j);
    }
  }

  // 2つの行の組を選ぶ
  for (int i=0; i<=h; i++) {
    for (int j=i+1; j<=h; j++) {

      // vectorとして下-上をする、選んだ行の間で各列に何個1があるかの情報になる
      vector<int> diff(w+1,0);
      for (int c=0; c<=w; c++) {
        diff.at(c) = vec.at(j).at(c)-vec.at(i).at(c);
      }

      // 右端-kちょうどの個数を尺取法で数える
      // それ「以下」と「未満」を同時に走らせることで、ちょうどの個数を拾う
      for (int l1=0,l2=0,r=0; r<=w; r++) {
        while (diff.at(l1)<diff.at(r)-k) l1++;
        while (l2<r&&diff.at(l2)<=diff.at(r)-k) l2++;
        result += l2-l1;
      }
    }
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}