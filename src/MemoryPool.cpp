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

    MemoryPool& HashBucket::getMemoryPool(int index){
        static MemoryPool memoryPool[MEMORY_POOL_NUM];
        return memoryPool[indedx];
    }

}