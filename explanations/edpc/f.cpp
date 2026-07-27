#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// 最長共通部分列
string lcs(const string& s, const string& t) {
  int m = ssize(s);
  int n = ssize(t);
  string result;
  vector<vector<int>> dp(m+1,vector<int>(n+1,0));
  for (int i=1; i<=m; i++) {
    for (int j=1; j<=n; j++) {
      if (s[i-1]==t[j-1]) {
        dp[i][j] = dp[i-1][j-1]+1;
      } else {
        dp[i][j] = max(dp[i-1][j],dp[i][j-1]);
      }
    }
  }
  int i = m;
  int j = n;
  while (i>0&&j>0) {
    if (s[i-1]==t[j-1]) {
      i--;
      j--;
      result += s[i];
    } else if (dp[i-1][j]==dp[i][j]) {
      i--;
    } else if (dp[i][j-1]==dp[i][j]) {
      j--;
    }
  }
  reverse(result.begin(),result.end());
  return result;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  string s, t;
  cin >> s >> t;

  //////////////// 出力変数定義 ////////////////

  string result = "";

  //////////////////// 処理 ////////////////////

  // ライブラリで求める
  result = lcs(s,t);

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}