#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// 1つ飛ばしで0/1反転する関数
void switch_parity(vector<int>& vec) {
  for (int i=1; i<ssize(vec); i+=2) vec.at(i) = 1-vec.at(i);
}

// ランレングス圧縮する（長さだけ返す）関数
vector<int> rle(const vector<int>& vec) {
  int n = ssize(vec);
  vector<int> result;
  int counter = 1;
  for (int i=0; i<n; i++) {
    if (i==n-1||vec.at(i)!=vec.at(i+1)) {
      result.emplace_back(counter);
      counter = 1;
    } else {
      counter++;
    }
  }
  return result;
}

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////



  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int n, m;
    cin >> n >> m;

    vector<int> a(n);
    for (int i=0; i<n; i++) {
      cin >> a.at(i);
    }
    vector<int> b(m);
    for (int i=0; i<m; i++) {
      cin >> b.at(i);
    }

    //////////////// 出力変数定義 ////////////////

    string result = "No";

    //////////////////// 処理 ////////////////////

    // 最初が異なる場合、もう無理なので出力して次へ
    if (a.at(0)!=b.at(0)) {
      cout << result << endl;
      continue;
    }
    
    // それぞれ、1つ飛ばしで0/1反転する
    switch_parity(a);
    switch_parity(b);

    // それぞれ、ランレングス圧縮（長さだけ）する
    vector<int> a_vec = rle(a);
    vector<int> b_vec = rle(b);

    // サイズが異なる場合、もう無理なので出力して次へ
    if (a_vec.size()!=b_vec.size()) {
      cout << result << endl;
      continue;
    }

    // DPテーブル
    // dp.at(i).at(0)は、i番目の01交互列境界からの増設なしでそこまで作れるか
    // dp.at(i).at(1)は、i番目の01交互列境界からの両側奇数増設でそこまで作れるか
    // dp.at(i).at(2)は、i番目の01交互列境界からの両側偶数増設でそこまで作れるか
    int len = b_vec.size();
    vector<vector<bool>> dp(len+1,vector<bool>(3,false));
    dp.at(0).at(0) = true;

    // DPする
    for (int i=0; i<len; i++) {

      // 書きやすさのため、よく見る値に別名をつけておく
      int& ai = a_vec.at(i);
      int& bi = b_vec.at(i);

      // biの方が小さい場合、どうしようもないので終了
      if (bi<ai) break;

      // aiとbiの偶奇が一致するかどうか見る
      if ((bi-ai)%2) {

        // aiとbiの偶奇が違う場合、片方は奇数増設が必要で、十分でもある
        // bi-aiが1のときだけ、奇数増設と偶数増設のペアでは不可能
        dp.at(i+1).at(0) = dp.at(i).at(1);
        if (bi-ai==1) dp.at(i+1).at(1) = dp.at(i).at(0);
        else dp.at(i+1).at(1) = (dp.at(i).at(0)||dp.at(i).at(2));
        if (bi-ai!=1) dp.at(i+1).at(2) = dp.at(i).at(1);

      } else if (bi>ai) {

        // aiとbiの偶奇が同じ（値そのものは異なる）場合、奇数増設はやるとしたら両方
        // aiが1のときだけ、増設なし同士では不可能
        // bi-aiが2のときだけ、偶数増設同士では不可能
        if (ai==1) dp.at(i+1).at(0) = dp.at(i).at(2);
        else dp.at(i+1).at(0) = (dp.at(i).at(0)||dp.at(i).at(2));
        dp.at(i+1).at(1) = dp.at(i).at(1);
        if (bi-ai==2) dp.at(i+1).at(2) = dp.at(i).at(0);
        else dp.at(i+1).at(2) = (dp.at(i).at(0)||dp.at(i).at(2));

      } else if (bi==ai) {

        // biがaiと同じ場合、増設なし同士のみ可能
        dp.at(i+1).at(0) = dp.at(i).at(0);

      }

    }

    // 最後を増設なしで終わらせられるなら "Yes"
    if (dp.at(len).at(0)) result = "Yes";

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}