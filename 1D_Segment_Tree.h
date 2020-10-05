#ifndef
  #define all(x) (x).begin(),x.end()
#endif

class segNode {
    array<int, static_cast<int>(10e7)> segTree;
    array<int, static_cast<int>(10e7)> lazy = {0};
    int n;

/*/---------------------------Identity&Operation----------------------/*/
    const int EMPTY = LLONG_MAX; //Identity element

    inline int Q(int &a, int &b) {
        return min(a, b);
    }//Binary Operator Q
/*/---------------------------Identity&Operation----------------------/*/

/*/---------------------------AsliMaal----------------------/*/

//Iterative Normal fns
    int rangeQuery(int l, int r) {
        l += n, r += n;
        int qAns = EMPTY;
        while (l < r) {
            if (isEven(l))
                qAns = Q(qAns, segTree[l++]);
            if (isEven(r))
                qAns = Q(qAns, segTree[--r]);
            l = __parentOf(l), r = __parentOf(r);
        }
        return qAns;
    }

    void pointUpdate(int pos, int value) {
        segTree[pos += n] = value;
        while (pos > 0) {
            pos = __parentOf(pos);//moving upto parent
            segTree[pos] = Q(segTree[__rightOf(pos)], segTree[__leftOf(pos)]);
        }
    }

//Recursive lazy fns : iterative commit needed :) 
    int lazyQuery(int l, int r, int start, int end, int index) {
        segTree[index] += lazy[index];//resolving lazy value
        if (start != end) {// propagating any previous lazy value while visiting
            lazy[__leftOf(index)] += lazy[index];
            lazy[__rightOf(index)] += lazy[index];
        }//we are sure that we wont any gross laziness to the tree hence this much resolve is enough
        lazy[index] = 0;//lazy value resolved
        if (l <= start && r >= end)
            return segTree[index];//CASE I
        if (r < start || l > end)
            return EMPTY;//CASE III
        int mid = start + (end - start) / 2;
        int left_query = lazyQuery(l, r, start, mid, __leftOf(index));//CASE II
        int right_query = lazyQuery(l, r, mid + 1, end, __rightOf(index));
        return Q(left_query, right_query);
    }

    void lazyUpdate(int l, int r, int value, int start, int end, int index) {
        segTree[index] += lazy[index];//resolving lazy value//lazy value resolved
        if (start != end) {// propagating any previous lazy value while visiting unless we hit leaf node
            lazy[__leftOf(index)] += lazy[index];
            lazy[__rightOf(index)] += lazy[index];
        }
        lazy[index] = 0;//lazy value value at present node resolved

        if (r < start || l > end)//CASE III No overlap
            return;

        if (l <= start && r >= end) {//CASE I Complete overlap
            segTree[index] += value;//UPDATE
            if (start != end) {//non-leaf node
                lazy[__leftOf(index)] += value;//postponing updates as lazy value to descendants
                lazy[__rightOf(index)] += value;//Saving O(K) calls ; k = r-l
            }
            return;//lazy optimization , baad mai karenge , no resolving k nodes where k = l-r
        }

        int mid = start + (end - start) / 2;
        lazyUpdate(l, r, value, start, mid, __leftOf(index));//CASE II Partial overlap
        lazyUpdate(l, r, value, mid + 1, end, __rightOf(index));
        segTree[index] = Q(segTree[__leftOf(index)], segTree[__rightOf(index)]);
    }

/*/---------------------------AsliMaal----------------------/*/

/*/---------------------------constructors----------------------/*/
public:
    segNode() {
        fill(all(segTree), EMPTY);
    }//static constructor

    void __build(vector<int> &arr) {
        n = arr.size();
        __build_tree(arr);
    }//segTree builder
/*/---------------------------constructors----------------------/*/

/*/---------------------------gearFns----------------------/*/
private:
    void __build_tree(vector<int> &arr) {
        for (int i = 0; i < n; i++)
            segTree[n + i] = arr[i];//Leaves

        for (int i = n - 1; i >= 0; i--)
            segTree[i] = Q(segTree[__leftOf(i)], segTree[__rightOf(i)]);//Internals
        /*Q[ROOT] = Q[ Q[LST] + Q[RST] ]*/
    }//DONT TOUCH 

    static inline int __leftOf(int &index) {
        return (index << 1) + 1;
    }//NEVER TOUCH

    static inline int __rightOf(int &index) {
        return (index << 1) + 2;
    }//NEVER TOUCH 

    static inline int __parentOf(int &index) {
        if (!index)
            cerr << "You already reached parent dumbfuck" << endl;
        return (index - 1) >> 1;
    }//NEVER TOUCH
/*/---------------------------gearFns----------------------/*/

/*/---------------------------Interfaces----------------------/*/
public: // f()->bool/void , edit according to requirements
    int query(int l, int r) {
        return rangeQuery(l, r);
    }

    void update(int pos, int value) {
        pointUpdate(pos, value);
    }

    int lQuery(int l, int r) {
        return lazyQuery(l, r, 0, n - 1, 0);
    }

    void lUpdate(int l, int r, int value) {
        lazyUpdate(l, r, value, 0, n - 1, 0);
    }
    /*/---------------------------Interfaces----------------------/*/
};
