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

}