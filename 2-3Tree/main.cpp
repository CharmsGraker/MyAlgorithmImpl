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

    tuple<BTreeNode*,int> Forerunner(BTreeNode* x,int key);//Ѱ��key��ǰ��
    tuple<BTreeNode*,int> Succeed(BTreeNode* x,int key);//Ѱ��key�ĺ��
    tuple<BTreeNode*,int> FindParent(BTreeNode* p,BTreeNode* x){
        if(x==root){
            return make_tuple(root,-1);
        } //���ڵ�û�и��ף���˵������Ҫɾ����ԭroot
        else{
            for(int i=0;i<2*t-1;++i){
                if(p->children[i]==x){
                    return make_tuple(p,i);
                }
            }
            for(int i=0;i<2*t;++i){
                if(p->vals[i]>x->vals.back())//�ҵ���һ��p->vals[i]>x->vals.back()��,��ôx������p�ĺ�����һ��
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
    //�Ȳ�����BTree�Ż����Ϸ��ѣ�����Ҷ�ӵĹؼ��ַǳ��ܼ���ÿ2*t-1���ؼ�����ɷ��ѲŻ��ڸ��ڵ��²���һ��ֵ������BTree�����޴�

    T->BTree_insert('A'-'A');
    T->BTree_insert('B'-'A');
    T->BTree_insert('C'-'A');
    T->BTree_insert('E'-'A');
    T->BTree_insert('J'-'A');
    T->BTree_insert('L'-'A');
    //����C�ڸ���

    T->BTree_insert('N'-'A');
    T->BTree_insert('O'-'A');
    //����L�ڸ���

    T->BTree_insert('K'-'A');

    T->BTree_insert('P'-'A');
    T->BTree_insert('Q'-'A');
    T->BTree_insert('R'-'A');
    T->BTree_insert('T'-'A');
    //����P�ڸ���

    T->BTree_insert('U'-'A');
    T->BTree_insert('V'-'A');
    T->BTree_insert('S'-'A');
    //����T�ڸ���

    T->BTree_insert('X'-'A');

    T->BTree_insert('Y'-'A');
    T->BTree_insert('Z'-'A');

    T->display();
    T->BTreeDeleteVal(110);
    T->display();

    //T->BTreeDeleteVal(25);
    //T->display();
    /*
     * ������,ɾ�������
     * case 1 ������
     * case 2a �ѽ��
        case 2b �ѽ��
        case 2c ������
        case 3a:
            yΪ����zΪ���Һ��������ѽ��
        case 3b:
            ����?�����?
     */
    return 0;
}

tuple<BTreeNode*,int> BTree::BTree_Search(BTreeNode* x,int key){
    int i=0;
    //���ڵ�ǰ�ڵ㺬��ֵ��ѯ
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
    //˺�ѽڵ�x�ĵ�idx+1������
    auto y=x->children[idx];
    auto z=new BTreeNode(t,t-1);//��Ҫ���������ú��Ӹ����������Ҷ�Ӿ�û�к��ӣ�
    z->isLeaf=y->isLeaf;

    for(auto i=0;i<z->vals.size();++i){
        z->vals[i]=y->vals[i+t];//��y�ĵ�t+1����ʼȡ
        //cout<<"z"<<z->vals[i]<<endl;
    }
    if(!y->isLeaf){
        //���y����Ҷ�ӣ���ôy����Щ���Ӿͳ�Ϊ���ѽڵ�z�ĺ���,��Ϊ���Ѿ��Ǹ���ֵ�ڵ㣬��ô������2*t-2������
        for( int j=0;j<=t-1;++j){
            z->children[j]=y->children[j+t];
            y->children[j+t]= nullptr;
        }
    }
    x->vals.insert(idx+x->vals.begin(),y->vals[t-1]);//����t-1�ǵ�t��
    y->vals.erase(y->vals.begin()+t-1,y->vals.end());
    //y is Leaf
    //cout<<y->vals[idx+1];
    //for(int i=0;i<y->vals.size();++i)cout<<"y->val "<<y->vals[i]<<endl;
    //cout<<"split y children count="<<y->children.size()<<endl;
    for(int i=2*t-1;i-1>=idx+1;--i){
        x->children[i]=x->children[i-1];//ע���1��z��Ϊ�俿�ҵĺ���
    }
    x->children[idx+1]=z;
    //x->children.insert(idx+1+x->children.begin(),z);//ע���1��z��Ϊ�俿�ҵĺ���

}
void BTree::BTree_insert_Notfull(BTreeNode* x,int key){
    //������������
    //����ζ��ֻ�а�Ҷ�ӽڵ����ϵĽڵ�������ڵ�
    if(x== nullptr){
        x= new BTreeNode(t);
        x->isLeaf=true;
    }
    if(x->isLeaf){
        auto it=upper_bound(x->vals.begin(),x->vals.end(),key);
        //�ҵ����Ԫ���������λ��
        x->vals.insert(it,key);
    }else{
        //x���ڲ��ڵ�,�������Ҷ�ӽڵ�
        auto it=lower_bound(x->vals.begin(),x->vals.end(),key);
        //���뵽����i�ĺ���
        auto idx=it-x->vals.begin();

        if(x->children[idx]->vals.size()==2*t-1){
            //����亢�����ˣ�������
            cout<<"splitChild in insert not full."<<endl;
            BTree_SplitChild(x,idx);
            if(key>x->vals[idx]) ++idx;//�������Ѻ��ҵ��Ǹ�����
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
        BTree_SplitChild(s,0);//˺�ѵ�һ������
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
//ǰ���ҵ��ǣ������ֵ��Ǹ���keyС��
tuple<BTreeNode*,int> BTree::Forerunner(BTreeNode* x,int key){
    if(x->isLeaf) return make_tuple(x,x->vals.size()-1);//�ҵ���Ҷ�ӣ�
    //���ȵ�ǰ�ڵ����һ��key�����Ǿ������һ���������ң�һֱ������
    return Forerunner(x->children.back(),key);
}//Ѱ��key��ǰ��
tuple<BTreeNode*,int> BTree::Succeed(BTreeNode* x,int key){
    if(x->isLeaf) return make_tuple(x,0);//һ���ǿ����
    return Succeed(x->children.front(),key);
}//Ѱ��key�ĺ��
tuple<BTreeNode*,bool> BTree::BTree_neighborNode(const BTreeNode*  x,const int k){
    if(k==0){
        return make_tuple(x->children[1],isRight);
    }else if(k==x->vals.size()){
        //y��x�����ұߵ�һ������
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
    //�������x������key����ôyһ�����ᳬ��x.n����Ϊx�Ҳ�ĺ���key�ܴ���x�ϵ�val,����z��Ϊy�����ֵܾ�������
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
            auto [Node,pos]=Forerunner(y,key);//��Ϊ���ӽڵ����Ҳ���ڲ��ڵ㣬�����Ѱ��,Ѱ�ҵ��ı���Ҷ�ӽڵ㣬��ô���Է���ɾ��k'

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
                auto [Node,pos]=Succeed(z,key);//��һ����Ҷ���������key
                auto suc=*(Node->vals.begin());

                x->vals[k]=suc;
                return BTreeDelete(z,suc);

            }
            //case 2c
            //y.n==t-1&&z.n==t-1
            //��Ҫ�ϲ�
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
            //�����x�е�key����y����Ϊz����һ���Ǹ��򵥽ڵ�

            x->vals.erase(k+x->vals.begin());

            if(x->vals.empty()){
                auto [Xparent,i]=FindParent(root,x);
                delete x;
                if(i==-1) root=y;
                else
                    Xparent->children[i]=y;
            }
            //ɾ�����ҵĺ���
            delete z;
            x->children[k+1]= nullptr;

            return BTreeDelete(y,key);

        }
    }else{
        //case 3
        //���ڽڵ�x�У���ô����x->children[k]��

        auto y=x->children[k];//����y��Ϊx�ĵ�ci������
        auto [z,Sideofy]= BTree_neighborNode(x,k);
        //y���ھ����Ǵ���
        //zΪy��һ���ھӣ�����ѡ����t���ؼ��ֵ�z��Ϊ�ھ�,��z��y�����ҽ��з���
        if(y->vals.size()==t-1&&z->vals.size()>=t){
            //case 3a
            cout<<"case 3a:";
            //��Ϊx�в�������key�ˣ���ôy���ܿ���Ҳ���ܿ���

            if(Sideofy==isRight){
                //����x��һ���ؼ��ֵ�y
                //�˴�����y����z����
                y->vals.insert(t-1+y->vals.begin(),x->vals[k]);

                //�ܹ�����x->val[k]��ֻ��ǰ���ͺ�̣������ﲢ������ȫ�����ǰ����
                x->vals[k]=z->vals[0];
                z->vals.erase(z->vals.begin());

                if(!z->isLeaf){
                    y->children[t]=z->children[0];//����ĩβ
                    for(int i=0;i+1<=2*t-1;++i){
                        z->children[i]= z->children[i+1];
                    }
                }
            }else{
                //z isLeft of y,��ôk--�ǰ�ȫ��
                y->vals.insert(y->vals.begin(),x->vals[k-1]);

                //�ܹ�����x->val[k]��ֻ��ǰ���ͺ�̣������ﲢ������ȫ�����ǰ����
                x->vals[k-1]=z->vals.back();
                z->vals.pop_back();
                //z������t���ؼ���
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
            //case 3b���ϲ���ڵ㣬��֤���䵽һ�����ٺ���t���ؼ��ֵĽڵ�
            //�������ֵ�zֻ��t-1���ؼ���
           //��ô��Ҫy��z�ϲ�
           //y��Ϊ���һ�������Ϊ���һ��������y��z

           if(k==x->vals.size()){
               z=y;
               y=x->children[k-1];
               --k;
               //y=x->children[k-1],z=x->children[k]����Ҫɾ�����ҵ�z
           }

           //��֤y���ǿ����

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
            //��Ҫ�ݹ�����ƺ��ӣ���Ϊx��һ���ؼ��ֽ�����
            for(int i=k+1;i+1<=2*t-1;++i){
                x->children[i]=x->children[i+1];
            }
            //�����x�е�key����y����Ϊz����һ���Ǹ��򵥽ڵ�
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
