# allocator-cpp
This is a C++ customized allocator based on a simple memory pool.

It was developed as the OS course project by Yu Shu and tested in VS 2015 at Windows 10. Any Unix-like OS should also work well, but you might need to write your own makefile (pretty easy).

## Brief introduction
This project was focused on implementing and testing a customized C++ allocator , which was intended to achieve better performance than the C++ Standard Library allocator in several specific (or overall) cases.

Memory pools, also called fixed-size blocks allocation, is the use of pools for memory management that allows dynamic memory allocation comparable to malloc or C++'s operator new.
As those implementations (malloc and new) suffer from fragmentation because of variable block sizes, it is not recommendable to use them in a real-time system due to performance.
A more efficient solution is preallocating a number of memory blocks with the same size called the memory pool.
The application can allocate, access, and free blocks represented by handles at run time. ([Memory pool](https://en.wikipedia.org/wiki/Memory_pool))

The following figure was drawn using [Inkscape](https://inkscape.org). It shows the basic structure of this project's implementation.

![img](/mempool.svg)

## Implementation
### Allocator
C++ allocator could be considered as a wrap of malloc/new. The standard implementation is always simple wrap.

The general way to use an allocator is like follows:

```C++
vector<int, some_allocator> v;
```

Here some_allocator could be std::allocator (default) or customized allocator defined by ourselves.
To let the customized allocator compatible with standard containers, we need to define the allocator similar to the standard template.
Here are some most important parts an allocator needed.

A couple of traditional type definitions, such as

```C++
typedef size_t    size_type;
typedef ptrdiff_t difference_type;
typedef T*        pointer;
typedef const T*  const_pointer;
```

Those names ensure that the allocators' client (for instance, 'std::vector') is able to use some relevant types by known names.

Also these names provide general ways to access specific types, such as pointer.
A peculiar looking template member structure:

```C++
template <class U>
struct rebind {
    typedef allocator<U> other;
};
```

This ensures the allocator to be able to allocate objects of different types than its template parameter.
For example, without this part, if we pass ```myallocator<int>``` to ```vector<int>```, we will get a lot of link errors. 
Because inside vector, it not only need an allocator for int, but also other types.
So the rebind mechanism is quite important for this.
The allocators' client has no idea what type the allocator actually has; thus, they need to ask the allocator itself to instantiate its own template with ```<U>``` and give it back to the client.

Allocation and deallocation member function. These two are the functions which actually do the allocation and deallocation work for allocator.

```C++
allocate(size_type n);
```

Returns storage for n elements of the element type being used in the memory model.
Elements will not be constructed/initialized.

```C++
deallocate(pointer p, size_type n)
```

Deallocates the storage for n elements of the element type being used in the memory model, beginning at location p.
Inside these two functions are our actually allocation mechanism.

We can easily put new into allocate and delete in deallocate.
Also, we can implement some more complex structure, such as memory pool.

### Memory pool
The basic idea of memory is to preallocate some a pool of memory, which is relatively large and used pretty offen. We don’t need to free this chunk of memory often, but let it there specially for our use. This reduces the ferquency call to melloc/free or new/delete, which imvolves system calls and cost more time. 

As shown before, to efficiently use this chunk of memory, we divide the pool to serveral blocks. Each block are linked as linked list. We maintain two linked list here, allocated one and free one (allocBlock and freeBlock in the figure). When container client acquire memory from this pool, we just pass the address of a free block back and remove the node from freeBlock, add it in allocBlock. When client call deallocate, we just do the other way. We can see here we don’t really do the allocation and deallocation from OS, so we can save time.

In this way, small chunk allocation can handle by memory pool. And if we need large memory size larger than block size, we can then directly pass them to malloc or new.

Balancing pool size, block size, and block numbers can help us achieve better performance.

### Tests
![img](/testresults.png)

For now, please refer to the code to see details of test.

### Discussion
At most experiments, our memory pool based allocator have better performance. But note that we also find that sometimes standard or self-implemented new based allocator perform better. 

Because our memory pool is a relatively simple implementation. There should be much improvement could be developing further. For example, we can let the memory pool grow by grow block numbers and link block inside to block outside current pool.

Also, our performance test is quite general. The block size and block number can be adjusted to fit specific usage. For example, we can set the block size multiple times as our element size, depending on container size.

## Reference

http://www.codeguru.com/cpp/cpp/cpp_mfc/stl/article.php/c4079/Allocators-STL.htm

https://www.codeproject.com/Articles/27487/Why-to-use-memory-pool-and-how-to-implement-it

http://www.josuttis.com/cppcode/allocator.html

http://docs.roguewave.com/sourcepro/11.1/html/toolsug/11-6.html

https://github.com/cacay/MemoryPool

http://www.drdobbs.com/cpp/improving-performance-with-custom-pool-a/184406243?pgno=1

http://www.codinglabs.net/tutorial_memory_pool.aspx

https://blogs.msdn.microsoft.com/calvin_hsia/2010/03/16/use-a-custom-allocator-for-your-stl-container/

http://www.boost.org/doc/libs/1_41_0/libs/pool/doc/interfaces/pool_alloc.html
