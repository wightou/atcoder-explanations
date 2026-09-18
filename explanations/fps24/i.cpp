#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// AtCoder Libraryのconvolutionのラッパー
vector<long long> convolution_acl(const vector<long long>& a_org, const vector<long long>& b_org, int lim = (1<<23)-1) {
  if (a_org.empty()||b_org.empty()||lim<0) return {};
  vector<long long> a(a_org.begin(),a_org.begin()+min((int)ssize(a_org),lim+1));
  vector<long long> b(b_org.begin(),b_org.begin()+min((int)ssize(b_org),lim+1));
  vector<long long> result = convolution(a,b);
  if (ssize(result)>lim+1) result.resize(lim+1);
  return result;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n, k;
  cin >> n >> k;

  vector<long long> a(n);
  for (int i=0; i<n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  int result = 0;

  //////////////////// 処理 ////////////////////

  deque<vector<long long>> que;

  // 母関数 1+a[i]x を全て用意する
  for (int i=0; i<n; i++) {
    que.emplace_back(vector({1LL,a.at(i)}));
  }

  // 大量の一次式畳み込みを、queueを使って高速化
  while (ssize(que)>1) {
    que.emplace_back(convolution_acl(que.at(0),que.at(1),k));
    que.pop_front();
    que.pop_front();
  }

  // 全部畳み込み終わった後の、x^kの係数が答え
  result = que.at(0).at(k);

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}