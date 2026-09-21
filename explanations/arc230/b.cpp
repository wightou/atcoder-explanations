#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////



  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int n;
    cin >> n;

    vector<int> x(n);
    for (int i=0; i<n; i++) {
      cin >> x.at(i);
    }
    vector<int> y(n);
    for (int i=0; i<n; i++) {
      cin >> y.at(i);
    }
    vector<int> a(n);
    for (int i=0; i<n; i++) {
      cin >> a.at(i);
    }
    vector<int> b(n);
    for (int i=0; i<n; i++) {
      cin >> b.at(i);
    }

    //////////////// 出力変数定義 ////////////////

    bool flag = true;
    vector<int> result;

    //////////////////// 処理 ////////////////////

    // 上下それぞれ、各位置の「ロック解除済か、ロックをしている操作番号一覧」を管理
    vector<pair<bool,vector<int>>> upper(n,{false,{}});
    vector<pair<bool,vector<int>>> lower(n,{false,{}});

    // 各操作のロック数を一次元化して管理
    // 区間[l,r]はl*n+rに対応させて、（chmax側）は+n^2
    // 半分くらい無駄になるが、書きやすさ優先にしている
    vector<int> lock_num(n*n*2,0);

    // 全区間について調べる
    for (int l=0; l<n; l++) {
      for (int r=l; r<n; r++) {

        // 区間内を1つずつ調べる
        for (int i=l; i<=r; i++) {

          // 上側のロック関係調査
          if (b.at(i)>y.at(r-l)) {
            lock_num.at(l*n+r)++;
            lower.at(i).second.emplace_back(l*n+r);
          }

          // 下側のロック関係調査
          if (b.at(i)<x.at(r-l)) {
            lock_num.at(l*n+r+n*n)++;
            upper.at(i).second.emplace_back(l*n+r+n*n);
          }

        }

      }
    }

    // 操作予約列
    deque<int> que;

    // 最初から操作可能なものを予約列に追加
    for (int l=0; l<n; l++) {
      for (int r=l; r<n; r++) {
        if (lock_num.at(l*n+r)==0) que.emplace_back(l*n+r);
        if (lock_num.at(l*n+r+n*n)==0) que.emplace_back(l*n+r+n*n);
      }
    }
  
    // キューを見て実際にロック解除していく
    while (!que.empty()) {

      // 先頭取り出し
      int id = que.front();
      que.pop_front();

      // idを分解
      bool is_upper = true;
      if (id>=n*n) is_upper = false;
      int l = (id%(n*n))/n;
      int r = id%n;

      // 実際にこの操作が有効だったところがあったか
      bool effective = false;

      // 上下でコードをわける
      if (is_upper) {

        // 区間内を1つずつ調べる
        for (int i=l; i<=r; i++) {

          // ロック解除済、ロック解除失敗はスキップ
          if (upper.at(i).first) continue;
          if (b.at(i)<y.at(r-l)) continue;

          // ロック解除
          effective = true;
          upper.at(i).first = true;

          // 各操作のロック数更新、0になったらその操作をqueに入れる
          for (int id2 : upper.at(i).second) {
            lock_num.at(id2)--;
            if (lock_num.at(id2)==0) que.emplace_back(id2);
          }

        }

      } else {

        // 区間内を1つずつ調べる
        for (int i=l; i<=r; i++) {

          // ロック解除済、ロック解除失敗はスキップ
          if (lower.at(i).first) continue;
          if (b.at(i)>x.at(r-l)) continue;

          // ロック解除
          effective = true;
          lower.at(i).first = true;

          // 各操作のロック数更新、0になったらその操作をqueに入れる
          for (int id2 : lower.at(i).second) {
            lock_num.at(id2)--;
            if (lock_num.at(id2)==0) que.emplace_back(id2);
          }

        }

      }

      // ロック解除が1ヶ所でも行われていたなら採用
      if (effective) result.emplace_back(id);

    }


    // ロック解除が足りているか判定
    for (int i=0; i<n; i++) {
      if (a.at(i)>b.at(i)) {
        if (!upper.at(i).first) flag = false;
      } else if (a.at(i)<b.at(i)) {
        if (!lower.at(i).first) flag = false;
      }
    }

    // 逆再生で解いたので、逆順にして正規順にする
    reverse(result.begin(),result.end());

    //////////////////// 出力 ////////////////////

    if (!flag) {
      cout << -1 << endl;
    } else {
      cout << ssize(result) << endl;
      for (int id : result) {
        string s = "chmin";
        if (id>=n*n) {
          s = "chmax";
          id -= n*n;
        }
        cout << s << " " << id/n+1 << " " << id%n+1 << endl;
      }
    }

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}