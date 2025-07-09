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

    template<typename T>
    T* allocate();

    template<typename T>
    void deallocate(T* block);
};

template<typename T>
class PoolPointer {
private:
    T* ptr;
    MemoryPool* pool;
public:
    PoolPointer(T* p, MemoryPool* pool) : ptr(p), pool(pool) {}
    ~PoolPointer() { pool->deallocate(ptr); }
};

#endif //MEMORYPOOLALLOCATOR_MEMORY_POOL_HPP
