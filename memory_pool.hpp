#ifndef MEMORYPOOLALLOCATOR_MEMORY_POOL_HPP
#define MEMORYPOOLALLOCATOR_MEMORY_POOL_HPP

#include <cstddef>


// Metadata header for each memory block in the pool
struct MemoryBlock {
    MemoryBlock* next;
};

/* A fixed - size memory pool that allows for allocation and deallocation
of objects without using the system heap. Memory is pre-allocated up-front
and then managed via a freelist */ 
class MemoryPool {
private:
    MemoryBlock* freeList;
    std::byte* buffer;
    size_t blockSize;
    size_t poolSize;
public:
    /* Constructor: Allocates a pool with a fixed number of blocks.
    Each block must be large enough to store MemoryBlock + object payload. */
    MemoryPool(size_t blockSize, size_t numBlocks);

    // Destructor: releases raw memory buffer
    ~MemoryPool();

    /* Allocates and constructs an objet of type T with given arguments.
    Returns a pointer to the constructed object or nullptr if pool is full */
    template<typename T, typename... Args>
    T* allocate(Args&&... args) {
        if (freeList == nullptr) {
            return nullptr;
        }
        static_assert(sizeof(T) <= blockSize - sizeof(MemoryBlock), "Type T is too large for the memory block size");

        // Find the next free block
        MemoryBlock* block = freeList;
        freeList = block->next;

        // Construct the object in the payload area using placement new
        void* payload = reinterpret_cast<void*>(block + 1);
        T* obj = new (payload) T(std::forward<Args>(args)...);
        
        return obj;
    }

    /* Manually destroy the object and return to the memory pool */
    template<typename T>
    void deallocate(T* ptr) {
        if (ptr == nullptr) return;

        ptr->~T(); // Destroy the object

        // Recover the MemoryBlock header by backing up one block
        MemoryBlock* block = reinterpret_cast<MemoryBlock*>(ptr) - 1;
        

        // Push the block back onto the freelist
        block->next = freeList;
        freeList = block;
    }
};

/* A smart RAII wrapper for objects allocated from a MemoryPool
Automatically deacllocates the object when it goes out of scope. */
template<typename T>
class PoolPointer {
private:
    T* ptr;
    MemoryPool* owner;
public:
    PoolPointer(T* p, MemoryPool* pool) : ptr(p), owner(pool) {}
    ~PoolPointer() { owner->deallocate(ptr); }

    // Disable copying to prevent double free errors
    PoolPointer(const PoolPointer&) = delete;
    PoolPointer& operator=(const PoolPointer&) = delete;
};

#endif //MEMORYPOOLALLOCATOR_MEMORY_POOL_HPP
