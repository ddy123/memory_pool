#include<../include/MemoryPool.h>
namespace ddy_MemoryPool{
    MemoryPool::MemoryPool(size_t BlockSize)
        :BlockSize_(BlockSize),
         SlotSize_(0),
         firstBlock_(nullptr),
         curSlot_(nullptr),
         lastSlot_(nullptr),
         freeList_(nullptr){
         }
    void MemoryPool::init(size_t size){
        assert(size>0);
        SlotSize_=size;
    }
     MemoryPool::~MemoryPool(){
        Slot* curr=firstBlock_;
        while(curr!=nullptr){
            Slot* next=curr->next.load(std::memory_order_relaxed);
            operator delete(reinterpret_cast<void*>(curr));
            curr=next;
        }
    }

    void* MemoryPool::allocate(){
        Slot* slot=popFreeList();
        if(slot!=nullptr){
            return slot;
        }

        Slot* tmp;

        {
        std::lock_guard<std::mutex> lock(mutexForBlock_);
        if(curSlot_>=lastSlot_){
                allocateNewBlock();
            }
        tmp=curSlot_;
        //转为char*进行指针运算
        //curSlot_=reinterpret_cast<Slot*>(reinterpret_cast<char*>(curSlot_)+SlotSize);
        //不能直接 curSlot_ += SlotSize_，因为 curSlot_ 是 Slot*类型
        //指针加法会按 sizeof(Slot) 为单位移动，直接相加就是SlotSize_*sizeof(Slot)
        //如果要移动 SlotSize_ 字节，所以需要除以sizeof(Slot)进行指针运算
        curSlot_+=SlotSize_/sizeof(Slot);

        }
            
        return tmp;
        
    }

    void MemoryPool::deallocate(void* ptr){
        if(ptr==nullptr){
            return;
        }
        pushFreeList(reinterpret_cast<Slot*>(ptr));
    }

    bool MemoryPool::pushFreeList(Slot* slot){
        while(true){
            //获取当前头节点
            Slot* oldHead=freeList_.load(std::memory_order_relaxed);
            //将新节点的next指向当前头节点
            slot->next.store(oldHead,std::memory_order_relaxed);
            //尝试将头节点更新为新节点
            if(freeList_.compare_exchange_weak(oldHead,slot,std::memory_order_release,std::memory_order_relaxed)){
                return true;
            }
        }
    }

  /*   //compare_exchange_weak失败时会将oldHead更新为当前freeList_的值，因此在下一次循环中会使用最新的头节点进行操作，保证了线程安全。
    bool MemoryPool::psuhFreeList(Slot* slot){
        Slot* oldHead=freeList_.load(std::memory_order_relaxed);
        do{
            slot->next.store(oldHead,std::memory_order_relaxed);
        }while(!freeList_.compare_exchange_weak(oldHead,slot,std::memory_order_release,std::memory_order_relaxed));
        return true;
    } */

    Slot* MemoryPool::popFreeList(){
        while(true){
            //获取当前头节点
            Slot* oldHead=freeList_.load(std::memory_order_relaxed);
            if(oldHead==nullptr){
                return nullptr;
            }
            //获取下一个节点
            Slot* next=oldHead->next.load(std::memory_order_relaxed);
            //尝试将头节点更新为下一个节点
            if(freeList_.compare_exchange_weak(oldHead,next,std::memory_order_acquire,std::memory_order_relaxed)){
                return oldHead;
            }
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
        
        lastSlot_=reinterpret_cast<Slot*>(reinterpret_cast<char*>(newBlock)+BlockSize_-SlotSize_+1);
    }


    MemoryPool& HashBucket::getMemoryPool(int index){
        static MemoryPool memoryPool[MEMORY_POOL_NUM];
        return memoryPool[index];
    }
    
    void HashBucket::initMemoryPool(){
        for(int i=0;i<MEMORY_POOL_NUM;i++){
            getMemoryPool(i).init((i+1)*SLOT_BASE_SIZE);
        }
    }

}