#include <bits/stdc++.h>

using namespace std;
#define all(x) (x).begin(), (x).end()
#define isOdd(x) ((x)&1)

/*Making them global to enable maximum size possible -> Part of Iterative segNode class*/
array<int, (int) (4 * 2 * 10e5)> segTree; // 1 - index based segTree
array<int, (int) (4 * 2 * 10e5)> __lazy;//Change size of segTree by getting proper value from segSize(int n) fn
/*IF YOU COMMENT OUT __lazy then fns depending on it will throw compiler errors*/


/*/---------------------------iterative-non-lazy----------------------/*/

class segNode {
    int n;
/*/---------------------------Identity&Operation----------------------/*/
    const int EMPTY = numeric_limits<int>::max(); //Identity element

    inline int Q(int &a, int &b) {
        return min(a, b);
    }//Binary Operator Q
/*/---------------------------Identity&Operation----------------------/*/

/*/---------------------------microservices----------------------/*/

//Iterative Normal fns

//TODO https://www.youtube.com/watch?v=Oq2E2yGadnU&t [explanation of the method]
    int rangeQuery(int l, int r) {
        //by default this fn computes in range [l,r) , add +1 to r to -> [l,r]
        l += n - 1, r += n + 1;
        auto qAns = EMPTY;
        while (l < r) {
            if (isOdd(l)) //if l is odd then in upper layer of calculation it wont be counted
                qAns = Q(qAns, segTree[l++]);//Hence operate on lth node at present level only
            if (isOdd(r))
                qAns = Q(qAns, segTree[--r]);//same -> take example of 8 sized array
            l = __parentOf(l), r = __parentOf(r);//float up the range to corresponding parents
        }
        return qAns;
    }

//Very intuitive , same as __buildTree method but working with only the ancestors of the segTree[pos]
    void pointUpdate(int pos, int value) {
        segTree[pos += n] = value; // change value at desired leaf node
        while (pos > 1) {//have hit root
            pos = __parentOf(pos);//moving upto parent
            segTree[pos] = Q(segTree[__rightOf(pos)], segTree[__leftOf(pos)]);//recompute till root
        }
    }

//Recursive lazy fns : iterative commit needed :)

/*/---------------------------microservices----------------------/*/

/*/---------------------------constructors----------------------/*/
public:
    segNode() {
        fill(all(segTree), EMPTY);
    }//static constructor

    void build(vector<int> &arr) {
        fill(all(segTree), EMPTY);
        fill(all(__lazy), 0);//comment it out if not using lazy
        n = arr.size();
        __build_tree(arr);
    }//segTree builder
/*/---------------------------constructors----------------------/*/

/*/---------------------------backend----------------------/*/
private:
    void __build_tree(vector<int> &arr) {
        for (int i = 0; i < n; i++)
            segTree[n + i] = arr[i];//leaf nodes of segTree are same as all elements of the array

        for (int i = n - 1; i >= 1; i--)
            segTree[i] = Q(segTree[__leftOf(i)], segTree[__rightOf(i)]);//computing internal nodes :
        //Q[ROOT] = Q[ Q[LST] + Q[RST] ]
    }//DONT TOUCH

    static inline int __leftOf(int &index) {
        return (index << 1);
    }//NEVER TOUCH

    static inline int __rightOf(int &index) {
        return (index << 1) + 1;
    }//NEVER TOUCH

    static inline int __parentOf(int &index) {
        if (index == 1)
            cerr << "You already reached parent dumbfuck" << endl;
        return index >> 1;
    }//NEVER TOUCH
/*/---------------------------backend----------------------/*/

/*/---------------------------Interfaces----------------------/*/
public: // frontend, edit according to requirements

    template<class size>
    //template size = datatype required to accommodate max element of the array
    static size_t segSize(size_t arrSize) {
        /*if we use lazy propagation then we need O(4 * N) space else O(2 * N) works*/
        arrSize = 2 * arrSize;
        size_t __maxSize = (long double) 10e8 / ((long double) sizeof(size) / 4);
        if (arrSize > __maxSize) //max array size of int_32t type is 10e8[globally]
            cerr << "segTree not possible for given constraints" << endl;
        return arrSize;
    }

/*0 - based queries and update f(l,r) -> operate on [l,r] ,l and r inclusive*/

    int query(int l, int r) {
        return rangeQuery(l, r);
    }

    void update(int pos, int value) {
        pointUpdate(pos, value);
    }

/*/---------------------------Interfaces----------------------/*/
};

/*/---------------------------iterative-non-lazy----------------------/*/


/*/---------------------------recursive-lazy----------------------/*/

class SEG_NODE {
    vector<int> seg_tree;
    vector<int> lazy;
    vector<int> &arr;
    unsigned int n;

/*/---------------------------Identity&Operation----------------------/*/
    const int EMPTY = numeric_limits<int>::max(); //Identity element

    inline int Q(int &a, int &b) {
        return min(a, b);
    }//Binary Operator Q
/*/---------------------------Identity&Operation----------------------/*/

public:
    explicit SEG_NODE(vector<int> &arr) : arr(arr), n(arr.size()) {
        auto tree_height = 2 * closest_exp2(n) - 1;
        seg_tree.assign(tree_height, EMPTY);
        lazy.assign(tree_height, 0);
        build_tree(0, n - 1, 1);
    }

    int query(int l, int r) {
        return query(l, r, 0, n - 1, 1);
    }

    void print() {
        for (auto &i : seg_tree) {
            if (i == EMPTY)
                cout << "\\0" << '\t';
            else
                cout << i << '\t';
        }
        cout << endl;
    }

    void update(int pos, int value) {//point update
        pointUpdate(pos, value, 0, n - 1, 1);
    }

    int lazy_query(int l, int r) {
        return lazy_query(l, r, 0, n - 1, 1);
    }

    void lazy_update(int l, int r, int value) {
        lazy_update(l, r, value, 0, n - 1, 1);
    }

private:
    template<typename UnsignedType>
    UnsignedType closest_exp2(UnsignedType v) {
        static_assert(std::is_unsigned<UnsignedType>::value, "Only works for unsigned types");
        v--;
        for (size_t i = 1;
             i < sizeof(v) * CHAR_BIT; i *= 2) //Prefer size_t "Warning comparison between signed and unsigned integer"
            v |= v >> i;
        return ++v;
    }

    static inline int __leftOf(int &index) {
        return (index << 1);
    }//NEVER TOUCH

    static inline int __rightOf(int &index) {
        return (index << 1) + 1;
    }//NEVER TOUCH

    void build_tree(int start, int end, int index) {
        if (start == end) {//Leaf nodes
            seg_tree[index] = arr[start];
            return;
        }
        int mid = start + (end - start) / 2;
        /*first reaching the base case
        by calling recursively before any operations to reach base case of trivial queries*/
        build_tree(start, mid, __leftOf(index));
        build_tree(mid + 1, end, __rightOf(index));//post-order
        /*Q[ROOT] = Q[ Q[LST] + Q[RST] ]*/
        seg_tree[index] = Q(seg_tree[__leftOf(index)], seg_tree[__rightOf(index)]);
    }

    int query(int l, int r, int start, int end, int index) {
        if (l <= start && r >= end)
            return seg_tree[index];//CASE I -> Complete Overlap
        if (r < start || l > end)
            return EMPTY;//CASE III -> NO overlap
        int mid = start + (end - start) / 2;
        int left_query = query(l, r, start, mid, __leftOf(index));//CASE II -> Partial overlap
        int right_query = query(l, r, mid + 1, end, __rightOf(index));
        return Q(left_query, right_query);
    }

    void pointUpdate(int pos, int value, int start, int end, int index) {
        if (pos < start || pos > end)
            return;//CASE III -> NO overlap
        if (start == end) {
            seg_tree[index] = value;//CASE I
            return;
        }
        int mid = start + (end - start) / 2;
        pointUpdate(pos, value, start, mid, __leftOf(index));//CASE II
        pointUpdate(pos, value, mid + 1, end, __rightOf(index));
        seg_tree[index] = Q(seg_tree[__leftOf(index)], seg_tree[__rightOf(index)]);
    }//point update helper

    int lazy_query(int l, int r, int start, int end, int index) {
        seg_tree[index] += lazy[index];//resolving lazy value
        if (start != end) {// propagating any previous lazy value while visiting
            lazy[__leftOf(index)] += lazy[index];
            lazy[__rightOf(index)] += lazy[index];
        }//we are sure that we wont any gross laziness to the tree hence this much resolve is enough
        lazy[index] = 0;//lazy value resolved
        if (l <= start && r >= end)
            return seg_tree[index];//CASE I
        if (r < start || l > end)
            return EMPTY;//CASE III
        int mid = start + (end - start) / 2;
        int left_query = lazy_query(l, r, start, mid, __leftOf(index));//CASE II
        int right_query = lazy_query(l, r, mid + 1, end, __rightOf(index));
        return Q(left_query, right_query);
    }

    void lazy_update(int l, int r, int value, int start, int end, int index) {
        seg_tree[index] += lazy[index];//resolving lazy value//lazy value resolved
        if (start != end) {// propagating any previous lazy value while visiting unless we hit leaf node
            lazy[__leftOf(index)] += lazy[index];
            lazy[__rightOf(index)] += lazy[index];
        }
        lazy[index] = 0;//lazy value value at present node resolved

        if (r < start || l > end)//CASE III No overlap
            return;

        if (l <= start && r >= end) {//CASE I Complete overlap
            seg_tree[index] += value;//UPDATE
            if (start != end) {//non-leaf node
                lazy[__leftOf(index)] += value;//postponing updates as lazy value to descendants
                lazy[__rightOf(index)] += value;//Saving O(K) calls ; k = r-l
            }
            return;//lazy optimization , baad mai karenge , no resolving k nodes where k = l-r
        }

        int mid = start + (end - start) / 2;
        lazy_update(l, r, value, start, mid, __leftOf(index));//CASE II Partial overlap
        lazy_update(l, r, value, mid + 1, end, __rightOf(index));
        seg_tree[index] = Q(seg_tree[__leftOf(index)], seg_tree[__rightOf(index)]);
    }
}; //Recursive Segment Tree with lazy propagation

/*/---------------------------recursive-lazy----------------------/*/
