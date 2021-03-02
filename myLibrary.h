/*/---------------------------imports----------------------/*/
#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
/*/---------------------------imports----------------------/*/

namespace myLib {
/*/---------------------------macros----------------------/*/
    using namespace __gnu_pbds;
    using namespace std;
    using ll = long long;
    using lf = long double;

    template<typename T = int>
    using v = vector<T>;

    template<typename T = int, typename fn = greater<>>
    using min_heap = priority_queue<T, v<T>, fn>;

    template<typename T = int, typename V = int, typename... Rest>
    using umap = unordered_map<T, V, Rest...>;

    template<typename T = int, typename... Rest>
    using uset = unordered_set<T, Rest...>;

    template<typename T = int, typename V = int, typename... Rest>
    using tup = tuple<T, V, Rest...>;

    const ll MOD = 1000000007;

#define endl '\n'
#define mt make_tuple
#define isOdd(x) ((x)&1)
#define isEven(x) (!((x)&1))
#define all(x) (x).begin(), (x).end()
#define allr(x) (x).rbegin(), (x).rend()

#define read(...)   \
    ll __VA_ARGS__; \
    __read(__VA_ARGS__)
#ifndef ONLINE_JUDGE
#define deb(...) cerr << "L(" << __LINE__ << ")*[" << #__VA_ARGS__ << "]: [", __write(__VA_ARGS__)
#else
#define deb(...)
#endif
#define __timeStart auto __start_time = chrono::high_resolution_clock::now()
#define __timeEnd                                                                             \
    auto __stop_time = chrono::high_resolution_clock::now();                                  \
    auto __duration = chrono::duration_cast<chrono::nanoseconds>(__stop_time - __start_time); \
    cerr << "Time taken : " << ((long double)__duration.count()) / ((long double)1e9) << "s " << endl
#define execTime(x)  \
    {                \
        __timeStart; \
        x;           \
        __timeEnd;   \
    }
/*/---------------------------macros----------------------/*/

/*/---------------------------STL overloaded I/O----------------------/*/
    template<class T>
    istream &operator>>(istream &is, v<T> &V);

    template<class L, class R>
    istream &operator>>(istream &is, pair<L, R> &P);

    template<class T, size_t S>
    istream &operator>>(istream &is, array<T, S> &A);

    template<class Head, class... Tail>
    tuple<Head, Tail...> tuple_read_impl(istream &is);

    template<class... Types>
    istream &operator>>(istream &is, tuple<Types...> &tup);

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

    template<class T>
    ostream &operator<<(ostream &os, const queue<T> &Q);

    template<class T>
    ostream &operator<<(ostream &os, const stack<T> &S);

    template<class T>
    ostream &operator<<(ostream &os, const forward_list<T> &A);

    template<class T>
    ostream &operator<<(ostream &os, const list<T> &A);

    template<class T, size_t S>
    ostream &operator<<(ostream &os, const array<T, S> &A);

    template<class... T>
    ostream &operator<<(ostream &os, const tuple<T...> &_tup);

    template<class T>
    istream &operator>>(istream &is, v<T> &V) {
      for (auto &e : V)
        is >> e;
      return is;
    }

    template<class L, class R>
    istream &operator>>(istream &is, pair<L, R> &P) {
      cin >> P.first >> P.second;
      return is;
    }

    template<class T, size_t S>
    istream &operator>>(istream &is, array<T, S> &A) {
      for (auto &a : A)
        is >> a;
      return is;
    }

    template<class T>
    ostream &operator<<(ostream &os, const v<T> &V) {
      auto matrixPrint = [&](v<T> V) {
          os << "[" << endl;
          for (int i = 0; i < V.size(); ++i) {
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

    template<class T>
    ostream &operator<<(ostream &os, const queue<T> &Q) {
      forward_list<T> temp;
      auto Qc = Q;
      while (!Qc.empty()) {
        temp.push_front(Qc.front());
        Qc.pop();
      }
      os << temp;
      return os;
    }

    template<class T>
    ostream &operator<<(ostream &os, const stack<T> &S) {
      list<T> temp;
      auto Sc = S;
      while (!Sc.empty()) {
        temp.push_back(Sc.top());
        Sc.pop();
      }
      os << temp;
      return os;
    }

    template<class T>
    ostream &operator<<(ostream &os, const forward_list<T> &A) {
      os << "[ ";
      for (const auto &a : A)
        os << a << " ";
      return os << "]";
    }

    template<class T>
    ostream &operator<<(ostream &os, const list<T> &A) {
      os << "[ ";
      for (const auto &a : A)
        os << a << " ";
      return os << "]";
    }

    template<class TupType, size_t... I>
    ostream &tuple_print(ostream &os, const TupType &_tup, index_sequence<I...>) {
      os << "(";
      (..., (os << (I == 0 ? "" : ", ") << get<I>(_tup)));
      os << ")";
      return os;
    }

    template<class... T>
    ostream &operator<<(ostream &os, const tuple<T...> &_tup) {
      return tuple_print(os, _tup, make_index_sequence<sizeof...(T)>());
    }

    template<class Head, class... Tail>
    tuple<Head, Tail...> tuple_read_impl(istream &is) {
      Head val;
      is >> val;
      if constexpr (sizeof...(Tail) == 0)  // this was the last tuple value
        return tuple{val};
      else
        return tuple_cat(tuple{val}, tuple_read_impl<Tail...>(is));
    }

    template<class... Types>
    istream &operator>>(istream &is, tuple<Types...> &tup) {
      tup = tuple_read_impl<Types...>(is);
      return is;
    }

/*/---------------------------STL overloaded I/O----------------------/*/

/*/---------------------------I/O Ports----------------------/*/

    void __write() {
      cerr << "]\n";
    }

    template<class T, class... V>
    void __write(const T &t, const V &...other) {
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
    void __read(T &t, V &...other) {
      __read(t);
      __read(other...);
    }
/*/---------------------------I/O Ports----------------------/*/

/*/---------------------------myFunctions----------------------/*/
    struct pair_hash {
        static int betterHash(uint64_t x) {
          x += 0x9e3779b97f4a7c15;
          x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
          x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
          return x ^ (x >> 31);
        }

        template<class T1, class T2>
        size_t operator()(const pair<T1, T2> &p) const {
          auto h1 = hash<T1>{}(p.first);
          auto h2 = hash<T2>{}(p.second);
          return betterHash(h1 * 1111111 + h2);
        }
    };

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

    void fastIO() {
      ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
      cout << fixed << setprecision(25), cerr << fixed << setprecision(10);
    }

    template<typename T, typename K>
    bool itHas(const T &container, const K &key) {
      return container.find(key) != container.end();
    }

/*/---------------------------myFunctions----------------------/*/
}  // namespace myLib

namespace std {
    namespace {
        template<class T>
        inline void hash_combine(std::size_t &seed, T const &v) {
          seed ^= hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }

        template<class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
        struct HashValueImpl {
            static void apply(size_t &seed, Tuple const &tuple) {
              HashValueImpl<Tuple, Index - 1>::apply(seed, tuple);
              hash_combine(seed, get<Index>(tuple));
            }
        };

        template<class Tuple>
        struct HashValueImpl<Tuple, 0> {
            static void apply(size_t &seed, Tuple const &tuple) {
              hash_combine(seed, get<0>(tuple));
            }
        };
    }  // namespace

    template<typename... TT>
    struct hash<std::tuple<TT...>> {
        size_t
        operator()(std::tuple<TT...> const &tt) const {
          size_t seed = 0;
          HashValueImpl<std::tuple<TT...>>::apply(seed, tt);
          return seed;
        }
    };
}  // namespace std

using namespace myLib;
using namespace std;
