#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int n;
  cin >> n;

  vector<string> s(n);
  vector<int> v(n);
  for (int i=0; i<n; i++) {
    cin >> s.at(i) >> v.at(i);
  }

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////

  // 各ドリンク情報を六次元配列の中に記録
  // 累積和を取りたいので各方向のサイズを11にすると楽だが、どんな方法でも何かが犠牲になる
  // 以下の4択になるが、今回は最後の方針を採用
  // - 11進法で6桁の数に圧縮する：毎回圧縮と解凍の処理が手間だし、バグったときに確認しづらい
  // - 10進法で累積和の方を工夫して書く：ただでさえ複雑な六次元累積和がさらに複雑化する
  // - 長さ6のtupleをキーにするmapを作る：わかりやすいが、計算量が不安
  // - 六次元配列を作る：欲しい要素を取り出すコードを書くのが大変
  vector vec(11,vector(11,vector(11,vector(11,vector(11,vector<long long>(11,0))))));
  for (int i=0; i<n; i++) {
    vec.at(s.at(i).at(0)-'0'+1).at(s.at(i).at(1)-'0'+1).at(s.at(i).at(2)-'0'+1).at(s.at(i).at(3)-'0'+1).at(s.at(i).at(4)-'0'+1).at(s.at(i).at(5)-'0'+1) += v.at(i);
  }

  // 全方向に累積和を取る
  // 順列全探索を用いて同じコードで全方向分やる
  vector<int> tmp = {0,0,0,0,0,1};
  do {
    for (int i0=0; i0<=10-tmp.at(0); i0++) {
      for (int i1=0; i1<=10-tmp.at(1); i1++) {
        for (int i2=0; i2<=10-tmp.at(2); i2++) {
          for (int i3=0; i3<=10-tmp.at(3); i3++) {
            for (int i4=0; i4<=10-tmp.at(4); i4++) {
              for (int i5=0; i5<=10-tmp.at(5); i5++) {
                vec.at(i0+tmp.at(0)).at(i1+tmp.at(1)).at(i2+tmp.at(2)).at(i3+tmp.at(3)).at(i4+tmp.at(4)).at(i5+tmp.at(5)) += vec.at(i0).at(i1).at(i2).at(i3).at(i4).at(i5);
              }
            }
          }
        }
      }
    }
  } while (next_permutation(tmp.begin(),tmp.end()));

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    string x, y;
    cin >> x >> y;

    //////////////// 出力変数定義 ////////////////

    long long result = 0;

    //////////////////// 処理 ////////////////////

    // 求めたい範囲の情報を、方向ごとでもっておく
    vector<int> c0 = {x.at(0)-'0',max(x.at(0)-'0',y.at(0)-'0'+1)};
    vector<int> c1 = {x.at(1)-'0',max(x.at(1)-'0',y.at(1)-'0'+1)};
    vector<int> c2 = {x.at(2)-'0',max(x.at(2)-'0',y.at(2)-'0'+1)};
    vector<int> c3 = {x.at(3)-'0',max(x.at(3)-'0',y.at(3)-'0'+1)};
    vector<int> c4 = {x.at(4)-'0',max(x.at(4)-'0',y.at(4)-'0'+1)};
    vector<int> c5 = {x.at(5)-'0',max(x.at(5)-'0',y.at(5)-'0'+1)};

    // 累積和をとるのに必要な2^6か所の和を取る
    for (uint b=0; b<(1<<6); b++) {
      int sign = 1;
      if (popcount(b)%2) sign=-1;
      result += vec.at(c0.at((b>>0)&1)).at(c1.at((b>>1)&1)).at(c2.at((b>>2)&1)).at(c3.at((b>>3)&1)).at(c4.at((b>>4)&1)).at(c5.at((b>>5)&1))*sign;
    }

    //////////////////// 出力 ////////////////////

    cout << result << endl;

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}