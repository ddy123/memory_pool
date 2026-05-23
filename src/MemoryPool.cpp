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

    void MemoryPool::allocateNewBlock(){
        void* newBlock=operator new(BlockSize_);

        //将新申请的块挂在头部
        reinterpret_cast<Slot*>(newBlock)->next=firstBlock_;
        firstBlock_=reinterpret_cast<Slot*>(newBlock);

        char* body=reinterpret_cast<char*>(newBlock)+sizeof(Slot*);
        size_t bodyPadding=padPointer(body,SlotSize_);

        curSlot_=reinterpret_cast<Slot*>(body+bodyPadding);
        lastSlot_=reinterpret_cast<Slot*>(reinterpret_cast<char*>(newBlock)+BlockSize_-SlotSize_);
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