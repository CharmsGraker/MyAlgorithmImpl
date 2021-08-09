#include <iostream>
#include "maxflow_2.h"

int main() {

    auto solution = new EdmondsKarp(4,3);
    //输入有三个x,y,z,分别表示:流量边起点，流量边终点，该边允许的流的最大容量(capacity)
    //为了输入方便，不至于像：
    //     solution->addEdge(4, 3, 20);
    //     solution->addEdge(4, 2, 30);
    //     solution->addEdge(2, 1, 30);
    //     solution->addEdge(2, 3, 20);
    //     solution->addEdge(1, 3, 30);
    //
    // 可以按以下方式输入
    solution->addEdge(4  ,3,20,4,2,30,2,1,30,2,3,20,1,3,30);

    auto maxflow = solution->solve();
    cout<<maxflow;
    return 0;
}
