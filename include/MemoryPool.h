#include <cstddef>
#include <atomic>
#include <mutex>
#include <cassert>
#include <utility>

namespace ddy_MempryPool{
#define MEMORY_POOL_NUM 64
#define SLOT_BASE_SIZE 8
#define MAX_SLOT_SIZE 512

struct Slot{
    std::atomic<Slot*> next;
}

class MemoryPool{
    //初始化和公共的函数
    public:
        MemoryPool(size_t Block_size);
        ~MemoryPool();
        void init(size_t size);
        void* allocate();
        void deallocate(void* ptr);
    private:
        size_t BlockSize_;
        size_t SlotSize_;
        Slot* firstBlock_;
        Slot* curSlot_;
        Slot* lastSlot_;
        std::atomic<Slot*> freeList_;
        std::mutex mutexForBlock_;
    private:
        void allocateNewBlock();
        size_t padPointer(char* p,size_t align);
        Slot* popFreeList();
        void pushFreeList(Slot* slot);
}

}