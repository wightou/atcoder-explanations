#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  string s;
  cin >> s;

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 書きやすさのために、入力の長さと、bitの上限を取得しておく
  int n = ssize(s);
  int m = (1<<10);

  // 入力「未満」しか求められないので、sに1を加えておく
  {
    int d = n-1;
    while (d>=0&&s.at(d)=='9') {
      s.at(d) = '0';
      d--;
    }
    if (d==-1) {
      s.at(0) = '1';
      s += '0';
      n++;
    } else {
      s.at(d)++;
    }
  }

  // その桁より下を自由に決めていい場合、どんなものがどのくらい作れるか
  // free[i][b][r] は、i桁目より下10^(n-i-1)個のうち、bで示された種類の数字だけで、%3がrになるような個数
  // ただし、先頭が0になっているものも含んでいる
  vector<vector<vector<int>>> free(n,vector(m,vector<int>(3,0)));
  free.at(n-1).at(0).at(0) = 1;
  for (int i=n-1; i>0; i--) {
    for (int b=0; b<m; b++) {
      for (int r=0; r<3; r++) {
        for (int j=0; j<10; j++) {

          // 配るDPでデータを作っていく
          free.at(i-1).at(b|(1<<j)).at((r+j)%3) += free.at(i).at(b).at(r);
          free.at(i-1).at(b|(1<<j)).at((r+j)%3) %= mod;

        }
      }
    }
  }

  // 上から見ていくときの、現状の桁数と数字の種類のbit
  int sum_d = 0;
  int bit_d = 0;

  // 上の桁から順に見ていくループ
  for (int i=0; i<n; i++) {
    int si = s.at(i)-'0';

    // その桁で許される最小の数字（先頭桁なら1、それ以外なら0）
    int left = 0;
    if (i==0) left++;

    // bとr全パターンについて調べる
    for (int b=0; b<m; b++) {
      for (int r=0; r<3; r++) {

        // i桁目まではsと同じで、i桁目がsより小さいやつを数える
        for (int j=left; j<si; j++) {

          // 数える条件を満たしているなら足す
          int tmp = b|(1<<j)|bit_d;
          int count = 0;
          if ((sum_d+j+r)%3==0) count++;
          if (tmp&(1<<3)) count++;
          if (popcount((uint)tmp)==3) count++;
          if (count==1) {
            result += free.at(i).at(b).at(r);
            result %= mod;
          }

        }

        if (i==0) continue;

        // sより桁数が短いデータについて、そこが先頭桁であるデータを同じように足す
        for (int j=1; j<10; j++) {
          int tmp = b|(1<<j);
          int count = 0;
          if ((j+r)%3==0) count++;
          if (tmp&(1<<3)) count++;
          if (popcount((uint)tmp)==3) count++;
          if (count==1) {
            result += free.at(i).at(b).at(r);
            result %= mod;
          }
        }
      }
    }

    // 次の桁に進む前に、ここまでの合計と種類を更新
    sum_d += si;
    bit_d |= (1<<si);

  }

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}