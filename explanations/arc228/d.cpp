#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// エラトステネスの篩を用いた素因数分解
vector<int> s_primes;
void make_s_primes (int n) {
  assert(n>=0);
  s_primes.assign(n+1,-1);
  bool flag = true;
  for (int i=2; i<=n; i++) {
    if (s_primes[i]!=-1) continue;
    s_primes[i] = i;
    if (!flag) continue;
    for (int j=i*i; j<=n; j+=i) {
      if (s_primes[j]==-1) s_primes[j] = i;
    }
    if (i*i>n) flag = false;
  }
}
vector<pair<int,int>> prime_factorization(int n) {
  assert(n>0);
  assert(n<ssize(s_primes));
  vector<pair<int,int>> result;
  while (n>1) {
    if (!result.empty()&&result.back().first==s_primes[n]) result.back().second++;
    else result.emplace_back(s_primes[n],1);
    n /= s_primes[n];
  }
  return result;
}

// 累乗
long long power(long long a, long long p) {
  assert(p>=0);
  long long result = 1;
  long long b = a;
  while (p>0) {
    if (p&1LL) result = result*b;
    p >>= 1;
    if (p>0) b = b*b;
  }
  return result;
}

// 恒等置換
vector<int> e(int n) {
  vector<int> result(n);
  iota(result.begin(),result.end(),0);
  return result;
}

// 置換の積
vector<int> prod(const vector<int>& a, const vector<int>& b) {
  vector<int> c;
  c.reserve(a.size());
  for (int i : a) {
    c.emplace_back(b.at(i));
  }
  return c;
}

// 置換のダブリング（繰り返し二乗法）
vector<int> doubling(const vector<int>& a, int p) {
  vector<int> result = e(ssize(a));
  vector<int> b = a;
  while (p>0) {
    if (p&1LL) result = prod(result,b);
    b = prod(b,b);
    p >>= 1;
  }
  return result;
}

// 置換の分解（巡回表現）
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

// 置換の位数、つまりその置換を何乗すれば恒等置換になるか、の素因数分解
// 各巡回の長さの最小公倍数
map<int,int> order(const vector<int>& a) {
  map<int,int> result;
  for (auto& v : cycle(a)) {
    for (auto [p,e] : prime_factorization(ssize(v))) result[p] = max(result[p],e);
  }
  return result;
}

// チェッカー
// aを累乗してbにできるか確認する
bool checker(const vector<int>& a, const vector<int>& b) {

  // 指数が、各素因数ごとにどういう条件を満たせばよいか
  // conditions[p]が{e,r}であれば、指数をp^eで割った余りがrでなければならないという意味
  map<int,pair<int,int>> conditions;

  // とりあえずaを巡回表現にする
  vector<vector<int>> vv = cycle(a);

  // その巡回を1つずつ調査
  for (const vector<int>& vec1 : vv) {

    // 巡回内の各数値が、ずらした後でいくつになっていればよいか
    vector<int> vec2;
    for (int i : vec1) {
      vec2.emplace_back(b.at(i));
    }

    // 巡回をいくつずらせばよいか
    // すなわち、vec2の先頭の数値がvec1のどこに入っているか調査
    int pos = -1;
    for (int i=0; i<ssize(vec1); i++) {
      if(vec1.at(i)==vec2.front()) {
        pos = i;
        break;
      }
    }

    // vec2の先頭の値がvec1のどこにもいなかったら不可能判定
    if (pos==-1) return false;

    // vec1の方をposだけ前にずらしてみて、一致しなかったら不可能判定
    // をしたいが、vec1を崩したくないのでvec2を後ろにposずらす
    if (pos>0) rotate(vec2.begin(),vec2.begin()+(ssize(vec1)-pos),vec2.end());
    if (vec1!=vec2) return false;

    // vec1のサイズを素因数分解
    vector<pair<int,int>> primes = prime_factorization(ssize(vec1));

    // 素因数ごとに、条件を満たすか確認と更新
    for (auto [p,e] : primes) {

      // 既存条件があるかないかで分岐
      if (!conditions.contains(p)) {

        // ない場合はただ追加して終わり
        conditions[p] = {e,pos%power(p,e)};

      } else {

        // ある場合
        // この素因数の、指数と、p^eで割った余り
        int e_tmp = e;
        int r_tmp = pos%power(p,e_tmp);

        // 厳しい条件の方をconditionsにいれておく
        // 矛盾判定は後でやるので、swapで元条件も取り出しておく
        if (conditions[p].first<e_tmp) {
          swap(conditions[p].first,e_tmp);
          swap(conditions[p].second,r_tmp);
        }

        // 矛盾判定、矛盾していたら不可能
        if ((conditions[p].second-r_tmp)%power(p,e_tmp)) return false;

      }

    }

  }

  // 矛盾なく処理が完了したら、可能
  return true;

}

/////////////////// メイン ///////////////////

int main () {

  /////////////////// 前入力 ///////////////////

  int q;
  cin >> q;

  /////////////////// 前処理 ///////////////////

  // 素因数分解の準備で、500以下の各数の最小素因数を求めておく
  make_s_primes(500);

  /////////////////// ループ ///////////////////

  for (int loop=0; loop<q; loop++) {

    //////////////////// 入力 ////////////////////

    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> a(m,vector<int>(n));
    for (int i=0; i<m; i++) {
      for (int j=0; j<n; j++) {
        cin >> a.at(i).at(j);
        a.at(i).at(j)--;
      }
    }

    //////////////// 出力変数定義 ////////////////

    // 恒等置換を用意しておく
    vector<int> result = e(n);

    //////////////////// 処理 ////////////////////

    // 現在の result の位数、つまり何乗すれば恒等置換になるか
    map<int,int> order1 = order(result);

    // aを1つずつ、考慮に入れる範囲に追加していく
    for (vector<int> vec : a) {

      // 追加する置換の位数
      map<int,int> order2 = order(vec);

      // resultと追加分、位数の最小公倍数が不変なまま互いに素になるよう、それぞれを適切に累乗する
      // 位数の各素因数の指数が小さい方を、位数からなくなるようにしていけばよい
      for (auto [p,e] : order2) {
        if (order1[p]>e&&e>0) vec = doubling(vec,power(p,e));
        else if (e>=order1[p]&&order1[p]>0) result = doubling(result,power(p,order1[p]));
      }

      // result更新
      // 位数が互いに素なら、それらを単純に結合したものが候補であり、これでダメなら不可
      // 位数は最小公倍数を、素因数分解状態のまま求める
      for (auto [p,e] : order2) {
        if (e>=order1[p]) order1[p] = e;
      }
      result = prod(result,vec);

    }

    // 可能なら作った候補でできるはず
    // よって、候補をチェッカーにかけて、成立しているならよし、してないなら不可能
    for (const vector<int>& vec : a) {
      if (!checker(result,vec)) {
        result.clear();
        break;
      }
    }

    //////////////////// 出力 ////////////////////

    if (result.empty()) {
      cout << -1 << endl;
    } else {
      for (size_t i=0; i<result.size(); i++) {
        cout << result.at(i)+1;
        if (i!=result.size()-1) {
          cout << " ";
        }
      }
      cout << endl;
    }

  }

  /////////////////// 後処理 ///////////////////



  //////////////////// 終了 ////////////////////

  return 0;

}