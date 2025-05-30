#include <core/allocator.hpp>
#include <util/except.hpp>

#include <sys/shm.h>
#include <sys/mman.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <format>

namespace city
{
    Allocator::~Allocator() = default;

    void* CPUAllocator::malloc(std::size_t n)
    {
        void* ptr = std::aligned_alloc(align, round_up_align(n));
        if (!ptr)
        {
            std::cerr << fmt("CPU memory allocation failed. n = {}, align = {}\n", n, align);
            throw city::out_of_memory("CPU memory allocation failed");
        }
        return ptr;
    }

    void CPUAllocator::free(void* mem)
    { 
        std::free(mem);
    }

    void CPUAllocator::init(void* mem, const std::size_t n)
    {
        memset(mem, 0, n);
    }

    void* SharedAllocator::malloc(const std::size_t n)
    {
        void* ptr = mmap(nullptr, round_up_align(n), PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);
        if (ptr == MAP_FAILED) {
            std::cerr << "Shared memory allocation failed n=" << n << std::endl;
            throw city::out_of_memory("Shared memory allocation failed");
        }
        size += round_up_align(n);
        return ptr;
    }

    void SharedAllocator::free(void* mem)
    {
        munmap(mem, size);
    }

    void SharedAllocator::init(void* mem, const std::size_t n)
    {
        memset(mem, 0, n);
    }
}