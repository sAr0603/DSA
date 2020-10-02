/*/---------------------------imports----------------------/*/
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

using namespace __gnu_pbds;
using namespace std;
/*/---------------------------imports----------------------/*/


/*/---------------------------macros----------------------/*/
#define int long long
#define endl '\n'
#define MOD 1000000007
#define isOdd(x) ((x)&1)
#define isEven(x) (!((x)&1))
#define all(x) (x).begin(), (x).end()
#define read(...) int __VA_ARGS__;__read(__VA_ARGS__)
#define TT template <class T>
#define LR template <class L, class R>
#define TV template<typename T, typename... V>
#define deb(...) cerr << "L(" << __LINE__ << ")*[" << #__VA_ARGS__ << "]: [", __write(__VA_ARGS__)
#define __timeStart auto __start_time = chrono::high_resolution_clock::now()
#define __timeEnd cout << "elapsed_time = " << (chrono::duration_cast<chrono::milliseconds>\
                  (chrono::high_resolution_clock::now() - __start_time).count()) << " ms\n"
/*/---------------------------macros----------------------/*/


/*/---------------------------STL overloaded I/O----------------------/*/
TT istream &operator>>(istream &is, vector<T> &V);

TT ostream &operator<<(ostream &os, vector<T> &V);

TT ostream &operator<<(ostream &os, unordered_set<T> &S);

TT ostream &operator<<(ostream &os, multiset<T> &S);

TT ostream &operator<<(ostream &os, set<T> &S);

LR ostream &operator<<(ostream &os, pair<L, R> &P);

LR ostream &operator<<(ostream &os, map<L, R> &M);

LR ostream &operator<<(ostream &os, unordered_map<L, R> &M);

TT istream &operator>>(istream &is, vector<T> &V) {
    for (auto &e : V)
        is >> e;
    return is;
}

TT ostream &operator<<(ostream &os, vector<T> &V) {
    os << "[ ";
    for (auto &v : V)
        os << v << " ";
    return os << "]";
}

TT ostream &operator<<(ostream &os, unordered_set<T> &S) {
    os << "{ ";
    for (auto &s : S)
        os << s << " ";
    return os << "}";
}

TT ostream &operator<<(ostream &os, multiset<T> &S) {
    os << "{ ";
    for (auto &s : S)
        os << s << " ";
    return os << "}";
}

TT ostream &operator<<(ostream &os, set<T> &S) {
    os << "{ ";
    for (auto &s : S)
        os << s << " ";
    return os << "}";
}

LR ostream &operator<<(ostream &os, pair<L, R> &P) {
    return os << "(" << P.first << "," << P.second << ")";
}

LR ostream &operator<<(ostream &os, map<L, R> &M) {
    os << "{ ";
    for (auto &m : M)
        os << "(" << m.first << ":" << m.second << ") ";
    return os << "}";
}

LR ostream &operator<<(ostream &os, unordered_map<L, R> &M) {
    os << "{ ";
    for (auto &m : M)
        os << "(" << m.first << ":" << m.second << ") ";
    return os << "}";
}

template<typename T, size_t arrSize>
ostream &operator<<(ostream &os, array<T, arrSize> &A) {
    os << "[ ";
    for (auto &a : A)
        os << a << " ";
    return os << "]";
}

/*/---------------------------STL overloaded I/O----------------------/*/


/*/---------------------------I/O Ports----------------------/*/
void __write() {
    cerr << "]\n";
}

TV void __write(T &t, V &... v) {
    cerr << t;
    if (sizeof...(v)) cerr << ", ";
    __write(v...);
}

TT void __read(T &x) {
    cin >> x;
}

TV void __read(T &t, V &... v) {
    __read(t);
    __read(v...);
}

int power(int x, int y) {
    int res = 1;
    while (y)
        res = !(y & 1) ?: res * x, y >>= 1, x *= x;
    return res;
}
/*/---------------------------I/O Ports----------------------/*/
