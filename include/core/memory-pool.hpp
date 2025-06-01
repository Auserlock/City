#ifndef CORE_MEMORY_POOL_HPP
#define CORE_MEMORY_POOL_HPP

#include <core/allocator.hpp>

#include <string>
#include <utility>
#include <vector>

namespace city
{
    class BasicMemoryPool
    {
    public:
        std::size_t used = 0; 

        explicit BasicMemoryPool(std::string  name, const size_t capacity, Allocator* aloc): name{std::move(name)}, aloc{aloc}
        {
            _sys_alloc(capacity);
            _init();
        }
        ~BasicMemoryPool()
        {
            aloc->free(mem);
        }
        void* allocate(std::size_t n);
        void free();
        void init() const
        {
            if (!used) return;
            aloc->init(mem, used);
        }
        [[nodiscard]] inline std::size_t get_cap() const { return capacity; }
    private:
        std::string name;
        std::size_t capacity{};
        Allocator* aloc;
        void* mem{};

        void _sys_alloc(std::size_t capacity);
        void _init() const
        {
            aloc->init(mem, capacity);
        }
    };

    // integrate multiple BasicMemoryPool. extending its function 
    class MemoryPool
    {
    public:
        MemoryPool() = delete;
        explicit MemoryPool(std::string  name, std::size_t init_cap, Allocator* aloc, std::size_t extend_unit = (1 << 24));
        ~MemoryPool();
        void* allocate(std::size_t n);
        void free();
        void init_memory() const;

        std::size_t get_used();
        void set_used(std::size_t n);
        std::size_t get_cap();
    private:
        std::string name;
        std::vector<BasicMemoryPool*> pools;
        std::size_t capacity;
        std::size_t extend_unit;
        Allocator* aloc;
        int cur;
    };
}

#endif