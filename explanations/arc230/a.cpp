#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;

// 累乗剰余
long long power_mod(long long a, long long p) {
  if (p==0) return 1LL;
  a %= mod;
  if (a<0) a += mod;
  if (p<0) {
    assert(gcd(a,mod)==1);
    p = (p%(mod-1))+mod-1;
  }
  long long result = 1;
  long long b = a;
  while (p>0) {
    if (p&1LL) result = result*b%mod;
    b = b*b%mod;
    p >>= 1;
  }
  return result;
}

// 階乗剰余
vector<long long> factorial;
vector<long long> fact_inv;
void make_factorial(int n, bool make_inv = true) {
  assert(n>=0);
  if (make_inv) assert(n<mod);
  factorial.assign(n+1,1);
  for (int i=1; i<=n; i++) {
    factorial[i] = factorial[i-1]*i%mod;
  }
  if (!make_inv) return;
  fact_inv.assign(n+1,power_mod(factorial[n],-1));
  for (int i=n; i>0; i--) {
    fact_inv[i-1] = fact_inv[i]*i%mod;
  }
}

// 二項係数剰余
long long comb(int n, int r) {
  if (r<0) return 0;
  if (r>n) return 0;
  long long result = factorial[n];
  result *= fact_inv[r];
  result %= mod;
  result *= fact_inv[n-r];
  result %= mod;
  return result;
}
long long comb_inv(int n, int r) {
  assert(r>=0);
  assert(r<=n);
  assert(n<mod);
  long long result = fact_inv[n];
  result *= factorial[r];
  result %= mod;
  result *= factorial[n-r];
  result %= mod;
  return result;
}

// 幅優先探索による根付き木化とトポロジカルソート
tuple<vector<int>,vector<int>,vector<vector<int>>,vector<int>> bfs(const vector<vector<int>>& graph, const vector<int>& starts) {
  int n = ssize(graph);
  vector<int> distances(n,-1);
  vector<int> parents(n,-1);
  vector<vector<int>> children(n);
  vector<int> order;
  deque<int> que;
  for (int v : starts) {
    assert(v>=0);
    assert(v<n);
    assert(distances[v]==-1);
    distances[v] = 0;
    que.emplace_back(v);
    order.emplace_back(v);
  }
  while (!que.empty()) {
    for (int v : graph[que.front()]) {
      if (distances[v]==-1) {
        distances[v] = distances[que.front()]+1;
        que.emplace_back(v);
        order.emplace_back(v);
        parents[v] = que.front();
        children[que.front()].emplace_back(v);
      }
    }
    que.pop_front();
  }
  return {distances,parents,children,order};
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  int m = n-1;
  vector<int> u(m), v(m);
  vector<vector<int>> graph(n);
  for (int i=0; i<m; i++) {
    cin >> u.at(i) >> v.at(i);
    u.at(i)--;
    v.at(i)--;
    graph.at(u.at(i)).emplace_back(v.at(i));
    graph.at(v.at(i)).emplace_back(u.at(i));
  }

  //////////////// 出力変数定義 ////////////////

  long long result = 0;

  //////////////////// 処理 ////////////////////

  // 階乗の用意
  make_factorial(n);

  // comb(n,i) の累積和と、i*comb(n,i) の累積和
  vector<long long> sum_comb1(n+1,0);
  vector<long long> sum_comb2(n+1,0);
  sum_comb1.at(0) = 1;
  for (int i=1; i<=n; i++) {
    sum_comb1.at(i) = sum_comb1.at(i-1)+comb(n,i);
    sum_comb1.at(i) %= mod;
    sum_comb2.at(i) = sum_comb2.at(i-1)+i*comb(n,i);
    sum_comb2.at(i) %= mod;
  }

  // 幅優先探索による根付き木化とトポロジカルソート
  // ソートは逆順で使いたいのでreverseしておく
  auto [distances,parents,children,order] = bfs(graph,{0});
  reverse(order.begin(),order.end());

  // 各頂点の、自身を含めたその枝の頂点数
  vector<int> num(n,1);

  // 2^(n-1) を求めておく
  long long p = power_mod(2,n-1);

  // トポロジカルソート逆順に、木DPで枝のサイズを求めながら、結果計算
  for (int i : order) {

    // その枝のサイズ
    for (int j : children.at(i)) num.at(i) += num.at(j);

    // その頂点と親を結ぶ辺でわけて、少ない方の個数
    int l = min(num.at(i),n-num.at(i));

    // l*2^(n-1)をベースに、Σ(l-i)*comb(n,i) を引く
    result += p*l-l*sum_comb1.at(l)+sum_comb2.at(l);
    result %= mod;

  }

  // 負数対応
  if (result<0) result += mod;

  //////////////////// 出力 ////////////////////

  cout << result << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}