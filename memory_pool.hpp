//
// Created by Aarush Goradia on 9/7/25.
//

#include <cstddef>

#ifndef MEMORYPOOLALLOCATOR_MEMORY_POOL_HPP
#define MEMORYPOOLALLOCATOR_MEMORY_POOL_HPP

struct MemoryBlock {
    MemoryBlock* next;
};


class MemoryPool {
private:
    MemoryBlock* freeList;
    std::byte* buffer;
    size_t blockSize;
    size_t poolSize;
public:
    MemoryPool(size_t blockSize, size_t numBlocks);

    ~MemoryPool();

    template<typename T, typename... Args>
    T* allocate(Args&&... args) {
        if (freeList == nullptr) {
            return nullptr;
        }
        static_assert(sizeof(T) <= blockSize - sizeof(MemoryBlock), "Type T is too large for the memory block size");

        // Find the next free block
        MemoryBlock* block = freeList;
        freeList = block->next;

        // Make the payload
        void* payload = reinterpret_cast<void*>(block + 1);
        T* obj = new (payload) T(std::forward<Args>(args)...);
        
        return obj;
    }

    template<typename T>
    void deallocate(T* ptr) {
        if (ptr == nullptr) return;

        ptr->~T(); // Destroy the object

        // Get the block header thta comes before the payload
        MemoryBlock* block = reinterpret_cast<MemoryBlock*>(ptr) - 1;
        void* payload = reinterpret_cast<void*>(block + 1);
        

        // Push the block back onto the freelist
        block->next = freeList;
        freeList = block;
    }
};

template<typename T>
class PoolPointer {
private:
    T* ptr;
    MemoryPool* owner;
public:
    PoolPointer(T* p, MemoryPool* pool) : ptr(p), owner(pool) {}
    ~PoolPointer() { pool->deallocate(ptr); }
};

#endif //MEMORYPOOLALLOCATOR_MEMORY_POOL_HPP
