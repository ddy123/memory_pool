#include "../include/MemoryPool.h"
#include <iostream>

using namespace ddy_MemoryPool;

class Test {
public:
    Test(int x) : x_(x) {
        std::cout << "Test constructor: " << x_ << std::endl;
    }

    ~Test() {
        std::cout << "Test destructor: " << x_ << std::endl;
    }

private:
    int x_;
};

int main() {
    HashBucket::initMemoryPool();

    Test* p = newElement<Test>(10);
    deleteElement(p);
    return 0;
}