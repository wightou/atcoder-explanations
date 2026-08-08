#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, l;
  cin >> n >> l;

  vector<int> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  double result = 0;

  //////////////////// 処理 ////////////////////

  // DPテーブル
  // dp[i][j][k]は、残りライフi、残りペア数j、片方見えているペア数k、という状態での期待値
  vector dp(l+1,vector(n+1,vector<double>(n+1,0.0)));

  // DPテーブル埋め
  // kは増加する可能性があるが、その場合iが必ず減少するので、一番内側で回す
  for (int i=1; i<=l; i++) {
    for (int j=1; j<=n; j++) {
      for (int k=0; k<=j; k++) {

        // 1枚目が既知のカードの相方だった場合の寄与
        double e1 = 0;
        if (k>0) e1 = (dp.at(i).at(j-1).at(k-1)+1)*k/(2*j-k);
        
        // 1枚目と2枚目が偶然ペアだった場合の寄与
        double e2 = 0;
        if (j-k>=1) e2 = (dp.at(i).at(j-1).at(k)+1)*2*(j-k)/(2*j-k)/(2*j-k-1);

        // 1枚目は未知で、2枚目が既知のカードの相方だった場合の寄与
        // ライフが1減った後でまだ行動できる場合だけ
        double e3 = 0;
        if (i>1&&j-k>=1) e3 = (dp.at(i-1).at(j-1).at(k)+1)*2*(j-k)*k/(2*j-k)/(2*j-k-1);

        // 1枚目も2枚目も未知のカードだった場合の寄与
        double e4 = 0;
        if (k<=j-2) e4 = dp.at(i-1).at(j).at(k+2)*4*(j-k)*(j-k-1)/(2*j-k)/(2*j-k-1);
        
        // 4つの寄与を合計する
        dp.at(i).at(j).at(k) = e1+e2+e3+e4;

      }
    }
  }

  // 残りライフl、残りペア数n、片方見えているペア数0、の場合の値が枚数期待値
  // それにカードの値の平均値をかければ答え
  result = dp.at(l).at(n).at(0)/n*accumulate(a.begin(),a.end(),0);

  //////////////////// 出力 ////////////////////

  cout << fixed << setprecision(10) << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}