#include <core/memory-pool.hpp>
#include <util/except.hpp>

#include <format>
#include <iostream>
#include <utility>

constexpr std::size_t max_alloc = static_cast<std::size_t>(64) << 30;

namespace city
{
    void BasicMemoryPool::_sys_alloc(const std::size_t capacity)
    {
        this->capacity = aloc->round_up_align(capacity);
        mem = aloc->malloc(capacity);
        if (!mem)
            RUNTIME_ERROR(fmt("{} failed to allocate {} memory.", name, capacity))
        used = 0;
    }

    void* BasicMemoryPool::allocate(const std::size_t n)
    {
        const auto aligned = aloc->round_up_align(n);
        if ((n + used) > capacity)
        {
            DEBUG_INFO(fmt("{} failed to allocate {} memory. capacity limit.", name, capacity))
            return nullptr;
        }
        void* res = static_cast<char*>(mem) + used;
        used += aligned;
        return res;
    }

    void BasicMemoryPool::free()
    {
        DEBUG_INFO(fmt("{} freeing {} memory.", name, used))
        used = 0;
    }

    MemoryPool::MemoryPool(std::string name, const std::size_t init_cap, Allocator *aloc, const std::size_t extend_unit) : name{std::move(name)}, capacity{init_cap}, aloc{aloc}, extend_unit{extend_unit}
    {
        ARG_CHECK(capacity < max_alloc, "attempt to allocate large memory exceeding 64GB in memory pool.")
        pools.push_back(new BasicMemoryPool(name, capacity, aloc));
        cur = 0;
    }

    MemoryPool::~MemoryPool()
    {
        for (const auto pool : pools) delete pool;
    }

    void* MemoryPool::allocate(const std::size_t n)
    {
        void* res = pools[cur]->allocate(n);
        if (!res)
        {
            const size_t extend_size = (n + extend_unit - 1) / extend_unit * extend_unit;
            pools.push_back(new BasicMemoryPool{name, extend_size, aloc});
            capacity += extend_size;
            cur++;
            res = pools[cur]->allocate(n);
        }
        if (!res)
            RUNTIME_ERROR(fmt("Failed to alloc memory"))
        return res;
    }

    void MemoryPool::free()
    {
        for (const auto pool : pools) { delete pool; }
        pools.clear();
        pools.push_back(new BasicMemoryPool{name, extend_unit, aloc});
        cur = 0;
    }

    void MemoryPool::init_memory() const
    {
        for (const auto pool : pools) { pool->init(); }
    }

    std::size_t MemoryPool::get_used() const
    {
        if (!cur) return pools[0]->used;
        size_t res = 0;
        for (const auto pool : pools) { res += pool->used; }
        return res;
    }

    void MemoryPool::set_used(std::size_t n)
    {
        
    }



}
