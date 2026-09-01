#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;

// 置換の分解（巡回表現）
// 今回必要なのは各巡回の長さだけではある
vector<vector<int>> cycle(const vector<int>& a) {
  vector<vector<int>> result;
  vector<bool> flags(a.size(),false);
  for (int i=0; i<ssize(a); i++) {
    if (flags.at(i)) continue;
    result.emplace_back(vector(1,i));
    flags.at(i) = true;
    while (true) {
      int j = a.at(result.back().back());
      if (i==j) break;
      result.back().emplace_back(j);
      flags.at(j) = true;
    }
  }
  return result;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;

  vector<int> p(n);
  for (int i=0; i<n; i++) {
    cin >> p.at(i);
    p.at(i)--;
  }
  vector<int> q(n);
  for (int i=0; i<n; i++) {
    cin >> q.at(i);
    q.at(i)--;
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  // 各位置にある P 側の値から Q 側の値への対応を置換と考えて、対応表を作る
  vector<int> tmp(n,-1);
  for (int i=0; i<n; i++) {
    tmp.at(p.at(i)) = q.at(i);
  }

  // その置換の巡回表現を作る
  // 今回必要なのは各巡回の長さだけではある
  vector<vector<int>> vv = cycle(tmp);

  // 以下、最終的に上にあるものは+、下にあるものは-、をつけて考えるものとする

  // DP前半（同じ値の組に+-を割り当てる2^N通りの分類）のテーブル
  // dp1[i][j] は、[+-]組がi組、[-+]組がj組ある状態になる+-割り当てパターン数
  // 巡回途中では、その巡回を[+?]から始めた前提で考える
  // firstは次に[+?]である必要があるパターン数、secondは次に[-?]である必要があるパターン数
  // 巡回の区切りでは、firstに全てのパターン数、secondは0
  vector<vector<pair<long long,long long>>> dp1(1,vector<pair<long long,long long>>(1,{1,0}));

  // 巡回を辿る順（1周したら次の巡回の先頭へ）に決めていくDP
  // cは次を辿ったら、辿ったものが何個になるかのカウンター（DPテーブルサイズ決定に使用）
  int c = 0;
  for (const vector<int>& vec : vv) {
    for (int loop=0; loop<ssize(vec); loop++) {

      // カウントを増やす
      c++;

      // 辿った後のテーブル（サイズを1大きくする）
      vector<vector<pair<long long,long long>>> next(c+1,vector<pair<long long,long long>>(c+1,{0,0}));

      if (loop<ssize(vec)-1) {

        // まだ巡回が続く場合
        // 値が入っている可能性がある範囲で配るDPをする
        for (int i=0; i<c; i++) {
          for (int j=0; i+j<c; j++) {

            next.at(i).at(j).second += dp1.at(i).at(j).first;    // そこを[++]にする
            next.at(i).at(j).first += dp1.at(i).at(j).second;    // そこを[--]にする
            next.at(i+1).at(j).first += dp1.at(i).at(j).first;   // そこを[+-]にする
            next.at(i).at(j+1).second += dp1.at(i).at(j).second; // そこを[-+]にする

          }
        }

        // 全データの剰余をとる
        for (int i=0; i<=c; i++) {
          for (int j=0; i+j<=c; j++) {
            next.at(i).at(j).first %= mod;
            next.at(i).at(j).second %= mod;
          }
        }

      } else {

        // 巡回の最後の場合
        // 値が入っている可能性がある範囲で配るDPをする
        for (int i=0; i<c; i++) {
          for (int j=0; i+j<c; j++) {
            next.at(i).at(j).first += dp1.at(i).at(j).second;    // そこを[--]にする
            next.at(i+1).at(j).first += dp1.at(i).at(j).first;   // そこを[+-]にする
          }
        }

        // この巡回セットが[-?]から始まった場合も加算する
        // 過去の対称性より、転置を足せばよい
        // ついでに全データの剰余をとる
        for (int i=0; i<=c; i++) {
          for (int j=i; i+j<=c; j++) {
            long long sum = (next.at(i).at(j).first+next.at(j).at(i).first)%mod;
            next.at(i).at(j).first = sum;
            next.at(j).at(i).first = sum;
          }
        }

      }

      // DPテーブル差し替え
      swap(dp1,next);

    }
  }

  // DP後半（1回ずつ実際に交換する）のテーブル
  // dp1のsecondは不要になるので、firstだけ取り出してデータを作り直す
  vector<vector<long long>> dp2(n+1,vector<long long>(n+1,0));
  for (int i=0; i<=c; i++) {
    for (int j=(i+n)%2; i+j<=c; j+=2) {
      dp2.at(i).at(j) = dp1.at(i).at(j).first;
    }
  }

  // 1回ずつ実際に交換するDP
  for (int loop=0; loop<m; loop++) {

    // 交換した後のテーブル
    vector<vector<long long>> next(n+1,vector<long long>(n+1,0));

    
    // 値が入っている可能性がある範囲で配るDPをする
    for (int i=0; i<=n; i++) {
      for (int j=(i+n)%2; i+j<=n; j+=2) {

        // [++][--]それぞれの個数（必ず一致する）
        int k = (n-i-j)/2;

        // iもjも不変であるパターン
        // - （[++]または[--]）と（[+-]または[-+]）の上段同士または下段同士の入れ替え 4*(i+j)*k 通り
        // - 同タイプ同士での上段同士または下段同士の入れ替え i*(i-1)+j*(j-1)+2*k*(k-1) 通り
        // - [++]または[--]での上下入れ替え 2*k 通り
        next.at(i).at(j) += (4*(i+j)*k+i*(i-1)+j*(j-1)+2*k*(k-1)+2*k)*dp2.at(i).at(j);

        // iもjも1つ減るパターン　[+-]と[-+]の上段同士または下段同士の入れ替え 2*i*j 通り
        if (i>0&&j>0) next.at(i-1).at(j-1) += 2*i*j*dp2.at(i).at(j);

        // iもjも1つ増えるパターン　[++]と[--]の上段同士または下段同士の入れ替え 2*k^2 通り
        if (k>0) next.at(i+1).at(j+1) += 2*k*k*dp2.at(i).at(j);

        // iが1つ減ってjが1つ増えるパターン　[+-]の上下入れ替え
        if (i>0) next.at(i-1).at(j+1) += i*dp2.at(i).at(j);

        // iが1つ増えてjが1つ減るパターン　[-+]の上下入れ替え
        if (j>0) next.at(i+1).at(j-1) += j*dp2.at(i).at(j);
      }
    }

    
    // 全データの剰余をとる
    for (int i=0; i<=n; i++) {
      for (int j=(i+n)%2; i+j<=n; j+=2) {
        next.at(i).at(j) %= mod;
      }
    }

    // DPテーブル差し替え
    swap(dp2,next);

  }

  // 最終的に全て[+-]になっている個数が答え
  result = dp2.at(n).at(0);

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}