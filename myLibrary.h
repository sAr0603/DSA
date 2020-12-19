/*/---------------------------imports----------------------/*/
#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>

using namespace __gnu_pbds;
using namespace std;
/*/---------------------------imports----------------------/*/

/*/---------------------------macros----------------------/*/
#define endl '\n'

using ll = long long;
using lf = long double;
template<typename T>
using v = vector<T>;

const int MOD = 1000000007;
#define isOdd(x) ((x) &1)
#define isEven(x) (!((x) &1))
#define all(x) (x).begin(), (x).end()

#define read(...) \
  ll __VA_ARGS__; \
  __read(__VA_ARGS__)
#ifndef ONLINE_JUDGE
#define deb(...) cerr << "L(" << __LINE__ << ")*[" << #__VA_ARGS__ << "]: [", __write(__VA_ARGS__)
#else
#define deb(...)
#endif
#define __timeStart auto __start_time = chrono::high_resolution_clock::now()
#define __timeEnd                                                                                     \
  auto __stop_time = std::chrono::high_resolution_clock::now();                                       \
  auto __duration = std::chrono::duration_cast<std::chrono::nanoseconds>(__stop_time - __start_time); \
  cerr << "Time taken : " << ((long double) __duration.count()) / ((long double) 1e9) << "s " << endl
/*/---------------------------macros----------------------/*/

/*/---------------------------STL overloaded I/O----------------------/*/
template<class T>
istream &operator>>(istream &is, v<T> &V);

template<class T>
ostream &operator<<(ostream &os, const v<T> &V);

template<class T>
ostream &operator<<(ostream &os, const unordered_set<T> &S);

template<class T>
ostream &operator<<(ostream &os, const multiset<T> &S);

template<class T>
ostream &operator<<(ostream &os, const set<T> &S);

template<class L, class R>
ostream &operator<<(ostream &os, const pair<L, R> &P);

template<class L, class R>
ostream &operator<<(ostream &os, const map<L, R> &M);

template<class L, class R>
ostream &operator<<(ostream &os, const unordered_map<L, R> &M);

template<class T, size_t S>
ostream &operator<<(ostream &os, array<T, S> &A);

template<class T, size_t S>
istream &operator<<(istream &is, array<T, S> &A);

template<class T>
istream &operator>>(istream &is, v<T> &V) {
  for (auto &e : V)
    is >> e;
  return is;
}

template<class T>
ostream &operator<<(ostream &os, const v<T> &V) {
  auto matrixPrint = [&](v<T> V) {
      cout << "[ ";
      for (int i = 0; i < V.size(); ++i) {
        if (i)
          os << "  ";
        os << V[i];
        if (i != V.size() - 1)
          os << endl;
      }
      os << "]" << endl;
  };
  if (typeid(T) == typeid(v<int>) || typeid(T) == typeid(v<ll>) || typeid(T) == typeid(v<float>) ||
      typeid(T) == typeid(v<lf>) || typeid(T) == typeid(v<bool>)) {
    matrixPrint(V);
    return os;
  }
  os << "[ ";
  for (const auto &e : V)
    os << e << " ";
  return os << "]";
}

template<class T>
ostream &operator<<(ostream &os, const unordered_set<T> &S) {
  os << "{ ";
  for (auto &s : S)
    os << s << " ";
  return os << "}";
}

template<class T>
ostream &operator<<(ostream &os, const multiset<T> &S) {
  os << "{ ";
  for (auto &s : S)
    os << s << " ";
  return os << "}";
}

template<class T>
ostream &operator<<(ostream &os, const set<T> &S) {
  os << "{ ";
  for (auto &s : S)
    os << s << " ";
  return os << "}";
}

template<class L, class R>
ostream &operator<<(ostream &os, const pair<L, R> &P) {
  return os << "(" << P.first << "," << P.second << ")";
}

template<class L, class R>
ostream &operator<<(ostream &os, const map<L, R> &M) {
  os << "{ ";
  for (auto &m : M)
    os << "(" << m.first << ":" << m.second << ") ";
  return os << "}";
}

template<class L, class R>
ostream &operator<<(ostream &os, const unordered_map<L, R> &M) {
  os << "{ ";
  for (auto &m : M)
    os << "(" << m.first << ":" << m.second << ") ";
  return os << "}";
}

template<class T, size_t S>
ostream &operator<<(ostream &os, const array<T, S> &A) {
  os << "[ ";
  for (auto &a : A)
    os << a << " ";
  return os << "]";
}

template<class T, size_t S>
istream &operator<<(istream &is, array<T, S> &A) {
  for (auto &a : A)
    is >> a;
  return is;
}
/*/---------------------------STL overloaded I/O----------------------/*/

/*/---------------------------I/O Ports----------------------/*/

void __write() {
  cerr << "]\n";
}

template<class T, class... V>
void __write(const T &t, const V &... other) {
  cerr << t;
  if (sizeof...(other))
    cerr << ", ";
  __write(other...);
}

template<class T>
void __read(T &x) {
  cin >> x;
}

template<class T, class... V>
void __read(T &t, V &... other) {
  __read(t);
  __read(other...);
}
/*/---------------------------I/O Ports----------------------/*/

/*/---------------------------myFunctions----------------------/*/
ll power(ll x, ll y, ll mod = LLONG_MAX) {
  ll res = 1;
  x = x % mod;
  while (y > 0) {
    if (y & 1)
      res = (res * x) % mod;
    y = y >> 1;
    x = (x * x) % mod;
  }
  return res;
}

mt19937_64 __MT19337GEN__(chrono::steady_clock::now().time_since_epoch().count());

inline int64_t random(ll l, ll r) {
  uniform_int_distribution<ll> generator(l, r);
  return generator(__MT19337GEN__);
}

inline int64_t random() {
  uniform_int_distribution<ll> generator(LLONG_MIN, LLONG_MAX);
  return generator(__MT19337GEN__);
}
/*/---------------------------myFunctions----------------------/*/
