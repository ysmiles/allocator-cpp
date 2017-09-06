#pragma once

#ifndef Y_ALLOC
#define Y_ALLOC

#include <limits>

#include "mem_pool.h"

// operator new based allocator, mimic Standard library
template <typename T>
class y_alloc {
public:
    // naming tradition
    typedef T value_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T &const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    // for inner rebind michanics
    // used to transform allocator of T to tyep U
    template <typename U> struct rebind {
        typedef y_alloc<U>
            other;
    };

    // constructors and destructors
    y_alloc() = default;
    y_alloc(const y_alloc &) {}
    template <typename U> y_alloc(const y_alloc<U> &other) {}
    // explicitly note that assignment operator is not needed
    y_alloc &operator=(const y_alloc &) = delete;
    ~y_alloc() = default;

    // reuturn address of value_type
    static pointer address(reference r) {
        return &r;
    }
    static const_pointer address(const_reference cr) {
        return &cr;
    }

    // reutrn maximun number can be allocated
    static size_type max_size() {
        return std::numeric_limits<size_type>::max();
    }

    // operator used for compare
    bool operator==(const y_alloc &) const {
        return true;
    }
    bool operator!=(const y_alloc &) const {
        return false;
    }

    // implement by wrapping operator new and delete
    // allocation and deallocation
    static pointer allocate(size_type n) {
        // result will be copied to return value
        return static_cast<pointer>(operator new(sizeof(T) * n));
    }
    static pointer allocate(size_type n, pointer ptr) {
        return allocate(n);
    }
    static void deallocate(pointer ptr, size_type n) {
        operator delete(ptr);
    }

    // constrution valu_type and destruction
    static void construct(pointer ptr, const value_type &t) {
        new(ptr) value_type(t);
    }
    static void destroy(pointer ptr) {
        ptr->~value_type();
    }
};

// memory pool based allocator
template <typename T>
class y_mp_alloc {
public:
    // naming tradition
    typedef T value_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T &const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    // for inner rebind michanics
    // used to transform allocator of T to tyep U
    template <typename U> struct rebind {
        typedef y_mp_alloc<U>
            other;
    };

    // constructors and destructors
    y_mp_alloc() = default;
    y_mp_alloc(const y_mp_alloc &) {}
    template <typename U> y_mp_alloc(const y_mp_alloc<U> &other) {}
    // explicitly note that assignment operator is not needed
    y_mp_alloc &operator=(const y_mp_alloc &) = delete;
    ~y_mp_alloc() = default;

    // reuturn address of value_type
    static pointer address(reference r) {
        return &r;
    }
    static const_pointer address(const_reference cr) {
        return &cr;
    }

    // reutrn maximun number can be allocated
    static size_type max_size() {
        return std::numeric_limits<size_type>::max();
    }

    // operator used for compare
    bool operator==(const y_mp_alloc &) const {
        return true;
    }
    bool operator!=(const y_mp_alloc &) const {
        return false;
    }

    // implement by wrapping operator new and delete
    // allocation and deallocation
    static pointer allocate(size_type n) {
        // result will be copied to return value
        // use memory pool memp
        return static_cast<pointer>(memp.allocate(sizeof(T) * n));
    }
    static pointer allocate(size_type n, pointer ptr) {
        return allocate(n);
    }
    static void deallocate(pointer ptr, size_type n) {
        // use memory pool memp
        memp.deallocate(ptr, n);
    }

    // constrution valu_type and destruction
    // still use new, here we just need to call the constructor
    static void construct(pointer ptr, const value_type &t) {
        new(ptr) value_type(t);
    }
    static void destroy(pointer ptr) {
        ptr->~value_type();
    }

private:
    static mem_pool memp;
};

template <typename T> mem_pool y_mp_alloc<T>::memp;

#endif // !Y_ALLOC
