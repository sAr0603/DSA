/*/---------------------------imports----------------------/*/
#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>

using namespace __gnu_pbds;
using namespace std;
/*/---------------------------imports----------------------/*/

/*/---------------------------macros----------------------/*/

#define endl '\n'
using ll = long long;
const int MOD = 10e7;
#define isOdd(x) ((x)&1)
#define isEven(x) (!((x)&1))

#define all(x) (x).begin(), (x).end()
#define read(...)    \
    int __VA_ARGS__; \
    __read(__VA_ARGS__)
#ifndef ONLINE_JUDGE
#define deb(...) cerr << "L(" << __LINE__ << ")*[" << #__VA_ARGS__ << "]: [", __write(__VA_ARGS__)
#else
#define deb(...)
#endif
#define __timeStart auto __start_time = chrono::high_resolution_clock::now()
#define __timeEnd                                                                                       \
    auto __stop_time = std::chrono::high_resolution_clock::now();                                       \
    auto __duration = std::chrono::duration_cast<std::chrono::nanoseconds>(__stop_time - __start_time); \
    cerr << "Time taken : " << ((long double)__duration.count()) / ((long double)1e9) << "s " << endl

/*/---------------------------macros----------------------/*/

/*/---------------------------STL overloaded I/O----------------------/*/
template<class T>
istream &operator>>(istream &is, vector<T> &V);

template<class T>
ostream &operator<<(ostream &os, const vector<T> &V);

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
istream &operator>>(istream &is, vector<T> &V) {
    for (auto &e : V)
        is >> e;
    return is;
}

template<class T>
ostream &operator<<(ostream &os, const vector<T> &V) {
    os << "[ ";
    for (auto &v : V)
        os << v << " ";
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

template<class T, class...V>
void __write(const T &t, const V &... v) {
    cerr << t;
    if (sizeof...(v))
        cerr << ", ";
    __write(v...);
}

template<class T>
void __read(T &x) {
    cin >> x;
}

template<class T, class...V>
void __read(T &t, V &... v) {
    __read(t);
    __read(v...);
}
/*/---------------------------I/O Ports----------------------/*/

/*/---------------------------myFunctions----------------------/*/
int power(int x, int y, int mod = LLONG_MAX) {
    int res = 1;
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

inline int64_t random(int l, int r) {
    uniform_int_distribution<int> generator(l, r);
    return generator(__MT19337GEN__);
}

inline int64_t random() {
    uniform_int_distribution<int> generator(LLONG_MIN, LLONG_MAX);
    return generator(__MT19337GEN__);
}
/*/---------------------------myFunctions----------------------/*/
