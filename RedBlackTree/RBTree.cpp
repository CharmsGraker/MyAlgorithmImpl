//
// Created by Administrator on 2021/3/16.
//
#include <iostream>
#include <iomanip>
#include "RBTree.h"

using namespace std;

RBTreeNode* nil= nullptr;

void RBTree::display() {cout<<"\nTranversal Order:"<<endl;display(root);}
RBTree::RBTree(){
    nil = new RBTreeNode{-1, nullptr, nullptr, nullptr, BLACK};
    root=nil;
}
RBTreeNode::RBTreeNode(Type val):key(val),color(RED),left(nil),right(nil),parent(nil){}
inline void RBTree::display(RBTreeNode *p) {
    if(p==nil) return;
    display(p->left);
    cout<<p->key<<setw(5);
    display(p->right);
}
RBTreeNode* RBTree::RBTreeSearch(RBTreeNode* p,Type val){
    auto z=nil;
    while(p) {
        if (p->key < val) {
            p=p->right;
        } else if(p->key==val){
            return p;
        } else p=p->left;
    }
    return z;
}
void RBTree::Delete(Type val){
    RBTreeNode* z=RBTreeSearch(root,val);
    if(z!=nil) Delete(z);
    else cout<<"\nThe "<<val<<" is not in RedBlackTree.";
};


void RBTree::left_Rotate(RBTreeNode* x) {
    //x��right����x��λ�ã�x��Ϊ��ԭ��left��λ��,x���Һ������Ϊԭ��right(y)�����ӣ�x��Ϊy(��right)������
    auto y=x->right;
     x->right=y->left;
    if(y->left!=nil)
        y->left->parent=x;//�����������������ӵ�x����֮����
    y->parent=x->parent;
    if(x->parent==nil){
        root=y;
    }//y����x����������parent��
    else if(x==x->parent->left){
        x->parent->left=y;
    }else{
        x->parent->right=y;
    }
    y->left=x;
    x->parent=y;
}
void RBTree::right_Rotate(RBTreeNode* x) {
    //y(x��left)����x��λ�ã�x��Ϊ��ԭ��right(x->right)��λ��,x���������Ϊԭ��left(y)���Һ��ӣ�x��Ϊy(��left)���Һ���
    auto y=x->left;
    x->left=y->right;
    if(y->right!=nil)
        y->right->parent=x;
    y->parent=x->parent;
    if(x->parent==nil){
        root=y;
    }else if(x==x->parent->left){
        x->parent->left=y;
    }else{
        x->parent->right=y;
    }
    y->right=x;
    x->parent=y;
}

//insert
void RBTree::insert(RBTreeNode* z) {
    //�Ƚ�z��Ϊ��ͨ�ڵ㣬��AVL����������
    auto y=nil;
    //x������ǰ·���ϵĽڵ�
    auto x=root;
    while(x!=nil){
        y=x;
        if(x->key>z->key){
            x=x->left;
        }else{
            x=x->right;
        }
    }
    z->parent=y;
    //x��Ҷ���ˣ�z���뵽y�ĺ���
    if(y==nil) root=z;//����
    else if(z->key<y->key){
        y->left=z;
    } else{
        y->right=z;
    }
    z->left=nil;
    z->right=nil;
    z->color=RED;
    //cout<<z->key;
    //cout<<"zparent"<<z->parent->key<<endl;
    INSERT_FIXUP(z);
    cout<<z->key<<" has inserted.\n";
}

void RBTree::insert(int val) {
    auto z=new RBTreeNode(val);
    insert(z);
}

//INSERT_FIXUP
void RBTree::INSERT_FIXUP(RBTreeNode* z) {
    /*
     * �����z��һ����ڵ�
     * ���z�ĸ����Ǻ�ɫ�ڵ㣬����ֱ�Ӳ��롣�����ڵ㲻������
     * ���Ҫ����while,��ôz.p�Ǻ�ɫ��֪z.pһ�����Ǹ��ڵ㣬��ôz.p.pһ��������z.p.p.color=BLACK,��Ȼz.pΪz.p.p�ĺ��ӣ�
     * ��ôz.p.pһ������Ҷ�ڵ㡣����ȴ�Ǻ�ɫ��˵��z.p.p�����ӣ������ò���Ⱦ�ɺ�ɫ����ˣ�z����ڵ�һ������
    */RBTreeNode* y=nil;
    while(z->parent->color==RED){
        if(z->parent==z->parent->parent->left){
            //cout<<"���ڵ�������"<<endl;
            y=z->parent->parent->right;
            if(y->color==RED){
                //cout<<"case a1:"<<endl;
                //case a1: z����ڵ�y�Ǻ�ɫ��
                //��z.p���ֵ�Ϊ���֪��z.p.pΪ�ڣ���ô�������ɫת�Ƶ�������ĺ��ӣ�Ҳ���Լ�������bhһ��
                z->parent->color=BLACK;
                y->color=BLACK;
                z->parent->parent->color=RED;
                z=z->parent->parent;
                //��Ҫ���ݵ���
            }
            //if(y->color==BLACK)
            else if(z==z->parent->right){
                //case a2����ת��Ϊcase 3
                //cout<<"case a2:";
                z=z->parent;
                left_Rotate(z);
            }
            else{
                //case 3
                //�����ڵ�z.p.p��������
                z->parent->color=BLACK;//��������������һ�㡣���뻭ͼ���
                z->parent->parent->color=RED;
                right_Rotate(z->parent->parent);
            }
            //������ݣ���Ϊ������ֹ���
        }else{
         //z->parent==z->parent->parent->right
            //cout<<"���ڵ����Һ���"<<endl;
            y=z->parent->parent->left;
            if(y->color==RED){
                //cout<<"case b1:"<<endl;
                //case b1: z����ڵ�y�Ǻ�ɫ��
                //��z.p���ֵ�Ϊ���֪��z.p.pΪ�ڣ���ô�������ɫת�Ƶ�������ĺ��ӣ�Ҳ���Լ�������bhһ��
                z->parent->color=BLACK;
                y->color=BLACK;
                z->parent->parent->color=RED;
                z=z->parent->parent;
                //��Ҫ���ݵ���
            }
            //if(y->color==BLACK)
            //case b2����ת��Ϊcase 3
            else if(z==z->parent->left){
                //cout<<"case b2:";
                z=z->parent;
                right_Rotate(z);
            }else{
                //case b3
                //�����ڵ�z.p.p��������
                z->parent->color=BLACK;//��������������һ�㡣���뻭ͼ���
                z->parent->parent->color=RED;
                left_Rotate(z->parent->parent);
            }
        }

    }
    root->color=BLACK;
    //��������½ڵ㣬�����˳����1������
}
RBTreeNode* RBTree::MIN_ELEMENT(RBTreeNode *z) {
    while(z->left){
        z=z->left;
    }
    return z;
}
void RBTree::RBTree_Transplant(RBTreeNode *u, RBTreeNode *v) {
    if(u->parent==nil) root=v;
    if(u==u->parent->left)u->parent->left=v;
    else  u->parent->right=v;
    v->parent=u->parent;
}
void RBTree::Delete(RBTreeNode* z) {
    //y��ָ��������Ǹ��ڵ�,xָ�����y���Ǹ��ڵ�
    auto y=z;
    RBTreeNode* x= nullptr;
    Color y_Origin_Color=y->color;
    if(z->left==nil){
        //��z������������
        x=z->right;
        RBTree_Transplant(z,z->right);
    }else if(z->right==nil){
        x=z->left;
        RBTree_Transplant(z,z->left);
    }else{
        //z��������
        y=MIN_ELEMENT(z->right);
        y_Origin_Color=y->color;
        x=y->right;
        if(y->parent==z){
            //��ôy��Ϊz��ֱϵ�Һ��ӣ���һ��û�����ӡ�����Ҫ�����棬ֱ�ӽ�y��ͬ����������z
            x->parent=y;
        }else{
            RBTree_Transplant(y,y->right);
            y->right=z->right;
            //��z->right�������ӵ�y
            y->right->parent=y;
        }
        //��y����z
        RBTree_Transplant(z,y);
        y->left=z->left;
        y->left->parent=z;
        y->color=z->color;
    }
    //�ǳ�һ����˼�룬��y��BLACK�������Ƹ�x������xΪ��ڻ�˫�غ�ɫ�ڵ㡣��취�����ַǺڷǺ����ɫ˦��ȥ
    if(y_Origin_Color==BLACK) RBTree_DELETE_FIXUP(x);
}
void RBTree::RBTree_DELETE_FIXUP(RBTreeNode* x){
    //����x��ԭ�����Ǻ�ɫ����ô����ʼ���μ�x.color==RED����x�Ǹ���ڽڵ㣬x.color==BLACK����x�Ǹ�˫�غ�ɫ�ڵ�
    while(x!=root&&x->color==BLACK){
        if(x==x->parent->left){
            auto w=x->parent->right;
            if(w->color==RED){
                cout<<"case 1:";
                //�ⰵ����w���������ӣ��Ҷ�ΪBLACK
                //��ô���ǿ��ԶԸ�С�ķ�Χ���е���
                x->parent->color=RED;
                w->color=BLACK;
                left_Rotate(x->parent);
                w=x->parent->right;
            }//w.color==BLACK
            if(w->left->color==BLACK&&w->right->color==BLACK){
                cout<<"case 2";
                //��ôw����Ⱦ��RED,���൱�ڸ�x��wͬʱ��һ���ɫ��̫������
                w->color=RED;
            }else{
                if(w->right->color==BLACK){
                    cout<<"case 3:";
                    w->color=RED;
                    w->left->color=BLACK;
                    right_Rotate(w);
                    //�������Ǹ�nilҲ��������
                    w=x->parent->right;
                }
                cout<<"case 4:";
                //��ʱx->parent����ɫ����w->left����ɫ����ν
                w->color=x->parent->color;
                x->parent->color=BLACK;
                w->right->color=BLACK;
                left_Rotate(x->parent);
                x=root;
            }
        }else{
            //x==x->parent->right
            auto w=x->parent->left;
            if(w->color==RED){
                cout<<"case 1:";
                //�ⰵ����w���������ӣ��Ҷ�ΪBLACK
                //��ô���ǿ��ԶԸ�С�ķ�Χ���е���
                x->parent->color=RED;
                w->color=BLACK;
                right_Rotate(x->parent);
                w=x->parent->left;
            }//w.color==BLACK
            if(w->left->color==BLACK&&w->right->color==BLACK){
                cout<<"case 2";
                //��ôw����Ⱦ��RED,���൱�ڸ�x��wͬʱ��һ���ɫ��̫������
                w->color=RED;
            }else{
                if(w->left->color==BLACK){
                    cout<<"case 3:";
                    w->color=RED;
                    w->right->color=BLACK;
                    left_Rotate(w);
                    //�������Ǹ�nilҲ��������
                    w=x->parent->left;
                }
                cout<<"case 4:";
                //��ʱx->parent����ɫ����w->left����ɫ����ν
                w->color=x->parent->color;
                x->parent->color=BLACK;
                w->left->color=BLACK;
                right_Rotate(x->parent);
                x=root;
            }
        }
    }
    x->color=BLACK;
}


