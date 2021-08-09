#include <iostream>
#include "Class/FIB_Heap.h"

int main() {
    FIB_Heap H1;
    H1.insert(2,9,2,3,1,4,100,2,20);
    FIB_Heap H2;
    H2.insert(8,213,44);
    auto z=H2.insert(17);
    FIB_Heap H;
    H._union(H1,H2);
    while(!H.isempty()){
        std::cout <<H << std::endl;
    }
    return 0;
}
