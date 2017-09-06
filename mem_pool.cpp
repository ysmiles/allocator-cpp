#include "stdafx.h"
#include "mem_pool.h"

#include <iostream>

mem_pool::mem_pool(size_t n, size_t sz) : block_num(n), block_size(sz) {
    mpool = operator new(pool_size);
    // succeess create the pool
    if (mpool != nullptr) {
        for (auto i = 0; i < block_num; ++i) {
            block_ptr curr = reinterpret_cast<block_ptr>(static_cast<char*>(mpool) + i * (block_size + sizeof(block)));

            // insert curr into free blocks linked list
            // insert before
            curr->prev = nullptr;
            curr->next = freeBlock;

            if (freeBlock != nullptr) {
                freeBlock->prev = curr;
            }

            freeBlock = curr;
        }
    }
}

mem_pool::mem_pool() : mem_pool(64, 1024) {
}

void* mem_pool::allocate(size_t sz) {
    // derectly use operator new
    if (sz > block_size || freeBlock == nullptr || mpool == nullptr)
        return operator new(sz);

    //std::cout << "use pool" << std::endl;
    block_ptr curr = freeBlock;
    // delete the node from free block linked list
    freeBlock = curr->next;
    if (freeBlock != nullptr) {
        freeBlock->prev = nullptr;
    }

    // add node to allocated block linked list
    curr->next = allocBlock;
    if (allocBlock != nullptr) {
        allocBlock->prev = curr;
    }
    allocBlock = curr;

    return static_cast<void *>(reinterpret_cast<char*>(curr) + sizeof(block));
}

void mem_pool::deallocate(void *ptr, size_t sz) {
    // pointer is inside memory pool
    if (mpool < ptr && ptr < (void *)((char *)mpool + pool_size)) {
        block_ptr curr = reinterpret_cast<block_ptr>(static_cast<char *>(ptr) - sizeof(block));

        // delete node to allocated block linked list
        allocBlock = curr->next;
        if (allocBlock != nullptr) {
            allocBlock->prev = nullptr;
        }

        // add the node from free block linked list
        curr->next = freeBlock;
        if (freeBlock != nullptr) {
            freeBlock->prev = curr;
        }
        freeBlock = curr;

        //std::cout << "use pool" << std::endl;
    }
    else
        operator delete(ptr);

}

mem_pool::~mem_pool() {
    if (mpool != nullptr)
        operator delete(mpool);
}
