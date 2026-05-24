#include <iostream>
#include <iomanip>

// 一个普通对象，大小为 16 字节（假设 int 为 4 字节，64 位系统）
struct Object {
    int a; // 偏移 0
    int b; // 偏移 4
    int c; // 偏移 8
    int d; // 偏移 12
};

// 链表节点，同样大小为 16 字节（指针 8 字节 + int 4 字节 + 填充 4 字节）
struct Node {
    Node* next; // 偏移 0，占 8 字节
    int   data; // 偏移 8，占 4 字节
    // 偏移 12 处有 4 字节填充（对齐到 16 字节）
};

int main() {
    // 1. 创建一个普通对象并填入已知值（便于观察内存布局）
    Object obj;
    obj.a = 0xAAAAAAAA; // 二进制：1010... 可读性强
    obj.b = 0xBBBBBBBB;
    obj.c = 0xCCCCCCCC;
    obj.d = 0xDDDDDDDD;

    std::cout << "Object 内存地址: " << &obj << "\n\n";

    // 2. 将同一块内存强行解释为 Node*
    Node* node = reinterpret_cast<Node*>(&obj);

    // 3. 读取“next”指针（实际上读取的是 obj 的前 8 字节：a 和 b 的组合）
    std::cout << "node->next (垃圾地址) = " << node->next << std::endl;
    // 以小端十六进制输出更清晰
    std::cout << "  十六进制: 0x" << std::hex << (uintptr_t)node->next << std::dec << "\n\n";

    // 4. 读取“data”成员（实际上读取的是 obj 的 c 和 d 的部分字节）
    std::cout << "node->data (垃圾整数) = " << node->data << std::endl;
    std::cout << "  十六进制: 0x" << std::hex << (unsigned int)node->data << std::dec << "\n\n";

    // 5. 危险操作（注释掉，否则可能崩溃）：
    // if (node->next) {
    //     std::cout << node->next->data; // 解引用无效指针 → 段错误
    // }

    // 6. 说明
    std::cout << "注意：以上行为是未定义行为（UB）。\n"
              << "实际输出的 next 值由 obj.a 和 obj.b 拼凑而成，\n"
              << "data 值由 obj.c 和 obj.d 拼凑而成，没有任何意义。\n"
              << "严格别名规则禁止这种访问，编译器可能优化出更诡异的结果。\n";

    return 0;
}