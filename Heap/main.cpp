#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

template<class Type>
class Less{
public:
    bool operator()(const Type& a,const Type & b){
        return a<b;
    };
};
template<typename Type>
class Greater{
public:
    bool operator()(const Type& a,const Type & b){
        return a>b;
    };
};
template<class T,class Compare=Less<T>>
class Heap{
    //默认构建大根堆
private:
    vector<T> less;
    vector<T> seq;
    Compare cmp;
public:
    void push(T val){
       seq.push_back(val);
       FixUp(seq.size()-1);
    };
    template<typename ... Args>
    void push(T val,Args... args){
        push(val);
        push(args...);
    };
    const T& top(){
        return seq[0];
    }
    void pop(){
        seq[0]=seq.back();
        seq.pop_back();
        FixDown(0);
    }
    void FixDown(int idx){
        //此函数表示第idx之后的所有元素构成一个堆，那么向下寻找当前堆顶插入位置
        auto val=seq[idx];
        int child=(idx<<1)+1;
        //沿着其父亲调节
        while(child<seq.size()){
            if(child+1<seq.size()&&cmp(seq[child],seq[child+1])) ++child;
            //if(child+1<seq.size()&&seq[child]<seq[child+1]) ++child;//指向右孩子（指向孩子最大的那个）
            if(!cmp(val,seq[child])) break;
            //if(seq[child]<=val) break;//val>=seq[child]

            //parent 's val < curr val
            seq[idx]=seq[child];//把孩子中最大的那个，且比val大的那个提升上来
            idx=child;
            child=(idx<<1)+1;
        }
        seq[idx]=val;
    };
    void FixUp(int idx){
        auto val=seq[idx];
        int parent=(idx-1)>>1;
        //沿着其父亲调节
        while(idx!=0){
            if(!cmp(seq[parent],val)) break;//seq[parent]>=val
            //parent 's val < curr val
            seq[idx]=seq[parent];
            idx=parent;
            parent=(idx-1)>>1;
        }
        seq[idx]=val;
    };
    bool isEmpty(){
        return seq.empty();
    }
    void Replace(T data){
        //当返回一个堆顶元素时，不想pop后再push
        seq[0]=data;
        FixDown(0);
    };
    void display(){
        auto Q=*this;
        while(!Q.isEmpty()){
            cout<<setw(3)<<Q.top();
            Q.pop();
        }
        cout<<endl;
    }
};
int main() {
    Heap<int,Greater<int>> Q;
    Q.push(1,88,3,6,2);
    Q.display();
    cout<<Q.top();
    return 0;
}
