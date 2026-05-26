#include "../include/MemoryPool.h"
#include <iostream>
using namespace ddy_MemoryPool;
//测试不同大小对象
class P1{
    int id_;
};
class P2{
    int id_[5];
};
class P3{
    int id_[10];
};
class P4{
    int id_[20];
};
int main(){
    HashBucket::initMemoryPool();

    std::cout << "sizeof(P1): " << sizeof(P1) << " bytes" << std::endl;
    std::cout << "sizeof(P2): " << sizeof(P2) << " bytes" << std::endl;
    std::cout << "sizeof(P3): " << sizeof(P3) << " bytes" << std::endl;
    std::cout << "sizeof(P4): " << sizeof(P4) << " bytes" << std::endl;

    P1* p1 = newElement<P1>();
    P2* p2 = newElement<P2>();
    P3* p3 = newElement<P3>();
    P4* p4 = newElement<P4>();

    std::cout << "第一次申请地址：" << std::endl;
    std::cout << "p1 = " << p1 << std::endl;
    std::cout << "p2 = " << p2 << std::endl;
    std::cout << "p3 = " << p3 << std::endl;
    std::cout << "p4 = " << p4 << std::endl;

    deleteElement(p1);
    deleteElement(p2);
    deleteElement(p3);
    deleteElement(p4);

    P1* p1_again = newElement<P1>();
    P2* p2_again = newElement<P2>();
    P3* p3_again = newElement<P3>();
    P4* p4_again = newElement<P4>();

    std::cout << "释放后再次申请地址：" << std::endl;
    std::cout << "p1_again = " << p1_again << std::endl;
    std::cout << "p2_again = " << p2_again << std::endl;
    std::cout << "p3_again = " << p3_again << std::endl;
    std::cout << "p4_again = " << p4_again << std::endl;

    deleteElement(p1_again);
    deleteElement(p2_again);
    deleteElement(p3_again);
    deleteElement(p4_again);

    return 0;
}