#include<../include/MemoryPool.h>
namespace ddy_MemoryPool{
    MemoryPool::MemoryPool(size_t BlockSize)
        :BlockSize_(BlockSize),
         SlotSize_(0),
         freeBlock_(nullptr),
         curSlot_(nullptr),
         lastSlot_(nullptr),
         freeList_(nullptr){
         }
    void MemoryPool::init(size_t size){
        assert(size>0);
        SlotSize_=size;
    }

    void HashBucket::initMemoryPool(){
        for(int i=0;i<MEMORY_POOL_NUM;i++){
            getMemoryPool(i).init((i+1)*SLOT_BASE_SIZE);
        }
    }

    //指针对齐所需偏移量
    size_t MemoryPool::padPointer(char* p,size_t align){
        size_t rem=reinterpret_cast<size_t>(p)%align;
        return rem==0?0:align-rem;
    }

    MemoryPool& HashBucket::getMemoryPool(int index){
        static MemoryPool memoryPool[MEMORY_POOL_NUM];
        return memoryPool[index];
    }
    size_t MemoryPool::padPointer(char* p,size_t align){
        uintptr_t result=(reinterpret_cast<uintptr_t>(p)+align-1)&~(align-1);
        return result;
    }

}