#include "myLibrary.h"

namespace numTheory {

    inline int bitLength(int n) {
        return n ? log2(n) + 1 : 0;
    }

    pair<int, int> extGcd(int a, int b = 0) {
        if (!b) return {1, 0};
        auto down = extGcd(b, a % b);
        int x = down.second;
        int y = down.first - floor(a / b) * down.second;
        return {x, y};
    }

    inline int modInv(int a, int m) {
        return __gcd(a, m) == 1 ? (extGcd(a, m).first + m) % m : 0;
    }

    pair<int, int> diophantine(int a, int b, int c) {
        int g = __gcd(a, b);
        if (c % g)
            return {};
        int k = c / g;
        auto ans = extGcd(a, b);
        return {ans.first * k, ans.second * k};
    }

    int CRT(vector<int> &N, vector<int> &R) {
        int product = 1;
        for (auto &i : N)
            product *= i;
        vector<int> PP(N.size());
        for (int i = 0; i < N.size(); ++i)
            PP[i] = product / N[i];
        int x = 0;
        for (int i = 0; i < N.size(); ++i)
            x += PP[i] * R[i] * modInv(PP[i], N[i]);
        return x;
    }
}

struct prime {
#define MAX_SIEVE_SIZE (unsigned int)(10e6 + 1)

    class sieve {
        unsigned int n;
        vector<int> primeList;
        vector<int> inRangePfxQuery;
        vector<unordered_set<int>> phiSieve;

    public:
        explicit sieve(int n = MAX_SIEVE_SIZE - 1) : n(n) {
            bitset<MAX_SIEVE_SIZE> flag;
            if (MAX_SIEVE_SIZE <= n) {
                cerr << "Stack Overflow : n > MAX_SIEVE_SIZE" << endl;
                exit(69);
            }
            inRangePfxQuery.resize(n + 1);

            for (int i = 2; i <= n; i++) {
                if (flag[i]) {
                    primeList.emplace_back(i);
                    inRangePfxQuery[i]++;
                    for (int j = i + i; j <= n; j = j + i)
                        flag[j] = false;
                }
                inRangePfxQuery[i] += inRangePfxQuery[i - 1];
            }
        }

    private:
        void buildPhiSieve(int x) {
            phiSieve.resize(x + 1);
            auto build = [&]() {
                phiSieve[0].insert(1);
                phiSieve[1].insert(1);
                for (int i = 0; i < count() && primeList[i] <= x; i++)
                    phiSieve[primeList[i]].insert(1);
                for (int i = 0; primeList[i] * primeList[i] <= x; i++) {
                    for (int j = 2 * primeList[i]; j <= x; j += primeList[i])
                        if (phiSieve[j].empty())
                            phiSieve[j].insert(primeList[i]);  //inserting smallest prime factor
                }
            };
            build();
            auto prime_factorize = [&](
                    int index) {  //index -> is the no. we prime_factorizing
                int divisor_index =
                        index / (*phiSieve[index].begin());  //divisor-> smallest prime_factor of no.
                if (divisor_index == index)              //the number is prime
                    return;
                if (phiSieve[divisor_index].size() == 1 &&
                    phiSieve[divisor_index].count(1))       //number at divisor index is prime
                    phiSieve[index].insert(divisor_index);  //insert that prime number
                else
                    for (auto &i : phiSieve[divisor_index])
                        phiSieve[index].insert(i);
            };
            int i = 3;
            while (++i <= x)
                prime_factorize(i);
        }

    public:
        bool xisPrime(int x) {
            return query(x, x);
        }

        int query(int A, int B) {
            if (A > B) {
                cerr << "err : invalid query.code=0 " << endl;
                return -1;
            }
            if (B > n * n) {
                cerr << "err : invalid query.code=1 " << endl;
                return -1;
            }  //edge and error handling
            if (B <= n) {
                return inRangePfxQuery[B] - inRangePfxQuery[abs(A - 1)];
            }
            int prime_count = 0;
            if (A <= n) {
                prime_count = inRangePfxQuery[n] - inRangePfxQuery[abs(A - 1)];
                A = n + 1;
            }
            A = A < 2 ? 2 : A;
            vector<bool> prime_query_sieve(B - A + 1, false);
            auto build = [&]() {
                for (auto i = primeList.begin(); i != primeList.end() && (*i * (*i) < B);
                     i++) {
                    for (int j = 0; j <= B - A; j++) {
                        if ((j + A) % (*i) == 0)
                            prime_query_sieve[j] = true;
                    }
                }
            };
            build();
            for (int i = 0; i <= B - A; ++i)
                if (!prime_query_sieve[i]) prime_count++;
            return prime_count;
        }

        inline const vector<int> &getPrimeList() {
            return primeList;
        }

        inline int count() {
            return inRangePfxQuery[n];
        }

        int phi(unsigned int N) {
            double phi = 1.00000f;
            if (!(N & 1)) {
                phi = 2.00000f;
                N /= 2;
            }
            buildPhiSieve(N);
            const auto &prime_factors = phiSieve[N];
            if (prime_factors.count(1))
                return phi * (N - 1);
            for (double i : prime_factors)
                phi *= (1 - (1 / i));
            phi *= N;
            return phi;
        }
    };

    bool isPrime(int n) {
        if (n < 5 || isEven(n) || n % 3 == 0)
            return (2 <= n && n <= 3);
        int s = numTheory::bitLength((n - 1) & (1 - n)) - 1;
        int d = n >> s;

        vector<int> __millerRabinSeeds = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};

        for (auto &i : __millerRabinSeeds) {
            int p = power(i, d, n);
            if (p == 1 || p == n - 1 || i % n == 0)
                continue;
            bool flag = false;
            for (int _ = 0; _ < s; _++) {
                p = (p * p) % n;
                if (p == n - 1)
                    break;
                if (_ == s - 1)
                    flag = true;
            }
            if (flag)
                return false;
        }
        return true;
    }  //rabin-miller test for int64_t base numbers
};

struct combinatorics {
    class nCr {
        vector<int> &arr;
        bitset<71> flag;
        int n, r, begin, end;
        int __calls = 0;

    public:
        nCr(vector<int> &arr, int r) : arr(arr), n(arr.size()), r(r) { begin = end = 0; }

        bool nextPossible() {
            return !__calls || end < (n - 1);
        }

        int next() {
            if (!__calls++)
                return getFirst();
            if (begin == end) {
                flag[end] = false;
                flag[end + 1] = true;
                begin += 1;
                end += 1;
                while (end + 1 < n && flag[end + 1])
                    ++end;
            }
            else {
                if (begin == 0) {
                    flag[end] = false;
                    flag[end + 1] = true;
                    end -= 1;
                }
                else {
                    flag[end + 1] = true;
                    for (int i = begin; i <= end; ++i)
                        flag[i] = false;
                    for (int i = 0; i < end - begin; ++i)
                        flag[i] = true;
                    end = end - begin - 1;
                    begin = 0;
                }
            }
            return combinationSum();
        }

    private:
        int getFirst() {
            for (int i = 0; i < n; ++i)
                flag[i] = i < r;
            begin = 0;
            end = r - 1;
            return combinationSum();
        }

        int combinationSum() {
            int ans = 0;
            for (int i = 0, count = 0; i < n && count < r; ++i) {
                if (flag[i]) {
                    ans += arr[i];
                    ++count;
                }
            }
            return ans;
        }
    };
};

namespace modulo {
    using intType = ll;

    template<const intType &modSeed>
    struct __mint {
        intType val;

        __mint(ll v = 0) {
            if (v < 0) v = v % modSeed + modSeed;
            if (v >= modSeed) v %= modSeed;
            val = intType(v);
        }

        __mint(unsigned ll v) {
            if (v >= modSeed) v %= modSeed;
            val = intType(v);
        }

        __mint(int v) : __mint((ll) (v)) {}

        __mint(unsigned v) : __mint((unsigned ll) (v)) {}

        static intType inv_mod(intType a, intType m = modSeed) {
            intType g = m, r = a, x = 0, y = 1;
            while (r != 0) {
                int q = g / r;
                g %= r;
                swap(g, r);
                x -= q * y;
                swap(x, y);
            }
            return x < 0 ? x + m : x;
        }

        explicit operator int() const { return val; }

        explicit operator unsigned() const { return val; }

        explicit operator ll() const { return val; }

        explicit operator unsigned ll() const { return val; }

        explicit operator double() const { return val; }

        explicit operator long double() const { return val; }

        __mint &operator+=(const __mint &other) {
            val -= modSeed - other.val;
            if (val < 0) val += modSeed;
            return *this;
        }

        __mint &operator-=(const __mint &other) {
            val -= other.val;
            if (val < 0) val += modSeed;
            return *this;
        }

        static unsigned fast_mod(unsigned ll x, unsigned m = modSeed) {
#if !defined(_WIN32) || defined(_WIN64)
            return unsigned(x % m);
#endif
            // Optimized mod for Codeforces 32-bit machines.
            // x must be less than 2^32 * m for this to work, so that x / m fits in an unsigned 32-bit int.
            unsigned x_high = unsigned(x >> 32), x_low = unsigned(x);
            unsigned quot, rem;
            asm("divl %4\n"
            : "=a" (quot), "=d" (rem)
            : "d" (x_high), "a" (x_low), "r" (m));
            return rem;
        }

        __mint &operator*=(const __mint &other) {
            val = fast_mod((unsigned ll) (val) * other.val);
            return *this;
        }

        __mint &operator/=(const __mint &other) { return *this *= other.inv(); }

        friend __mint operator+(const __mint &a, const __mint &b) { return __mint(a) += b; }

        friend __mint operator-(const __mint &a, const __mint &b) { return __mint(a) -= b; }

        friend __mint operator*(const __mint &a, const __mint &b) { return __mint(a) *= b; }

        friend __mint operator/(const __mint &a, const __mint &b) { return __mint(a) /= b; }

        __mint &operator++() {
            val = val == modSeed - 1 ? 0 : val + 1;
            return *this;
        }

        __mint &operator--() {
            val = val == 0 ? modSeed - 1 : val - 1;
            return *this;
        }

        __mint operator++(int) {
            __mint before = *this;
            ++*this;
            return before;
        }

        __mint operator--(int) {
            __mint before = *this;
            --*this;
            return before;
        }

        __mint operator-() const { return val == 0 ? 0 : modSeed - val; }

        friend bool operator==(const __mint &a, const __mint &b) { return a.val == b.val; }

        friend bool operator!=(const __mint &a, const __mint &b) { return a.val != b.val; }

        friend bool operator<(const __mint &a, const __mint &b) { return a.val < b.val; }

        friend bool operator>(const __mint &a, const __mint &b) { return a.val > b.val; }

        friend bool operator<=(const __mint &a, const __mint &b) { return a.val <= b.val; }

        friend bool operator>=(const __mint &a, const __mint &b) { return a.val >= b.val; }

        friend bool operator!(const __mint &a) { return !a.val; }

        friend bool operator&&(const __mint &a, const __mint &b) { return a.val && b.val; }

        friend bool operator||(const __mint &a, const __mint &b) { return (a.val || b.val) % modSeed; }

        friend bool operator^(const __mint &a, const __mint &b) { return (a.val ^ b.val) % modSeed; }

        __mint inv() const { return inv_mod(val); }

        __mint pow(ll p) const {
            if (p < 0)
                return inv().pow(-p);

            __mint a = *this, result = 1;

            while (p > 0) {
                if (p & 1)
                    result *= a;

                p >>= 1;

                if (p > 0)
                    a *= a;
            }

            return result;
        }

        friend ostream &operator<<(ostream &os, const __mint &m) { return os << m.val; }

//    private: //! Use only when needed to avoid silly bugs
        friend intType operator%(const __mint &a, const __mint &b) { return (a.val % b.val) % modSeed; }
    };

    template<const intType &modSeed>
    struct metrics {
        using mint = __mint<modSeed>;
        vector<mint> inv, fac, invFac;
        ll prepared_maximum = -1;

        explicit metrics(ll maximum) {
            static ll prepare_calls = 0;
            if (prepare_calls++ == 0) {
                // Make sure modSeed is prime, which is necessary for the inverse algorithm below.
                for (ll p = 2; p * p <= modSeed; p += p % 2 + 1)
                    assert(modSeed % p != 0);
                inv = {0, 1};
                fac = invFac = {1, 1};
                prepared_maximum = 1;
            }

            if (maximum > prepared_maximum) {
                inv.resize(maximum + 1);
                fac.resize(maximum + 1);
                invFac.resize(maximum + 1);

                for (ll i = prepared_maximum + 1; i <= maximum; i++) {
                    inv[i] = inv[modSeed % i] * (modSeed - modSeed / i);
                    fac[i] = i * fac[i - 1];
                    invFac[i] = inv[i] * invFac[i - 1];
                }

                prepared_maximum = (ll) (maximum);
            }
        }

        mint nCr(ll n, ll r) {
            if (r < 0 || r > n) return 0;
            return fac[n] * invFac[r] * invFac[n - r];
        }

        mint inv_nCr(ll n, ll r) {
            assert(0 <= r && r <= n);
            return invFac[n] * fac[r] * fac[n - r];
        }

        mint nPr(ll n, ll r) {
            if (r < 0 || r > n) return 0;
            return fac[n] * invFac[n - r];
        }

        mint inv_nPr(ll n, ll r) {
            assert(0 <= r && r <= n);
            return invFac[n] * fac[n - r];
        }
    };
}
