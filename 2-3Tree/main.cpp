#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iomanip>
using namespace std;
struct BTreeNode{
    //static int tt;
    vector<int> vals;
    vector<BTreeNode*> children;
    bool isLeaf;
    explicit BTreeNode(int tt,int n=0):isLeaf(false){
        vals=vector<int>(n);
        children=vector<BTreeNode*>(2*tt, nullptr);
    };
};
class BTree {
private:
    enum {isLeft,isRight};
    int t;
    void BTree_insert_Notfull(BTreeNode *x, int key);
    void BTree_SplitChild(BTreeNode *x, int idx);
    tuple<BTreeNode*,bool> BTree_neighborNode(const BTreeNode* x,int k);
public:
    BTreeNode *root;
    explicit BTree(int _t) : t(_t),root(nullptr) {
        //BTreeNode::tt=_t;
        cout << "You choose: t=" << t << endl;
        cout << "Now,each node can't contain over " << 2 * t - 1 << " children." << endl;
    };

    BTreeNode *CreatBTree() {
        root = new BTreeNode(t);
        root->isLeaf = true;
        return root;
    };

    tuple<BTreeNode*,int> BTree_Search(BTreeNode *x, int key);
    tuple<BTreeNode*,int> BTree_Search(int key) {return BTree_Search(root, key);};

    void BTree_insert(int key);

    template<typename... Args>
    void BTree_insert(int key,Args... args){
        BTree_insert(key);
        BTree_insert(args...);
    }

    void display(BTreeNode *x);
    void display() {
        cout<<"Show me the Order Tranverse:"<<endl;
        return display(root);
    };

    bool BTreeDelete(BTreeNode* x,int key);

    tuple<BTreeNode*,int> Forerunner(BTreeNode* x,int key);//寻找key的前驱
    tuple<BTreeNode*,int> Succeed(BTreeNode* x,int key);//寻找key的后继
    tuple<BTreeNode*,int> FindParent(BTreeNode* p,BTreeNode* x){
        if(x==root){
            return make_tuple(root,-1);
        } //根节点没有父亲，那说明我们要删的是原root
        else{
            for(int i=0;i<2*t-1;++i){
                if(p->children[i]==x){
                    return make_tuple(p,i);
                }
            }
            for(int i=0;i<2*t;++i){
                if(p->vals[i]>x->vals.back())//找到第一个p->vals[i]>x->vals.back()的,那么x至少在p的孩子那一层
                    return FindParent(p->children[i],x);
            }
            return make_tuple(nullptr,-2);
        }
    };
    void BTreeDeleteVal(int val){
        if(BTreeDelete(root,val)){
            cout<<"delete "<<val<<" successfully!"<<endl;
        }
        return;
    };
};

int main() {
    int t=3;
    auto T=new BTree(t);
    T->CreatBTree();
    //迫不得已BTree才会向上分裂，所以叶子的关键字非常密集，每2*t-1个关键字造成分裂才会在父节点新插入一个值。所以BTree容量巨大

    T->BTree_insert('A'-'A');
    T->BTree_insert('B'-'A');
    T->BTree_insert('C'-'A');
    T->BTree_insert('E'-'A');
    T->BTree_insert('J'-'A');
    T->BTree_insert('L'-'A');
    //现在C在根里

    T->BTree_insert('N'-'A');
    T->BTree_insert('O'-'A');
    //现在L在根里

    T->BTree_insert('K'-'A');

    T->BTree_insert('P'-'A');
    T->BTree_insert('Q'-'A');
    T->BTree_insert('R'-'A');
    T->BTree_insert('T'-'A');
    //现在P在根里

    T->BTree_insert('U'-'A');
    T->BTree_insert('V'-'A');
    T->BTree_insert('S'-'A');
    //现在T在根里

    T->BTree_insert('X'-'A');

    T->BTree_insert('Y'-'A');
    T->BTree_insert('Z'-'A');

    T->display();
    T->BTreeDeleteVal(110);
    T->display();

    //T->BTreeDeleteVal(25);
    //T->display();
    /*
     * 经检验,删除情况中
     * case 1 无问题
     * case 2a 已解决
        case 2b 已解决
        case 2c 无问题
        case 3a:
            y为靠左，z为靠右孩子情形已解决
        case 3b:
            好像?解决了?
     */
    return 0;
}

tuple<BTreeNode*,int> BTree::BTree_Search(BTreeNode* x,int key){
    int i=0;
    //先在当前节点含的值查询
    while(i<x->vals.size()&&key>x->vals[i]){
        ++i;
    }
    if(i<x->vals.size()&&key==x->vals[i]){
        return make_tuple(x,i);
    }
    else if(x->isLeaf){
        cout<<key<<" is not in B-Tree."<<endl;
        return tuple{nullptr,-1};
    }
    else{
        return BTree_Search(x->children[i],key);
    }

}
void BTree::BTree_SplitChild(BTreeNode* x,int idx){
    //撕裂节点x的第idx+1个孩子
    auto y=x->children[idx];
    auto z=new BTreeNode(t,t-1);//不要在这里设置孩子个数，如果是叶子就没有孩子！
    z->isLeaf=y->isLeaf;

    for(auto i=0;i<z->vals.size();++i){
        z->vals[i]=y->vals[i+t];//从y的第t+1个开始取
        //cout<<"z"<<z->vals[i]<<endl;
    }
    if(!y->isLeaf){
        //如果y不是叶子，那么y的那些孩子就成为分裂节点z的孩子,因为他已经是个满值节点，那么至少有2*t-2个孩子
        for( int j=0;j<=t-1;++j){
            z->children[j]=y->children[j+t];
            y->children[j+t]= nullptr;
        }
    }
    x->vals.insert(idx+x->vals.begin(),y->vals[t-1]);//索引t-1是第t个
    y->vals.erase(y->vals.begin()+t-1,y->vals.end());
    //y is Leaf
    //cout<<y->vals[idx+1];
    //for(int i=0;i<y->vals.size();++i)cout<<"y->val "<<y->vals[i]<<endl;
    //cout<<"split y children count="<<y->children.size()<<endl;
    for(int i=2*t-1;i-1>=idx+1;--i){
        x->children[i]=x->children[i-1];//注意加1，z作为其靠右的孩子
    }
    x->children[idx+1]=z;
    //x->children.insert(idx+1+x->children.begin(),z);//注意加1，z作为其靠右的孩子

}
void BTree::BTree_insert_Notfull(BTreeNode* x,int key){
    //锁定插入区间
    //这意味着只有把叶子节点以上的节点必是满节点
    if(x== nullptr){
        x= new BTreeNode(t);
        x->isLeaf=true;
    }
    if(x->isLeaf){
        auto it=upper_bound(x->vals.begin(),x->vals.end(),key);
        //找到这个元素最晚插入位置
        x->vals.insert(it,key);
    }else{
        //x是内部节点,必须插在叶子节点
        auto it=lower_bound(x->vals.begin(),x->vals.end(),key);
        //插入到索引i的孩子
        auto idx=it-x->vals.begin();

        if(x->children[idx]->vals.size()==2*t-1){
            //如果其孩子满了，分裂它
            cout<<"splitChild in insert not full."<<endl;
            BTree_SplitChild(x,idx);
            if(key>x->vals[idx]) ++idx;//则进入分裂后靠右的那个孩子
        }
        //cout<<"idx"<<idx<<endl;
        BTree_insert_Notfull(x->children[idx],key);
    }
}
void BTree::BTree_insert(int key){
    if(root->vals.size()==2*t-1){
        auto s=new BTreeNode(t);
        s->isLeaf= false;
        s->children[0]=root;
        root=s;
        cout<<"Split root"<<endl;
        BTree_SplitChild(s,0);//撕裂第一个孩子
        BTree_insert_Notfull(s,key);
    }
    else{
        BTree_insert_Notfull(root,key);
    }
    cout<<"The "<<setw(3)<<key<<" has inserted!"<<endl;
}
void BTree::display(BTreeNode* x){
    if(!x) return;
    if(x->isLeaf){
        for(auto num:x->vals)cout<<num<<endl;
        return;
    }
    int n=x->vals.size();
    int i=0;
    for(;i<x->vals.size();++i){
        display(x->children[i]);
        cout<<x->vals[i]<<endl;
    }
    display(x->children[n]);
}
//前驱找的是，最后出现的那个比key小的
tuple<BTreeNode*,int> BTree::Forerunner(BTreeNode* x,int key){
    if(x->isLeaf) return make_tuple(x,x->vals.size()-1);//找到了叶子，
    //它比当前节点最后一个key还大，那就在最后一个孩子里找，一直往右找
    return Forerunner(x->children.back(),key);
}//寻找key的前驱
tuple<BTreeNode*,int> BTree::Succeed(BTreeNode* x,int key){
    if(x->isLeaf) return make_tuple(x,0);//一定是靠左的
    return Succeed(x->children.front(),key);
}//寻找key的后继
tuple<BTreeNode*,bool> BTree::BTree_neighborNode(const BTreeNode*  x,const int k){
    if(k==0){
        return make_tuple(x->children[1],isRight);
    }else if(k==x->vals.size()){
        //y是x的最右边的一个孩子
        return make_tuple(x->children[k-1],isLeft);
    }
    //if(k>0&&k<x->vals.size()){
    else if(x->children[k-1]->vals.size()>=t){
        return make_tuple(x->children[k-1],isLeft);
    }else{
        return make_tuple(x->children[k+1],isRight);
    }
}
bool BTree::BTreeDelete(BTreeNode* x,int key) {
    auto it=lower_bound(x->vals.begin(),x->vals.end(),key);
    int k=it-x->vals.begin();
    if(x->isLeaf&&*it!=key){
        cout<<key<<" is not in B-Tree."<<endl;
        return false;
    }
    //如果能在x中命中key，那么y一定不会超过x.n，因为x右侧的孩子key总大于x上的val,这样z作为y的右兄弟就有意义
    if(*it==key){
        //case 1
        if(x->isLeaf){
            cout<<"case 1:"<<endl;
            x->vals.erase(it);
            //cout<<"x->children.size()="<<x->children.size()<<endl;
            return true;
        }
        //case2 x is not leaf
        //case 2a
        auto y=x->children[k];
        if(y->vals.size()>=t){
            cout<<"case 2a:";
            auto [Node,pos]=Forerunner(y,key);//因为其子节点可能也是内部节点，必须得寻找,寻找到的必是叶子节点，那么可以放心删除k'

            auto pre=Node->vals[pos];
            //cout<<"pre"<<pre;
            x->vals[k]=pre;
            return BTreeDelete(y,pre);
        }
        else{
            auto z=x->children[k+1];
            //y.n<t
            //case 2b
            if(z->vals.size()>=t){
                cout<<"case 2b:";
                auto [Node,pos]=Succeed(z,key);//它一定是叶子里最左的key
                auto suc=*(Node->vals.begin());

                x->vals[k]=suc;
                return BTreeDelete(z,suc);

            }
            //case 2c
            //y.n==t-1&&z.n==t-1
            //需要合并
            cout<<"case 2c:";
            y->vals.push_back(x->vals[k]);
            //cout<<"z->children.size()="<<z->children.size()<<endl;
            for(int i=0;i<z->vals.size();++i){
                //cout<<"z->val["<<i<<"]="<<z->vals[i]<<endl;
                y->vals.push_back(z->vals[i]);
            }
            for(int i=0;i<t-1;++i){
                y->children[t+i]=z->children[i];
            }
            //cout<<"y->children.size()="<<y->children.size()<<endl;
            //必须把x中的key纳入y，因为z并不一定是个简单节点

            x->vals.erase(k+x->vals.begin());

            if(x->vals.empty()){
                auto [Xparent,i]=FindParent(root,x);
                delete x;
                if(i==-1) root=y;
                else
                    Xparent->children[i]=y;
            }
            //删除靠右的孩子
            delete z;
            x->children[k+1]= nullptr;

            return BTreeDelete(y,key);

        }
    }else{
        //case 3
        //不在节点x中，那么必在x->children[k]中

        auto y=x->children[k];//定义y总为x的第ci个孩子
        auto [z,Sideofy]= BTree_neighborNode(x,k);
        //y的邻居总是存在
        //z为y的一个邻居，首先选择有t个关键字的z作为邻居,按z在y的左右进行分类
        if(y->vals.size()==t-1&&z->vals.size()>=t){
            //case 3a
            cout<<"case 3a:";
            //因为x中不能命中key了，那么y可能靠左也可能靠右

            if(Sideofy==isRight){
                //降落x的一个关键字到y
                //此处假设y靠左，z靠右
                y->vals.insert(t-1+y->vals.begin(),x->vals[k]);

                //能够顶替x->val[k]的只有前驱和后继（但这里并不是完全意义的前驱）
                x->vals[k]=z->vals[0];
                z->vals.erase(z->vals.begin());

                if(!z->isLeaf){
                    y->children[t]=z->children[0];//插在末尾
                    for(int i=0;i+1<=2*t-1;++i){
                        z->children[i]= z->children[i+1];
                    }
                }
            }else{
                //z isLeft of y,那么k--是安全的
                y->vals.insert(y->vals.begin(),x->vals[k-1]);

                //能够顶替x->val[k]的只有前驱和后继（但这里并不是完全意义的前驱）
                x->vals[k-1]=z->vals.back();
                z->vals.pop_back();
                //z至少有t个关键字
                if(!z->isLeaf){
                    for(int i=2*t-1;i>=1;--i){
                        y->children[i]=y->children[i-1];
                    }
                    y->children[0]=z->children[t];
                    z->children[t]= nullptr;
                }
            }
            return BTreeDelete(y,key);

        }else{
            cout<<"case 3b:";
            //case 3b，合并其节点，保证降落到一个至少含有t个关键字的节点
            //其相邻兄弟z只有t-1个关键字
           //那么需要y和z合并
           //y不为最后一个，如果为最后一个，交换y和z

           if(k==x->vals.size()){
               z=y;
               y=x->children[k-1];
               --k;
               //y=x->children[k-1],z=x->children[k]，总要删除靠右的z
           }

           //保证y总是靠左的

            y->vals.push_back(x->vals[k]);
            for(auto zvals:z->vals){
                y->vals.push_back(zvals);
            }
            for(auto i=0;i<=t-1;++i){
                y->children[t+i]=z->children[i];
                z->children[i]= nullptr;
            }
            delete z;
            x->vals.erase(k+x->vals.begin());
            //需要递归的左移孩子，因为x的一个关键字降落了
            for(int i=k+1;i+1<=2*t-1;++i){
                x->children[i]=x->children[i+1];
            }
            //必须把x中的key纳入y，因为z并不一定是个简单节点
            if(x->vals.empty()){
                auto [Xparent,i]=FindParent(root,x);
                delete x;
                if(i==-1) root=y;
                else
                    Xparent->children[i]=y;
            }
            return BTreeDelete(y,key);
        }
    }
}
