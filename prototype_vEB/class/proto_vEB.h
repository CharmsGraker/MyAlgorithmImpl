//
// Created by DELL on 2021/3/22.
//

#ifndef PROTOTYPE_VEB_PROTO_VEB_H
#define PROTOTYPE_VEB_PROTO_VEB_H
#include <vector>
#include <cmath>
using namespace std;

//summary概括这个vEBNode所含键值情况，是在前半部分还是在后半部分
//cluster基础为4位
struct vEBNode{
    int u;
    vector<int> A;
    vector<vEBNode*> cluster;
    vEBNode* summary;
    //vEBNode_not_leaf
    vEBNode():u(0),cluster(vector<vEBNode*>(0)),summary(nullptr){};
    explicit vEBNode(int x):summary(nullptr),A(vector<int>(0)),cluster(vector<vEBNode*>(0)){
        if(x<=2){
            u=2;
            A=vector<int>(2,0);
        }else{
            u=x;
            cluster=vector<vEBNode*>(sqrt(u), nullptr);
            return;
        }
    }
    int index(int x,int y){
        return x*sqrt(this->u)+y;
    }
    int high(int x);
    int low(int x);
};

class proto_vEB {
private:

    //计算值
public:
    vEBNode* root;

    proto_vEB(int _u){
        root= nullptr;
        root=init(_u);
    };
    vEBNode* init(int range);
    bool member(vEBNode* V,int x);
    int minimum(vEBNode* V);
    int minimum(){
        return minimum(root);
    }
    void insert(vEBNode* &V,int val);
    void insert(int val){
        insert(root,val);
    }
    int maximum();
};


#endif //PROTOTYPE_VEB_PROTO_VEB_H
