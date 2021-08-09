#include <iostream>
#include "class/proto_vEB.h"

int main() {
    proto_vEB T(16);
    T.insert(0);
    cout<<T.minimum();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
