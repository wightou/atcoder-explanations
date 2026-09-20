#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }
  vector<int> b(n);
  for (int i=0; i<n; i++) {
    cin >> b.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // とりあえず各マスの人口を求めておく
  vector<vector<long long>> c(n,vector<long long>(n));
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      c.at(i).at(j) = 1LL*a.at(i)*b.at(j)%m;
    }
  }

  // 二次元imos法をやるための二次元配列
  vector<vector<long long>> mat(n,vector<long long>(n));

  // (i,j)にいる人口Cからの寄与は、以下
  // ただし、添字が負になるところは0扱い、添字がn以上にものは無視してよい
  // - (-INF,-INF) に +n*C
  // - (i-n,j-n)から(i+n-1,j+n-1)まで、＼向きの斜めに-Cずつ
  // - (i+n-1,j-n)から(i-n,j+n-1)まで、／向きの斜めに+Cずつ
  // これを愚直にやると間に合わないので、斜め方向にまとめて処理することで高速化

  // まず、以下の2つ
  // - (-INF,-INF) に +n*C
  // - (i-n,j-n)から(i+n-1,j+n-1)まで、＼向きの斜めに-Cずつ
  // i-jの値ごとに処理する
  for (int k=-n+1; k<=n-1; k++) {

    // その列の各マスから引くべき値
    long long sum1 = 0;

    // (0,0)に圧縮される値、つまり添字が両方0以下になってしまう範囲の値の合計
    long long sum2 = 0;

    // ライン上の人口を合計
    for (int i=max(0,k); i<min(n,n+k); i++) {
      sum1 += c.at(i-k).at(i);
      sum2 += max(i,i-k)*c.at(i-k).at(i);
    }

    // (0,0)への反映
    mat.at(0).at(0) += sum2;

    // 範囲内のライン上の各マスへの加算
    // (0,0)だけは、sum2の方にすでに反映されているのでスキップ
    for (int i=max(0,k); i<min(n,n+k); i++) {
      if (k!=0||i!=0) mat.at(i-k).at(i) -= sum1;
    }

    // 範囲外にあるけど、(0,0) になるわけではない分の反映
    if (k>0) {
      for (int i=1; i<k; i++) {
        mat.at(0).at(i) -= sum1;
      }
    } else if (k<0) {
      for (int i=1; i<-k; i++) {
        mat.at(i).at(0) -= sum1;
      }
    }
  }

  // 続いて、以下の処理
  // - (i+n-1,j-n)から(i-n,j+n-1)まで、／向きの斜めに+Cずつ
  for (int k=0; k<=2*n-3; k++) {

    // その列の各マスに足すべき値
    long long sum1 = 0;
    
    // ライン上の人口を合計
    for (int i=max(0,k-n+1); i<min(n,k+1); i++) {
      sum1 += c.at(k-i).at(i);
    }

    // 範囲内のライン上の各マスへの加算
    for (int i=max(0,k-n+2); i<min(n,k+2); i++) {
      mat.at(k-i+1).at(i) += sum1;
    }

    // 範囲外にある分の反映
    for (int i=k+2; i<n; i++) {
      mat.at(0).at(i) += sum1;
      mat.at(i).at(0) += sum1;
    }

  }

  // 二次元累積和を取って、f(i,j)の値にする
  for (int i=0; i<n; i++) {
    for (int j=0; j<n-1; j++) {
      mat.at(i).at(j+1) += mat.at(i).at(j);
    }
  }
  for (int i=0; i<n-1; i++) {
    for (int j=0; j<n; j++) {
      mat.at(i+1).at(j) += mat.at(i).at(j);
    }
  }

  // 指示通りに、xorを取る
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      result ^= mat.at(i).at(j)+i*n+j;
    }
  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}