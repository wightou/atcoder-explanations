#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr long long INF = 1001001001001001001LL;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> a(n), b(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i) >> b.at(i);
  }

  int q;
  cin >> q;

  vector<int> x(q);
  for (int i=0; i<q; i++) {
    cin >> x.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  vector<long long> result(q);

  //////////////////// 処理 ////////////////////

  // xの最大値+1
  int m = *max_element(x.begin(),x.end())+1;

  // 定数倍削減（最悪ケースで2.75億ループ）にするためのパラメータ √(2m) 付近の2の冪乗
  int t = 1;
  while (t*t<2*n) t <<= 1;

  // 定数による底上げ値（負の数だと下がるが……）の集計
  long long boost = 0;

  // 最頻値処理による、実質個数の変化分
  long long num = 1LL*n*t;

  // B<2t であるデータの集約
  vector<vector<long long>> smaller(2*t);
  for (int i=0; i<2*t; i++) {
    smaller.at(i).assign(i,0);
  }

  // B>=2t であるデータの集約
  vector<pair<int,int>> bigger;

  // AとBのデータをt倍して入れる（答えも一旦t倍で出す）
  for (int i=0; i<n; i++) {
    if (b.at(i)<2*t) smaller.at(b.at(i)).at(a.at(i)) += t;
    else bigger.emplace_back(a.at(i),b.at(i));
  }

  // B<2tであるものを、出現数最頻値が0になるように調整
  // B<tであるものはさらに、(A,B)×2 → (A,2B) + (A+B,2B) - Bの定数処理 に分解
  for (int i=1; i<2*t; i++) {

    // 最頻値を調査
    long long mode = -1;
    {
      vector<long long> tmp = smaller.at(i);
      sort(tmp.begin(),tmp.end());
      tmp.emplace_back(INF);
      int mx = -1;
      int counter = 0;
      for (int j=0; j<i; j++) {
        if (tmp.at(j)==tmp.at(j+1)) counter++;
        else {
          if (mx<counter) {
            mode = tmp.at(j);
            mx = counter;
          }
          counter = 0;
        }
      }
    }

    // 最頻値が0になるように、全てのAをmode個ずつ集約する
    for (int j=0; j<i; j++) {
      smaller.at(i).at(j) -= mode;
    }
    num -= i*mode;
    boost += mode*i*(i-1)/2;

    // 以下はB<tのときだけ
    if (i>=t) continue;

    // (A,B)×2 → (A,2B) + (A+B,2B) - Bの定数処理 に分解
    for (int j=0; j<i; j++) {
      smaller.at(2*i).at(j) += smaller.at(i).at(j)/2;
      smaller.at(2*i).at(j+i) += smaller.at(i).at(j)/2;
      boost -= i*smaller.at(i).at(j)/2;
    }

  }

  // メモ配列
  vector<long long> vec(m,0);

  // 階差数列を作っていく、ついでに初項分をboostに足していく
  for (int i=t; i<2*t; i++) {
    for (int j=0; j<i; j++) {
      if (smaller.at(i).at(j)!=0) {
        boost += j*smaller.at(i).at(j);
        long long val = i*smaller.at(i).at(j);
        for (int k=i-j; k<m; k+=i) {
          vec.at(k) -= val;
        } 
      }
    }
  }
  for (auto [j,i] : bigger) {
    boost += j*t;
    for (int k=i-j; k<m; k+=i) {
      vec.at(k) -= i*t;
    } 
  }

  // 階差数列を本来の値に戻す
  for (int i=1; i<m; i++) {
    vec.at(i) += vec.at(i-1);
  }
  
  // さらに、累積しない処理も足す
  for (int i=0; i<m; i++) {
    vec.at(i) += 1LL*i*num+boost;
  }

  // 各クエリを答えの値に変換
  for (int i=0; i<q; i++) {
    result.at(i) = vec.at(x.at(i))/t;
  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}