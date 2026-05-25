#include "../include/MemoryPool.h"
#include<iostream>
using namespace ddy_MemoryPool;

//测试同一个槽是否被重复使用
int main() {
    HashBucket::initMemoryPool();

    int* p1 = newElement<int>(1);
    deleteElement(p1);

    int* p2 = newElement<int>(2);
    deleteElement(p2);

    std::cout << p1 << std::endl;
    std::cout << p2 << std::endl;

    return 0;
}