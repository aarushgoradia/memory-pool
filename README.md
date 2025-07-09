# MemoryPoolAllocator

A lightweight fixed-size memory pool allocator in C++ with support for placement `new` and RAII-safe deallocation.

This project demonstrates low-level memory management using a preallocated buffer, free list tracking, and type-safe allocation.  It is suitable for scenarios where fast, 
repeated allocations of similarly sized objects are required.

## Features

- Fixed-size block allocation from a preallocated buffer
- Manual and RAII-safe deallocation via `PoolPointer<T>`
- Alignment-aware and type-safe
- Minimal dependencies

## Build instructions

This is a CMake based project. To build:
```bash
cmake -B build
cmake --build build
```
Or open the folder in an IDE like CLion or Visual Studio with CMake support

## Example Usage
```cpp
struct Enemy {
    int hp;
    float x, y;
    Enemy(int h, float xpos, float ypos);
    ~Enemy();
};

int main {
    size_t blockSize = sizeof(MemoryBlock) + sizeof(Enemy);
    MemoryPool pool(blockSize, 10);

    Enemy* e1 = pool.allocate<Enemy>(100, 1.0f, 2.0f);
    Enemy* e2 = pool.allocate<Enemy>(75, 3.0f, 4.0f);
    PoolPointer<Enemy> e3(pool.allocate<Enemy>(60, 5.0f, 6.0f), &pool);

    pool.deallocate(e1);
    pool.deallocate(e2);
}
```

## License
[MIT License](LICENSE.txt)