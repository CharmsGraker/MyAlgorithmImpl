//
// Created by Administrator on 2021/3/19.
//

#include "FIB_Heap.h"
#include <iostream>
#include <unordered_map>
//节点方法
HeapNode* nil= nullptr;
void FIB_Heap::add_child(HeapNode* p,HeapNode *y) {
    y->parent=p;
    ++(p->degree);
    if(p->child==nil){
        p->child=y;
        y->left=y;
        y->right=y;
    }else{
        y->right=p->child->right;
        y->right->left=y;
        y->left=p->child;
        y->left->right=y;
    }
    //cout<<"\n"<<y->val<<" inserted to"<<this->val<<"\n";
}
void FIB_Heap::remove_child(HeapNode* &p,HeapNode *&x) {
    //待删节点x是孩子是由外部调用者保证的
    if(x->left==x&&x->right==x){
        p->child=nil;
    }else{
        x->right->left=x->left;
        x->left->right=x->right;
        if(x==p->child){
            p->child=x->right;
        }
        x->left=nil;
        x->right=nil;
    }
}

//堆方法
//注：插入总是插入到根里的，仅当被抽取的时候才会合并
bool FIB_Heap::isempty() const{
    return  n==0;
}
ostream &operator <<(ostream &os,FIB_Heap &H){
    //操作的类对象必须是引用！！！！！！！被整惨了，
    // 不然永远都是对副本操作！！
    if(!H.isempty()) os<<H.extract_min();
    return os;
}
void FIB_Heap::InitNil(){
    if(nil==nullptr)nil=new HeapNode(INT_MIN);
}
FIB_Heap::FIB_Heap():n(0),rootN(0){
    InitNil();
    min=nil;
}
HeapNode*& FIB_Heap::minimum(){
    return min;
}
void FIB_Heap::exchange(HeapNode *&x, HeapNode *&y) {
    auto temp=x;
    x=y;
    y=temp;
}
void concatenate_root(HeapNode* rootA,HeapNode* rootB){
    HeapNode* p=rootA,*q=rootB;
    p->left->right=q;
    q->left->right=p;
    q->left=p->left;
    p->left=q;
}

int FIB_Heap::extract_min(){
    //由调用者保证一定可以抽取当前堆
    auto z=min;
    if(z==nil) return INT_MIN;
    //让最小节点的孩子全部提升至根列表
    if(z->child!=nil) {
        auto p = z->child;
        int i = 0;
        while (i < z->degree) {
            auto next = p->right;
            //cout<<p->val;
            insert2rootList(p);
            p->parent = nil;
            //cout<<"up child to rootlist";
            p = next;
            ++i;
        }
        //移除z
    }
        z->right->left=z->left;
        z->left->right=z->right;//z->Left   z   z->right
        if(z==z->right){
            ////cout<<"no node now\n";
            min=nil;
        }else{
            ////cout<<"\nconsolidate:";
            min=z->right;
            --rootN;
            //cout<<min->val;
            consolidate();
        }
        --n;
    return z->val;
}
void FIB_Heap::consolidate(){
    //degree vector
    vector<HeapNode*> A(D(n),nil);
    auto p=min;//只有一个根树就不用合并了
    int cnt=0;
    //把根中的一个节点移除之后，不好设置退出条件了，暂且用unorder—_map
    //最好的是把其他节点插到靠左的节点，即环形链表为升序
    //难不成是min被合并进孩子了？因为从extractmin进来的min并不一定是最小
    //根节点都是nil的孩子
    while(cnt<rootN){
        //可能visit那个元素变成孩子了，那也不可能退出循环来了
        auto pnext=p->right;
        auto x=p;
        //cout<<"cnt"<<cnt;
        int d=x->degree;
        //寻找相同度，把他们合并
        while(A[d]!=nil){
            auto y=A[d];
            //cout<<"\nA["<<d<<"]="<<" "<<A[d]->val<<" "<<x->val<<endl;
            if(x->val>y->val){
                exchange(x,y);
            }
            link(x,y);//将y连接到x孩子
            //y被合并了，不出现在根里了，
            A[d]=nil;
            d=d+1;
        }
        A[d]=x;
        p=pnext;
        ++cnt;
    }
    min=nil;
    rootN=0;
    //cout<<"\nrerange root list:\n";
    //重找Min,需要重新设置rootList，准确的来讲是更新
    min->parent=nil;
    for(int i=0;i<D(n);++i){
        if(A[i]!=nil) {
            ++rootN;
            if(min==nil)min=A[i];
            else if(A[i]->val<min->val){
                min=A[i];
            }
        }
    }
    //cout<<"consolidate done!\n";
}
void FIB_Heap::link(HeapNode* &x,HeapNode* &y){
    //x.key<=y.key y成为x的孩子
    //保证链接孩子总是将右侧的链接到左侧
        //remove from root list
        y->left->right=y->right;
        y->right->left=y->left;
//        cout<<"\ny.left"<<y->left->val<<"y.val"<<y->val;
//        cout<<"y.right"<<y->right->val<<endl;
//        cout<<y->left->right->val;
//        cout<<y->right->left->val;
        y->left=nil;
        y->right=nil;

        add_child(x,y);
        y->mark=false;
        //cout<<"link done\n";
}

void FIB_Heap::_union(class FIB_Heap &H1,class  FIB_Heap &H2){
    if(H1.isempty()&&H2.isempty()) return;
    auto H1left=H1.min->left;
    H1.min->left=H2.min->left;
    H1.min->left->right=H1.min;
    H1left->right=H2.min;
    H1left->right->left=H1left;

    cout<<"hello";
    if(H1.isempty()||H2.min->val<H1.min->val){
        this->min=H2.min;
    } else{
        this->min=H1.min;
    }
    if(this->min==nil){
        return;
    }
    this->rootN=H1.rootN+H2.rootN;
    this->n=H1.n+H2.n;
}
void FIB_Heap::insert(HeapNode* x){
    x->degree=0;
    x->parent=nil;
    x->left=nil;
    x->right=nil;
    x->child=nil;
    x->mark= false;
    //插入到根列表里
    insert2rootList(x);
    if(x->val<min->val){
        min=x;
    }
    ++n;
}
HeapNode* FIB_Heap::insert(int val) {
    auto z=new HeapNode(val);
    insert(z);
    return z;
}
void FIB_Heap::insert2rootList(HeapNode* x){
    ++rootN;
    if(min==nil){
        x->left=x;
        x->right=x;
        x->parent=nil;
        min=x;
    }else{
        x->right=min->right;
        x->left=min;
        x->right->left=x;
        x->left->right=x;
        x->parent=nil;
    }
    //cout<<x->left->val<<x->val<<x->right->val;

    //cout<<x->val<<"insert successfully!\n";
}
void FIB_Heap::deleteNode(HeapNode* &x){
    if(min==nil){
        cout<<"Heap is empty.You can't delete more.";
        return;
    }
    //将最小值减至TYPE_MIN,然后抽取掉它
    decrease_key(x,INT_MIN);
    extract_min();
}
void FIB_Heap::decrease_key(HeapNode* &x,int key){
    if(key>x->val){
        cout<<"you can't change val above the node key!"<<endl;
        return;
    }
    x->val=key;
    //如果减值后还是能够成为y的孩子，即x.key>=y.key,那就不动
    //如果这不是个根节点
    auto y=x->parent;
    if(y!=nil&&x->val<y->val){
        cut(y,x);
        cascading_cut(y);
    }if(x->val<min->val){
        min=x;
    }
}
void FIB_Heap::cut(HeapNode* &y,HeapNode* &x){
    insert2rootList(x);
    x->mark=false;
    remove_child(y,x);
}
void FIB_Heap::cascading_cut(HeapNode* y){
    auto z=y->parent;
    if(z!=nil){
        if(y->mark== false){
            y->mark= true;
        }else{
            cut(z,y);
            cascading_cut(z);
        }
    }
}
