#include "myLibrary.h"

namespace numTheory {

inline int bitLength(int n) {
	return n ? log2(n) + 1 : 0;
}

int fastMod(int a, unsigned int b, int m) {
	if (!b) return 1;
	a %= m;
	if (b == 1 || !a)
		return a;
	int res = 1;
	while (b > 0) {
		if (b & 1)
			res = (res * a) % m;
		b >>= 1;
		a = (a * a) % m;
	}
	return res;
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
};  // namespace numTheory

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
			int p = numTheory::fastMod(i, d, n);
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

class metrics {
#define __MAX_NCR_SIZE__ (int)10e3
    int mod;
public:
    array<int, __MAX_NCR_SIZE__ + 1> fac;

    metrics(int mod = LLONG_MAX) : mod(mod) {
        fac[0] = 1;
        for (int i = 1; i <= __MAX_NCR_SIZE__; ++i)
            fac[i] = (i * fac[i - 1]) % mod;
    }

    inline int modInverse(int n) {
        return power(n, mod - 2, mod);
    }

    int nCr(int n, int r) {
        assert(n <= __MAX_NCR_SIZE__);
        if (r == 0)
            return 1;
        return (fac[n] * modInverse(fac[n - r]) % mod) % mod;
    }

    int nPr(int n, int r) {
        return (nCr(n, r) * fac[r]) % mod;
    }
};
};
