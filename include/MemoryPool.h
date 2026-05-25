#include <cstddef>
#include <atomic>
#include <mutex>
#include <cassert>
#include <utility>

namespace ddy_MemoryPool{
#define MEMORY_POOL_NUM 64
#define SLOT_BASE_SIZE 8
#define MAX_SLOT_SIZE 512

struct Slot{
    std::atomic<Slot*> next;
}

class MemoryPool{
    //初始化和公共的函数
    public:
        MemoryPool(size_t Blocksize=4096);
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
 
class HashBucket{
    public:
        static void initMemoryPool();
        static MemoryPool& getMemoryPool(int index);
        static void* useMemory(size_t size){
             if(size<=0){
            return nullptr;
                }
            if(size>MAX_SLOT_SIZE){
            return operator new(size);
             }
             return getMemoryPool(((size+SLOT_BASE_SIZE-1)/SLOT_BASE_SIZE)-1).allocate();
        }
        static void freeMemory(void* ptr,size_t size){
            if(ptr==nullptr||size<=0){
                return;
            }
            if(size>MAX_SLOT_SIZE){
                operator delete(ptr);
                return;
            }
            
            getMemoryPool(((size+SLOT_BASE_SIZE-1)/SLOT_BASE_SIZE)-1).deallocate(ptr);
            
        }
    }
template<typename T,typename... Args>
T* newElement(Args&&... args){
    T* p=reinterpret_cast<T*>(HashBucket::useMemory(sizeof(T)));
    if(p!=nullptr){
        new(p) T(std::forward<Args>(args)...);
    }
    return p;
}
}