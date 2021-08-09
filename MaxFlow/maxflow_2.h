//
// Created by DELL on 2021/7/15.
//

#ifndef MAXFLOW_MAXFLOW_2_H
#define MAXFLOW_MAXFLOW_2_H
#include<iostream>
#include<cstring>
#include <string>
#include<algorithm>
#include<vector>
#include <queue>
#include <cassert>

#define rr register
using namespace std;

struct edge{
    int to,c,rev;//边里存储的分别是连接点、容量、反向边的编号
};
class EdmondsKarp{
private:
    static const int inf=0x3f3f3f3f;
    static const int maxn= 1e5 + 10;

    vector<edge> a[maxn];
    int visited[maxn];
    int s;
    int t;
    int m;//forward edge count
    int maxflow;
    string stratagy;
    vector<int> record_a_path;

    int Residual(int x,int y,int flow) {
        if( stratagy == "bfs"){
            return bfs(x,y,flow);
        } else return dfs(x,y,flow);
    }

    bool hasResidual(int delta_f) {
        return delta_f != 0;
    }
    int getadjEdge(int s){
        return a[s].size();
    }

    int dfs(int s,int t,int now);
    int bfs(int s,int t,int now);

    void showPath(int flow);

    void resetRecord(){
        while (!record_a_path.empty()){
            record_a_path.pop_back();
        }
    }

public:
    EdmondsKarp(int s,int t,string _stratagy="bfs"):stratagy(_stratagy),maxflow(0){
        set(s,t);
        transform(this->stratagy.begin(), this->stratagy.end(), this->stratagy.begin(),::tolower);
        assert(stratagy == "bfs" || stratagy == "dfs");
    };

    void set(int s,int t) {
        this->s = s;
        this -> t= t;
    }
    void addEdge(int x,int y,int z);

    template<typename... Arg>
    void addEdge(int x,int y,int z,Arg... arg) {

        addEdge(x,y,z);
        addEdge(arg...);
    }

    int solve() {
        return solve(s,t);
    }

    int solve(int x,int y){
        maxflow = 0;
        int delta_f=0;
        do {
            cout<<"--------"<<endl;
            maxflow += delta_f;
            //重置标记位

            memset(visited,0,sizeof(visited));
            delta_f = Residual(x,y,inf);
            //寻找增广路，因为在寻找过程中要取此时的流量与该路径的容量的min值，故流量初始化为极大值
            resetRecord();

        } while(hasResidual(delta_f));//当存在增广路的时候就继续找
        cout<<"maxflow:"<<maxflow<<endl;
        return maxflow;
    }
};



void EdmondsKarp::addEdge(int x,int y,int z){
    int No1=a[y].size();
    int No2=a[x].size();
    //加入反向边和正向边
    a[x].push_back((edge){y,z,No1});
    //对于一条边，它的反向边的编号就是它所连向的点此时所拥有的边的个数+1
    //但vector里数组下标都是0开始的，所以直接是a[x].size()就好了
    //反向边容量为0
    a[y].push_back((edge){x,0,No2});
    ++m;
}
int EdmondsKarp::bfs(int s,int t,int now) {
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        record_a_path.push_back(u);
        q.pop();

        int S = getadjEdge(u);

        for(int i = 0;i < S;++i) {
            int v = a[u][i].to;
            int f = a[u][i].c;
            int reverse_number = a[u][i].rev;

            if(!visited[v] && f > 0) {
                visited[v] = 1;
                q.push(v);
            }

            int tmp = min(f,now);

            if(tmp > 0) {
                a[u][i].c -= tmp;
                a[v][reverse_number].c += tmp;
                showPath(tmp);
                return tmp;
            }
        }
        record_a_path.pop_back();

    }
    return 0;
}

int EdmondsKarp::dfs(int s,int t,int now){
    record_a_path.push_back(s);

    if(s == t) {
        showPath(now);
        return now;
    }
    visited[s] = 1;
    //获取所有相连边
    int S  = getadjEdge(s);
    for(rr int i = 0;i < S;++i){
        int v = a[s][i].to;
        int f = a[s][i].c;
        int rev_edge_number=a[s][i].rev;
        if(!visited[v] && f > 0){

            int tmp = dfs(v,t,min(f,now));//从子节点开始找增广路
            record_a_path.pop_back();

            if(tmp > 0) {//如果存在增广路
                a[s][i].c -= tmp;//容量减去增加的流量就相当于流量加上这么多
                a[v][rev_edge_number].c += tmp;//正向边流量增加相当于反向边流量减少

                return tmp;
            }
        }
    }
    return 0;
}
void EdmondsKarp::showPath(int flow) {
    cout<<"find a path,length "<<record_a_path.size()<<": ";
    for (auto vertex=record_a_path.begin();vertex !=record_a_path.end();++vertex){
        cout << *vertex;
        if (next(vertex) !=record_a_path.end()){
            cout<<"->";
        }
    }
    cout<<","<<"delta_f: "<<flow<<endl;
}
#endif //MAXFLOW_MAXFLOW_2_H
