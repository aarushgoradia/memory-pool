#include <cstddef>
#include <stdexcept>
#include "memory_pool.hpp"

MemoryPool::MemoryPool(size_t blockSize, size_t numBlocks) {
	// Ensure blockSize is large enough to hold a MemoryBlock header
	if (blockSize < sizeof(MemoryBlock)) {
		throw std::invalid_argument("Block size must be at least the size of MemoryBlock.");
	}

	// Align blockSize to satisfy the strictest alignment requirement on this platform
	size_t alignment = alignof(std::max_align_t);
	if (blockSize % alignment != 0) {
		blockSize += alignment - (blockSize % alignment);
	}

	this->blockSize = blockSize;
	this->poolSize = blockSize * numBlocks;

	// Allocate raw memory buffer to hold all the blocks
	buffer = new std::byte[poolSize];

	// Slice the buffer into equal-sized blocks and link them into a free list
	for (size_t i = 0; i < numBlocks; ++i) {
		MemoryBlock* block = reinterpret_cast<MemoryBlock*>(buffer + i * blockSize);
		block->next = (i < numBlocks - 1) ? reinterpret_cast<MemoryBlock*>(buffer + (i + 1) * blockSize) : nullptr;
	}

	// Set the head of the free list to the beginning of the buffer
	this->freeList = reinterpret_cast<MemoryBlock*>(buffer);
}

MemoryPool::~MemoryPool() {
	delete[] buffer; // Release entire memory buffer
}
