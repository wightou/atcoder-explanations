#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int INF = 1001001001;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m;
  cin >> n >> m;

  vector<int> a(m), b(m);
  for (int i=0; i<m; i++) {
    cin >> a.at(i) >> b.at(i);
    a.at(i)--;
    b.at(i)--;
  }

  //////////////// 出力変数定義 ////////////////

  vector<int> result(n,-1);

  //////////////////// 処理 ////////////////////

  // 各人の第一希望得票数と、自分が第一のときの第二希望投票先一覧
  vector<int> counters(n,0);
  vector<vector<int>> second(n);
  for (int i=0; i<m; i++) {
    counters.at(a.at(i))++;
    second.at(a.at(i)).emplace_back(b.at(i));
  }

  // 自分より小さい番号での「得票数の-1倍、候補番号」の最小値
  vector<pair<int,int>> from_left(n,{INF,INF});
  for (int i=0; i<n-1; i++) from_left.at(i+1) = min(from_left.at(i),{-counters.at(i),i});

  // 自分より大きい番号での「得票数の-1倍、候補番号」の最小値
  vector<pair<int,int>> from_right(n,{INF,INF});
  for (int i=n-1; i>0; i--) from_right.at(i-1) = min(from_right.at(i),{-counters.at(i),i});

  // 自分以外の「得票数の-1倍、候補番号」の最小値を出して、候補番号だけ入れておく
  // これで、第一希望が辞退したら無効票、の場合の当選者が出る
  for (int i=0; i<n; i++) result.at(i) = min(from_left.at(i),from_right.at(i)).second;

  // 各人、自分が辞退した場合の第二希望振り分けを調査する
  for (int i=0; i<n; i++) {

    // 第二の人のカウントに足す
    for (int j : second.at(i)) {
      counters.at(j)++;
    }

    // 第二で足された全員、resultに入っている人より上位じゃないかを確認
    for (int j : second.at(i)) {
      if (counters.at(result.at(i))<counters.at(j)||(counters.at(result.at(i))==counters.at(j)&&result.at(i)>j)) result.at(i) = j;
    }

    // 第二の人に足した分を戻す
    for (int j : second.at(i)) {
      counters.at(j)--;
    }

  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i)+1 << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}