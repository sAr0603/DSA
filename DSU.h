#include "myLibrary.h"

class dsu {
    gp_hash_table<int, int> parentOf,
            rankOf,//rankOf[Dx] = longest path to the furthest leaf node
    minOf,//minOf[Dx] = minimum element of the set with representative el Dx
    maxOf,//maxOf[Dx] = maximum element of the set with representative el Dx
    sizeOf;//sizeOf[Dx] = number of elements of the set with representative el Dx
    int n;

    inline bool isRoot(int x) {
      return parentOf[x] == x;//self loop
    }

    template<typename mapType>
    //map.contains(x) fn of cpp20
    static inline bool mapHas(mapType &map, int key) {
      return map.find(key) != map.end();
    }

    inline void delParent(int D) {
      maxOf.erase(D), minOf.erase(D), rankOf.erase(D), sizeOf.erase(D);
    }

    inline void mergeProps(int Dx, int Dy) {//merge properties of the two sets
      sizeOf[Dy] += sizeOf[Dx];
      minOf[Dy] = min(minOf[Dx], minOf[Dy]);
      maxOf[Dy] = max(maxOf[Dx], maxOf[Dy]);
    }

public:
    explicit dsu(const v<int> &p = {}) : n(p.size() + 1) {//1-indexed representation
      if (n == 1)
        return;

      for (int i = 1; i < n; ++i) {
        parentOf[i] = p[i - 1];
      }

      for (auto &[x, ignore]: parentOf) {
        find(x);
      }//initial tree compression

      for (auto &[x, Dx] :  parentOf) {
        if (isRoot(x)) {
          minOf[Dx] = INT_MAX;
          maxOf[Dx] = INT_MIN;
          rankOf[Dx] = 0;
          sizeOf[Dx] = 0;
        }//initialising
      }

      for (auto &[x, Dx] :  parentOf) {
        minOf[Dx] = min(minOf[Dx], x);
        maxOf[Dx] = max(maxOf[Dx], x);
        sizeOf[Dx]++;
        rankOf[Dx] = rankOf[Dx] || x != Dx;
      }
    }

    void insertNew(int x, int Dx) {
      if (mapHas(parentOf, Dx))
        assert(isRoot(Dx));//make sure Dx is a representative element only
      else
        makeSet(Dx);
      parentOf[x] = Dx;//directly attach newNode to Dx : Path compression heuristic
      rankOf[x] = 0;//newNode is leaf of rankOf->0
      rankOf[Dx] = rankOf[Dx] || 1;//update rankOf of parentOf if not already atleast 1
      sizeOf[Dx]++;
      minOf[Dx] = min(minOf[Dx], x), maxOf[Dx] = max(maxOf[Dx], x);//set max/min elements
    }

    void insertNew(const v<int> &listOfX, int Dx) {//fn overloading both for single el and list of el
      for (auto &x : listOfX)
        insertNew(x, Dx);
    }

    bool isPresent(int D) {//set with rep el D is present or not
      return rankOf.find(D) != rankOf.end();
    }

    void makeSet(int D) {
      assert(!isPresent(D));
      parentOf[D] = D;
      rankOf[D] = 0;
      maxOf[D] = INT_MIN;
      minOf[D] = INT_MAX;
    }

    int getMin(int D) {
      D = find(D);//if not rep el then find the set in which D belongs
      return minOf[D];
    }

    int getMax(int D) {
      D = find(D);//if not rep el then find the set in which D belongs
      return maxOf[D];
    }

    int getSize(int D) {
      D = find(D);//if not rep el then find the set in which D belongs
      return sizeOf[D];
    }

    int find(int x) {//Amortized T->O(log*n)
      if (isRoot(x))
        return x;//rep el found
      return parentOf[x] = find(parentOf[x]);//?Path Compression
      //!directly linking all children/grandchildren of Dx with Dx itself rather than coming through a path
    }//O(log*n)

    void Union(int x, int y) {//Amortized T with Path compression->O(a(m,n))
      auto Dx = find(x), Dy = find(y);
      assert(Dx != Dy);//both not of the same set

      if (rankOf[Dx] > rankOf[Dy])//?newRoot-> Dy
        swap(Dx, Dy);//if this is not done then newH = max(Dx.h,Dy.h) + 1.
      // Since the bigger Dx root will be now a child of Dy. So Dx is effectively shifted one level below
      parentOf[Dx] = Dy;//?assertion-> newH   = max(Dx.h,Dy.h) true for all cases the one below!
      //?NOTE : ranks remains same for the repElement(i.e,newRoot) except the one below!
      if (rankOf[Dx] == rankOf[Dy])//!worst case of rankOf heuristics
        rankOf[Dy]++;
      //set new merged properties
      mergeProps(Dx, Dy);
      //since Dx is no longer a parent
      delParent(Dx);
    }//!O(log*n) .Total T = O(nlog*n) -> T(n)//Since its fastens up with more queries

};
