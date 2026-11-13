// chatGPT による実装
// ABC483 F: Gaussian gridding による NUFFT と、整数の直接更新を併用する別解。
// 外部FFTライブラリは使用せず、C++23だけでコンパイルできる。
//
// 【誤差評価の前提と結論】
// L=2^21, T=1400, R=15, TAU=1.58 でのGaussian近似誤差は、
// 理想的な実数演算なら全周波数を合成しても 8.51e-6 未満。
// さらに、doubleがIEEE 754 binary64（最近接丸め）であり、使用する引数について
// sin/cosの絶対誤差が5e-16以下、exp/sqrtの相対誤差が5e-16以下、
// std::complexの加減乗算が通常の倍精度四則演算に従い、
// 複素除算の相対誤差が1e-14以下であることを仮定する。
// この条件を置いた誤差解析では、DFT・FFT・重み計算・格子加算・補正も含め、
// J(x)の絶対誤差を0.256未満と評価する。0.5未満なので整数への丸めが可能。
// ただし、C++23規格は数学関数や複素除算の上記精度を保証しない。
// したがって、この評価はこれらの条件が成立する環境についてのものである。

#include <bits/stdc++.h>
using namespace std;

using cd = complex<double>;
static constexpr double PI = 3.1415926535897932384626433832795;
static constexpr int L = 1 << 21;
static constexpr int K = L / 2;
static constexpr int T = 1400;
static constexpr int R = 15;
static constexpr double TAU = 1.58;

// 2の冪長に対する反復FFT。
// 正変換は exp(-2*pi*i*j*k/n)、逆変換は符号を反転した変換を用いる。
// 回転因子は長さLの複素数の根から間引いて使う。
// 回転因子のsin/cos計算とバタフライ演算の両方に浮動小数点誤差が生じる。
struct FFT {
    vector<cd> roots;
    FFT() : roots(K) {
        for (int j = 0; j < K; ++j) {
            double a = 2.0 * PI * j / L;
            roots[j] = cd(cos(a), sin(a));
        }
    }

    void run(vector<cd>& a, bool inverse, bool normalize = true) const {
        const int n = (int)a.size();
        for (int i = 1, j = 0; i < n; ++i) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            const int step = L / len;
            for (int st = 0; st < n; st += len) {
                for (int j = 0; j < len / 2; ++j) {
                    cd w = roots[j * step];
                    if (!inverse) w = conj(w);
                    const cd u = a[st + j];
                    const cd v = a[st + j + len / 2] * w;
                    a[st + j] = u + v;
                    a[st + j + len / 2] = u - v;
                }
            }
        }
        if (inverse && normalize) {
            const double invn = 1.0 / n;
            for (cd& z : a) z *= invn;
        }
    }

    // Bluestein法で任意長bのDFTを計算する。
    // C_b(k) = sum_a c[a] * exp(2*pi*i*a*k/b)。
    // exp(pi*i*a*a/b) を使うチャープ変換により、畳み込みへ帰着する。
    // 実数列の共役対称性から、返すのは非零の正周波数 k=1..floor(b/2) のみ。
    vector<cd> bluestein(const vector<int>& c, int b) const {
        int p = 1;
        while (p < 2 * b - 1) p <<= 1;
        vector<cd> f(p), h(p), chirp(b);
        for (int a = 0; a < b; ++a) {
            const double theta = PI * (1LL * a * a % (2LL * b)) / b;
            chirp[a] = cd(cos(theta), sin(theta));
            f[a] = double(c[a]) * chirp[a];
            h[a] = conj(chirp[a]);
            if (a != 0) h[p - a] = conj(chirp[a]);
        }
        run(f, false);
        run(h, false);
        for (int j = 0; j < p; ++j) f[j] *= h[j];
        run(f, true);
        vector<cd> result(b / 2 + 1);
        for (int k = 1; k <= b / 2; ++k) result[k] = f[k] * chirp[k];
        return result;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<pair<int, int>> ab(N);
    long long initial = 0;
    for (auto& [a, b] : ab) {
        cin >> a >> b;
        initial += a;
    }
    int Q;
    cin >> Q;
    vector<int> query(Q);
    int M = 0;
    for (int& x : query) {
        cin >> x;
        M = max(M, x);
    }
    if (M == 0) {
        for (int i = 0; i < Q; ++i) cout << initial << '\n';
        return 0;
    }

    // J(x) = sum_{i: b_i | x+a_i} b_i とすれば、
    // F(0) = sum_i a_i, F(x) = F(x-1) + N - J(x) が成り立つ。
    // small[b][a] は法b・オフセットaの個数。
    // 大きい法のJ(x)は、整数だけを使う等差数列上の直接更新で求める。
    vector<vector<int>> small(T + 1);
    vector<long long> jump(M + 1);
    auto update_direct = [&](int a, int b, int count) {
        const long long v = 1LL * b * count;
        for (int x = b - a; x <= M; x += b) jump[x] += v;
    };
    for (auto [a, b] : ab) {
        if (b <= T) {
            if (small[b].empty()) small[b].assign(b, 0);
            ++small[b][a];
        } else {
            update_direct(a, b, 1);
        }
    }

    // 小さい法の周期関数をDFTで分解し、同じ既約周波数 k/b=r/d の係数を合算する。
    // freq[d*(d-1)/2+r] にその係数を格納する。
    // spectral_count はNUFFTで処理する法と一様な法の、周波数0の寄与の合計。
    vector<cd> freq((T + 1) * (T + 2) / 2);
    long long spectral_count = 0;
    bool has_nonzero_frequencies = false;
    unique_ptr<FFT> fft;
    for (int b = 1; b <= T; ++b) {
        if (small[b].empty()) continue;
        const vector<int>& c = small[b];
        const int total = accumulate(c.begin(), c.end(), 0);
        const int mn = *min_element(c.begin(), c.end());
        const int mx = *max_element(c.begin(), c.end());
        if (mn == mx) {
            // c[a]が全て等しければ、法bのJ(x)への寄与は常にtotal。
            // 非零周波数は厳密に消えるため、NUFFTの計算は不要。
            spectral_count += total;
            continue;
        }

        long long direct_ops = 0;
        int nonzero = 0, changes = 0;
        for (int a = 0; a < b; ++a) {
            if (c[a] != 0) {
                ++nonzero;
                const int first = b - a;
                if (first <= M) direct_ops += 1 + (M - first) / b;
            }
            changes += (c[a] != c[(a + b - 1) % b]);
        }

        // 更新回数の少ない法では、DFTより整数の直接更新が安い。
        // この判定は実行速度のためのヒューリスティックであり、正確性の近似条件ではない。
        const int depth = std::bit_width(unsigned(2 * b - 2));
        const long long spectral_cost = 100LL * b * depth + 50LL * b;
        if (direct_ops <= 20000 || (direct_ops < spectral_cost && nonzero < b / 2)) {
            for (int a = 0; a < b; ++a) if (c[a]) update_direct(a, b, c[a]);
            continue;
        }

        spectral_count += total;
        has_nonzero_frequencies = true;
        if (!fft) fft = make_unique<FFT>();
        // c[a]から共通の最小値を引いて疎にする。
        // 定数列を引いても非零周波数のDFT係数は変わらない。
        vector<int> centered(b);
        int sparse = 0;
        for (int a = 0; a < b; ++a) {
            centered[a] = c[a] - mn;
            sparse += (centered[a] != 0);
        }
        vector<cd> modes(b / 2 + 1);

        if (min(sparse, changes) <= 9) {
            // centeredが疎なら、非零の項だけから直接DFTを計算する。
            // 巡回階差が疎なら、sum_a(c[a]-c[a-1])z^a
            // = (1-z)*sum_a c[a]z^a を使い、最後に(1-z)で割る。
            // zが1に近い周波数では除算によって丸め誤差が拡大する。
            // 誤差評価には |1-exp(2*pi*i*k/b)| >= 4*k/b と
            // 冒頭に記した複素除算の精度条件を用いる。
            const bool use_difference = changes < sparse;
            for (int a = 0; a < b; ++a) {
                int coef = use_difference ? c[a] - c[(a + b - 1) % b] : centered[a];
                if (!coef) continue;
                const double theta = 2.0 * PI * a / b;
                const cd step(cos(theta), sin(theta));
                cd power = step;
                for (int k = 1; k <= b / 2; ++k) {
                    modes[k] += double(coef) * power;
                    power *= step;
                }
            }
            if (use_difference) {
                const cd step(cos(2.0 * PI / b), sin(2.0 * PI / b));
                cd z = step;
                for (int k = 1; k <= b / 2; ++k) {
                    modes[k] /= (cd(1.0, 0.0) - z);
                    z *= step;
                }
            }
        } else {
            modes = fft->bluestein(centered, b);
        }

        for (int k = 1; k <= b / 2; ++k) {
            const int g = gcd(b, k);
            const int d = b / g;
            const int r = k / g;
            freq[d * (d - 1) / 2 + r] += modes[k];
        }
    }

    // 非零周波数がなければ、以降は整数の差分・累積和だけで正確に求められる。
    if (!has_nonzero_frequencies) {
        long long cur = initial;
        for (int x = 1; x <= M; ++x) {
            cur += N - jump[x] - spectral_count;
            jump[x] = cur;
        }
        jump[0] = initial;
        for (int x : query) cout << jump[x] << '\n';
        return 0;
    }
    if (!fft) fft = make_unique<FFT>();

    // 不等間隔の既約周波数r/dを、長さLのFFT格子にGaussianで分配する。
    // 正周波数側[0,L/2]だけを保持し、負周波数側は共役対称性で復元する。
    // 各周波数は中心付近の2*R+1=31点に配置する（近似による打ち切り）。
    // expで初期重みと比を求め、等比的な漸化式で隣の格子点の重みを作る。
    // 近似核自体の誤差と、これらのdouble演算による誤差は別々に評価する必要がある。
    vector<cd> grid(K + 1);
    const double alpha = exp(-1.0 / (2.0 * TAU));
    for (int d = 2; d <= T; ++d) {
        for (int r = 1; r <= d / 2; ++r) {
            if (gcd(d, r) != 1) continue;
            const cd coefficient = freq[d * (d - 1) / 2 + r];
            if (coefficient == cd(0.0, 0.0)) continue;
            const int base = int(1LL * r * L / d);
            const double frac = double(1LL * r * L % d) / d;
            const double delta = -R - frac;
            double weight = exp(-delta * delta / (4.0 * TAU));
            double ratio = exp(-(2.0 * delta + 1.0) / (4.0 * TAU));
            for (int j = -R; j <= R; ++j) {
                const int p = base + j;
                if (p <= K) grid[p] += coefficient * weight;
                weight *= ratio;
                ratio *= alpha;
            }
        }
    }

    // 実数列の逆FFTを、長さK=L/2の複素逆FFT1回にまとめる。
    // 共役な周波数係数を組み直すと、出力の実部が偶数x、虚部が奇数xに対応する。
    // normalize=false とするのは、ここで必要な値が周波数成分の「和」だから。
    vector<cd> packed(K);
    for (int k = 0; k < K; ++k) {
        const cd u = grid[k];
        const cd v = conj(grid[K - k]);
        const cd w = fft->roots[k];
        const cd iw(-w.imag(), w.real());
        packed[k] = (u + v) + iw * (u - v);
    }
    fft->run(packed, true, false);

    vector<long long> answer(M + 1);
    answer[0] = initial;
    for (int x = 1; x <= M; ++x) {
        // Gaussian配置と逆FFTで生じた減衰
        // sqrt(4*pi*tau)*exp(-tau*theta^2) を除去し、非零周波数の寄与を復元する。
        // 冒頭の浮動小数点精度条件を仮定した解析では、J(x)の絶対誤差は0.256未満。
        // この条件下では0.5未満のため、llroundで正しい整数へ丸められる。
        const double theta = 2.0 * PI * x / L;
        const double norm = sqrt(4.0 * PI * TAU) * exp(-TAU * theta * theta);
        const double signal = (x & 1) ? packed[x / 2].imag() : packed[x / 2].real();
        const long long extra = llround(signal / norm);
        answer[x] = answer[x - 1] + N - jump[x] - spectral_count - extra;
    }
    for (int x : query) cout << answer[x] << '\n';
}
