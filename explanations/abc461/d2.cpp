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

  // AVX-512による並列処理を活用する形で書いて、四重ループをCPUパワーで通す
  for (int i=0; i<=h; i++) {
    for (int j=i+1; j<=h; j++) {
      for (int l=0; l<=w; l++) {
        for (int m=l+1; m<=w; m++) {
          int num = vec[j][m];
          num -= vec[j][l];
          num -= vec[i][m];
          num += vec[i][l];
          if (num==k) result++;
        }
      }
    }
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}