#include <iostream>
#include "RBTree.h"

using namespace std;
int main() {
    RBTree T;
    T.insert(5);
    T.insert(3,25);
    T.insert(2,85,34,9,11);
    T.insert(31);
    T.display();
    T.Delete(90);
    //T.display();
    return 0;
}
