#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

constexpr int mod = 998244353;

// 繰り返し二乗法（負冪対応）
long long power_mod(long long a, long long p) {
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

// 階乗とその逆元
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

// 二項係数
long long comb(int n, int r) {
  if (r<0) return 0;
  if (r>n) return 0;
  long long result = factorial.at(n);
  result *= fact_inv.at(r);
  result %= mod;
  result *= fact_inv.at(n-r);
  result %= mod;
  return result;
}

/////////////////// メイン ///////////////////

int main () {
  
  //////////////////// 入力 ////////////////////

  int n;
  cin >> n;

  vector<int> a(2*n);
  for (int i=0; i<2*n; i++) {
    cin >> a.at(i);
  }

  //////////////// 出力変数定義 ////////////////

  vector<int> result(2*n,0);

  //////////////////// 処理 ////////////////////

  // 階乗とその逆元を、2Nまで作っておく
  make_factorial(2*n);

  // 最後の試合前での勝利数最大値を取得
  int mx = *max_element(a.begin(),a.end());

  // 勝利数最大の人をt、1つ少ない人をs、2つ以上少ない人をoと表現する
  // 最終試合の「t同士」「tとs」「s同士」「tとo」「sとo」の組数
  int tt = 0;
  int ts = 0;
  int ss = 0;
  int to = 0;
  int so = 0;

  // それぞれの組数をカウントする。
  // ついでに、各人について最終戦がどういう扱いか記録する
  // 「t同士決戦」→ -1
  // 「自分はtで相手はs」→ -2
  // 「自分はsで相手はt」→ -3
  // 「s同士決戦」→ -4
  // 「自分はtで相手はo」→ -5
  // 「自分はsで相手はo」→ -6
  for (int i=0; i<2*n; i+=2) {
    if (a.at(i)==mx&&a.at(i+1)==mx) {
      tt++;
      result.at(i) = -1;
      result.at(i+1) = -1;
    } else if (a.at(i)==mx&&a.at(i+1)==mx-1) {
      ts++;
      result.at(i) = -2;
      result.at(i+1) = -3;
    } else if (a.at(i)==mx-1&&a.at(i+1)==mx) {
      ts++;
      result.at(i) = -3;
      result.at(i+1) = -2;
    } else if (a.at(i)==mx-1&&a.at(i+1)==mx-1) {
      ss++;
      result.at(i) = -4;
      result.at(i+1) = -4;
    } else if (a.at(i)==mx) {
      to++;
      result.at(i) = -5;
    } else if (a.at(i+1)==mx) {
      to++;
      result.at(i+1) = -5;
    } else if (a.at(i)==mx-1) {
      so++;
      result.at(i) = -6;
    } else if (a.at(i+1)==mx-1) {
      so++;
      result.at(i+1) = -6;
    }
  }

  // 「t同士」の人の優勝確率
  // tt戦の勝者全員と、tsとtoのうちtで勝ったi人で優勝決定戦をする
  // よって、確率は Σ[i] 1/(tt+i) * Comb(ts+to,i) * (1/2)^(ts+to+1)
  // （+1は、自分が勝つ必要があるため）
  long long ptt = 0;
  if (tt>0) {
    for (int i=0; i<=ts+to; i++) {
      long long tmp = power_mod(tt+i,-1);
      tmp *= comb(ts+to,i);
      tmp %= mod;
      tmp *= power_mod(2,-ts-to-1);
      tmp %= mod;
      ptt += tmp;
      ptt %= mod;
    }
  }

  // 「tとs」のt側の人の優勝確率
  // まず、自分が勝つ場合。
  // tt戦の勝者全員と、tsとtoのうちtで勝ったi人で優勝決定戦をする
  // よって、確率は Σ[i] 1/(tt+i) * Comb(ts+to-1,i-1) * (1/2)^(ts+to)
  // （-1は、勝つ人に自分が含まれる必要があるため）
  // 次に、自分が負ける場合。
  // そもそもtt戦があれば不可能で、tsとtoは全部tが負ける必要がある。
  // その上で、ts戦の両者、to戦のt側の人、ss戦の勝者、so戦のsで勝った人i人で優勝決定戦をする。
  // よって、確率は Σ[i] 1/(2*ts+ss+to+i) * Comb(so,i) * (1/2)^(ts+to+so)
  long long pts = 0;
  if (ts>0) {
    for (int i=1; i<=ts+to; i++) {
      long long tmp = power_mod(tt+i,-1);
      tmp *= comb(ts+to-1,i-1);
      tmp %= mod;
      tmp *= power_mod(2,-ts-to);
      tmp %= mod;
      pts += tmp;
      pts %= mod;
    }
    if (tt==0) for (int i=0; i<=so; i++) {
      long long tmp = power_mod(2*ts+ss+to+i,-1);
      tmp *= comb(so,i);
      tmp %= mod;
      tmp *= power_mod(2,-ts-to-so);
      tmp %= mod;
      pts += tmp;
      pts %= mod;
    }
  }

  // 「tとs」のs側の人の優勝確率
  // そもそもtt戦があれば不可能で、tsとtoは全部tが負ける必要がある。
  // その上で、ts戦の両者、to戦のt側の人、ss戦の勝者、so戦のsで勝った人i人で優勝決定戦をする。
  // よって、確率は Σ[i] 1/(2*ts+ss+to+i) * Comb(so,i) * (1/2)^(ts+to+so)
  // （実はptsの後半の計算と全く同じ）
  long long pst = 0;
  if (tt==0&&ts>0) {
    for (int i=0; i<=so; i++) {
      long long tmp = power_mod(2*ts+ss+to+i,-1);
      tmp *= comb(so,i);
      tmp %= mod;
      tmp *= power_mod(2,-ts-to-so);
      tmp %= mod;
      pst += tmp;
      pst %= mod;
    }
  }

  // 「s同士」の人の優勝確率
  // そもそもtt戦があれば不可能で、tsとtoは全部tが負ける必要がある。
  // その上で、ts戦の両者、to戦のt側の人、ss戦の勝者、so戦のsで勝った人i人で優勝決定戦をする。
  // よって、確率は Σ[i] 1/(2*ts+ss+to+i) * Comb(so,i) * (1/2)^(ts+to+so+1)
  // （+1は、自分が勝つ必要があるため）
  long long pss = 0;
  if (tt==0&&ss>0) {
    for (int i=0; i<=so; i++) {
      long long tmp = power_mod(2*ts+ss+to+i,-1);
      tmp *= comb(so,i);
      tmp %= mod;
      tmp *= power_mod(2,-ts-to-so-1);
      tmp %= mod;
      pss += tmp;
      pss %= mod;
    }
  }

  // 「tとo」のt側の人の優勝確率
  // まず、自分が勝つ場合。
  // tt戦の勝者全員と、tsとtoのうちtで勝ったi人で優勝決定戦をする
  // よって、確率は Σ[i] 1/(tt+i) * Comb(ts+to-1,i-1) * (1/2)^(ts+to)
  // （-1は、勝つ人に自分が含まれる必要があるため）
  // 次に、自分が負ける場合。
  // そもそもtt戦があれば不可能で、tsとtoは全部tが負ける必要がある。
  // その上で、ts戦の両者、to戦のt側の人、ss戦の勝者、so戦のsで勝った人i人で優勝決定戦をする。
  // よって、確率は Σ[i] 1/(2*ts+ss+to+i) * Comb(so,i) * (1/2)^(ts+to+so)
  // （実はptsの計算と全く同じ）
  long long pto = 0;
  if (to>0) {
    for (int i=1; i<=ts+to; i++) {
      long long tmp = power_mod(tt+i,-1);
      tmp *= comb(ts+to-1,i-1);
      tmp %= mod;
      tmp *= power_mod(2,-ts-to);
      tmp %= mod;
      pto += tmp;
      pto %= mod;
    }
    if (tt==0) for (int i=2*ts+ss+to; i<=2*ts+ss+to+so; i++) {
      long long tmp = power_mod(i,-1);
      tmp *= comb(so,i-2*ts-ss-to);
      tmp %= mod;
      tmp *= power_mod(2,-ts-to-so);
      tmp %= mod;
      pto += tmp;
      pto %= mod;
    }
  }

  // 「sとo」のs側の人の優勝確率
  // そもそもtt戦があれば不可能で、tsとtoは全部tが負ける必要がある。
  // その上で、ts戦の両者、to戦のt側の人、ss戦の勝者、so戦のsで勝った人i人で優勝決定戦をする。
  // よって、確率は Σ[i] 1/(2*ts+ss+to+i) * Comb(so-1,i-1) * (1/2)^(ts+to+so)
  // （-1は、勝つ人に自分が含まれる必要があるため）
  long long pso = 0;
  if (tt==0&&so>0) {
    for (int i=1; i<=so; i++) {
      long long tmp = power_mod(2*ts+ss+to+i,-1);
      tmp *= comb(so-1,i-1);
      tmp %= mod;
      tmp *= power_mod(2,-ts-to-so);
      tmp %= mod;
      pso += tmp;
      pso %= mod;
    }
  }

  // 最初に記録した最終戦の扱いに合わせて確率を代入していく
  for (int& i : result) {
    if (i==-1) i = ptt;
    else if (i==-2) i = pts;
    else if (i==-3) i = pst;
    else if (i==-4) i = pss;
    else if (i==-5) i = pto;
    else if (i==-6) i = pso;
  }

  //////////////////// 出力 ////////////////////

  for (size_t i=0; i<result.size(); i++) {
    cout << result.at(i);
    if (i!=result.size()-1) {
      cout << " ";
    }
  }
  cout << endl;

  //////////////////// 終了 ////////////////////

  return 0;

}