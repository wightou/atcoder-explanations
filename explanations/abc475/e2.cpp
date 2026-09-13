#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, m, k;
  string t;
  cin >> n >> m >> k >> t;

  vector<string> s(n);
  for (int i=0; i<n; i++) {
    cin >> s.at(i);
  }

  int q;
  cin >> q;

  vector<int> a(q), b(q);
  for (int i=0; i<q; i++) {
    cin >> a.at(i) >> b.at(i);
    a.at(i)--;
    b.at(i)--;
  }

  //////////////// 出力変数定義 ////////////////

  vector<string> result(q,"No");

  //////////////////// 処理 ////////////////////

  // 登場しうる正誤文字列を全て座標圧縮で管理するmap
  map<string,int> mp;

  // 全問不正解の人を番兵として追加しておく
  // 全問不正解がm位判定で通過しないようにする意味がある
  mp[string(k,'1')] = -1;

  // 初期の正誤判定を全て行って記録し、登場した文字列は全てmapへ
  for (int i=0; i<n; i++) {
    for (int j=0; j<k; j++) {
      if (s.at(i).at(j)==t.at(j)) s.at(i).at(j) = '0';
      else s.at(i).at(j) = '1';
    }
    mp[s.at(i)] = -1;
  }

  // 初期状態をコピーして先に一度正誤変更シミュレーションを走らせる
  // 登場した文字列は全てmapへ
  vector<string> tmp = s;
  for (int i=0; i<q; i++) {
    tmp.at(a.at(i)).at(b.at(i)) ^= '0'^'1';
    mp[tmp.at(a.at(i))] = -1;
  }

  // 扱うべき文字列に、昇順に番号をつける
  int count = 0;
  for (auto& [str,val] : mp) {
    val = count;
    count++;
  }

  // 扱うべき文字列のパターン数だけの要素を持つfenwick木（座標圧縮してから入れる）
  fenwick_tree<int> f(count);

  // 最初からある正誤判定文字列分と番兵の分を入れておく
  for (int i=0; i<n; i++) {
    int pos = mp[s.at(i)];
    f.add(pos,1);
  }
  f.add(count-1,1);

  // 再度、正誤変更シミュレーションを走らせる
  for (int i=0; i<q; i++) {

    // 差分更新で、正誤文字列の個数を更新する
    f.add(mp[s.at(a.at(i))],-1);
    s.at(a.at(i)).at(b.at(i)) ^= '0'^'1';
    f.add(mp[s.at(a.at(i))],1);

    // 自分より優秀または同じ成績の人が、m人以下なら予選突破
    if (f.sum(0,mp[s.at(a.at(i))]+1)<=m) result.at(i) = "Yes";

  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i) << endl;
  }

  //////////////////// 終了 ////////////////////

  return 0;

}
