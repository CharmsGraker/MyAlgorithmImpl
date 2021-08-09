//
// Created by DELL on 2021/3/22.
//

#include "proto_vEB.h"
#define NIL -1
#include <iostream>

void proto_vEB::insert(vEBNode* &V,int val){
    if(V->u==2){
        //cout<<val;
        V->A[val]=1;
        return;
    }else{
        auto x=V->high(val);
        auto y=V->low(val);
        update(V->summary);
        insert(V->cluster[x], y);
    }
}
void update()
int vEBNode::high(int x){
    return x/sqrt(this->u);
}
int vEBNode::low(const int x){
    return x % (int)sqrt(this->u);
}

vEBNode* proto_vEB::init(int range){
    auto p = new vEBNode(range);
    if(range==2){
        return p;
    }else{

        for(int i = 0;i < p->cluster.size();++i){

            p->cluster[i] = init((int)sqrt(range));
        }
        p->summary=init(sqrt(range));
        return p;
    }
}
//查找一个元素是否在vEB树中
bool proto_vEB::member(vEBNode* V, int x) {
    if(V->u==2){
        return V->A[x];
    } else{
        return member(reinterpret_cast<vEBNode *>(V->cluster[V->high(x)]), V->low(x));
    }
}


int proto_vEB::maximum() {

}
int proto_vEB::minimum(vEBNode* V){
    //每个summary指示该节点的cluster包含键值情况。
    //并且summary所指节点的cluster大小按sqrt(u)递减
    //找最小元素，那得锁定最早出现的cluster序号，这需要通过summary知道。
    //index可计算出他在回溯节点的cluster序号
    //根节点的一层index()便是计算出键值。
    //或者每向下递归一层，我们要寻找的键值就等效于查找sqrt(key)，那么如法炮制，就这样将index一层层向上递归
    if(V->u==2){
        if(V->A[0]==1){
            return 0;//位于靠前的一个
        }if(V->A[1]==1){
            return 1;//位于靠后的一个
        }else{
            return NIL;
        }
    }else{
        auto min_cluster=minimum(V->summary);
        if(min_cluster==NIL){
            return NIL;
        }else{
            auto offset=minimum(V->cluster[min_cluster]);
            return V->index(min_cluster,offset);
        }
    };
};