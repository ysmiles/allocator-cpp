#pragma once

#ifndef MEM_POOL
#define MEM_POOL

class mem_pool {
public:
    mem_pool();
    mem_pool(size_t n, size_t sz);
    void* allocate(size_t sz);
    void deallocate(void *ptr, size_t = 0);
    ~mem_pool();

private:
    struct block {
        struct block *prev = nullptr, *next = nullptr;
    };
    typedef struct block* block_ptr;
    void* mpool = nullptr; // address of the whole pool
    block_ptr freeBlock = nullptr; // free memory block linked list
    block_ptr allocBlock = nullptr; // alloced memory bleck linked list

    size_t block_size;
    size_t block_num;
    size_t pool_size = block_num * (block_size + sizeof(block));
};

#endif // !MEM_POOL
