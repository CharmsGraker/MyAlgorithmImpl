//
// Created by Administrator on 2021/3/16.
//

#ifndef REDBLACKTREE_RBTREE_H
#define REDBLACKTREE_RBTREE_H
typedef  enum{RED=0,BLACK=1} Color;
typedef int Type;

struct RBTreeNode{
    Type key;
    RBTreeNode* parent;
    RBTreeNode* left;
    RBTreeNode* right;
    Color color;
    explicit RBTreeNode(Type val);
    RBTreeNode(Type a,RBTreeNode* p,RBTreeNode* l,RBTreeNode* r,Color cl):key(a),parent(p),left(l),right(r),color(cl){}
};

class RBTree{
private:
    //nilͳһ����
    RBTreeNode* root;
    void RBTree_DELETE_FIXUP(RBTreeNode* x);
    void RBTree_Transplant(RBTreeNode* u,RBTreeNode* v);
    void INSERT_FIXUP(RBTreeNode* z);
    void left_Rotate(RBTreeNode* x);
    void right_Rotate(RBTreeNode* x);
public:
    RBTree();
    void insert(RBTreeNode* z);

    template<typename... Args>
    void insert(Type val,Args... args){
        insert(val);
        insert(args...);
    };
    void insert(Type val);

    void Delete(RBTreeNode* z);
    void Delete(Type val);
    RBTreeNode* MIN_ELEMENT(RBTreeNode* z);
    RBTreeNode* RBTreeSearch(RBTreeNode* root,Type val);
    static void display(RBTreeNode* p);
    void display();

    ~RBTree(){};
};
#endif //REDBLACKTREE_RBTREE_H
