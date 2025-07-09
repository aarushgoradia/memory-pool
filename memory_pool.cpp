//
// Created by Aarush Goradia on 9/7/25.
//

#include <cstddef>
#include <stdexcept>
#include "memory_pool.hpp"

MemoryPool::MemoryPool(size_t blockSize, size_t numBlocks) {
	if (blockSize < sizeof(MemoryBlock)) {
		throw std::invalid_argument("Block size must be at least the size of MemoryBlock.");
	}

	// Makes sure alignment of blocks are fine, otherwise makes the alignment correct
	size_t alignment = alignof(std::max_align_t);
	if (blockSize % alignment != 0) {
		blockSize += alignment - (blockSize % alignment);
	}

	this->blockSize = blockSize;
	this->poolSize = blockSize * numBlocks;

	buffer = new std::byte[poolSize];

	// Slices up into equal blocks
	for (size_t i = 0; i < numBlocks; ++i) {
		MemoryBlock* block = reinterpret_cast<MemoryBlock*>(buffer + i * blockSize);
		block->next = (i < numBlocks - 1) ? reinterpret_cast<MemoryBlock*>(buffer + (i + 1) * blockSize) : nullptr;
	}

	this->freeList = reinterpret_cast<MemoryBlock*>(buffer);
}

MemoryPool::~MemoryPool() {
	delete[] buffer;
}
