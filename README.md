# memory_pool
1.MemoryPool::allocateNewBlock() lastSlot_=reinterpret_cast<Slot*>(reinterpret_cast<char*>(newBlock)+BlockSize_-SlotSize_+1);为什么要+1
MemoryPool::allocate() if(curSlot_>=lastSlot_) 为什么判断是>=

如果不+1，那么allocate里的判断就须是>。这样一来首次调用curSlot和lastSlot都是nullptr，就无法调用allocateNewBlock()正常分配内存

