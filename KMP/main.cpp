#include <iostream>
#include <vector>

using namespace std;
class Solution{
private:
    vector<int> next;
public:
    int operator()(string &s,string &p,int pos){
        next=vector<int>(p.size());
        get_next(p);
        return KMP(s,p,pos);
    }
    int KMP(const string & s,const string & p,int pos=0){
        int i=pos;
        int plen =p.size(),slen=s.size();
        int j=0;
        while(i<slen&&j<plen){
            if(j==-1||s[i]==p[j]){
                //等于-1，那么从模式串的头和主串的下一个字符开始比较
                //字符相等，都往右移
                ++i;
                ++j;
            }else{
                j=next[j];
            }
        }
        if(j>=plen){
            return i-plen;
        }else{
            return -1;
        }
    }
    void get_next(string p){
        int i=0;
        next[0]=-1;/*索引0失配，i和j都得右移*/
        int j=-1;
        int pLen=p.size();
        /*以下是序号，不是索引
        //（1）若p[k]==p[j]，那么next[j+1]由next[j]+1转移而来
        //（2）若p[k]!=p[j]，那么next[j+1]得找到上次与p[k]相等的字符，即next[k]
        //知next[j+1]=next[k]+1
        //若上述都不满足，边界条件:next[j+1]=1，从第一个字符重新开始匹配*/
        while(i<pLen-1){
            if(j==-1||p[i]==p[j]){
                ++i;
                ++j;
                next[i]=j;/*写成状态转移方程比较直观一些*/
            }else{
                /*失配，回退至next[next[j]+1]*/
                j=next[j];
            }
        }
    }
};
int main() {
    string s="acabaabaabcacaabc";
    string p="abaabcac";
    Solution KMP;
    cout<<"w88jjj"<<KMP(s,p,0);
    return 0;
}
