//
// Created by Administrator on 2021/3/19.
//

#ifndef FIB_HEAP_FIB_HEAP_H
#define FIB_HEAP_FIB_HEAP_H
#include <vector>
#include <cmath>
#include <ostream>

using namespace std;
struct HeapNode{
    int val;
    int degree;
    bool mark;
    HeapNode* child;//指向第一个孩子
    HeapNode* parent;
    HeapNode* left;
    HeapNode* right;
    explicit HeapNode(int x):val(x),degree(0),mark(false),parent(nullptr),left(nullptr),right(nullptr),child(nullptr){};
};

class FIB_Heap {
private:
    int n;
    HeapNode* min;
    int rootN;
    void cut(HeapNode*& y,HeapNode* &x);
    void cascading_cut(HeapNode* y);
    void insert2rootList(HeapNode* x);
    void consolidate();
    static void exchange(HeapNode* &x,HeapNode*& y);
    void link(HeapNode* &x,HeapNode*&y);

    void add_child(HeapNode* x,HeapNode* y);
    void remove_child(HeapNode*&p,HeapNode* &x);
public:
    FIB_Heap();
    static void InitNil();
    int count()const{return n;}
    bool isempty()const;
    static int D(int x){return (int)(1+(log(x)/log(1.618)));}
    HeapNode*& minimum();

    void insert(HeapNode* x);
    HeapNode* insert(int val);
    template<class... Args>
    void insert(int val,Args...args){
        insert(val);
        insert(args...);
    };

    int extract_min();
    void deleteNode(HeapNode *&x);
    void decrease_key(HeapNode* &,int);

    friend ostream &operator<<(ostream &os, FIB_Heap &H);
    void _union(class FIB_Heap &H1,class  FIB_Heap& H2);

    ~FIB_Heap(){};
};


#endif //FIB_HEAP_FIB_HEAP_H
