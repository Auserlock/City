#ifndef CORE_ALLOCATOR_HPP
#define CORE_ALLOCATOR_HPP

#include <vector>

#define fmt std::format

namespace city
{
    struct Allocator
    {
        const int align;

        explicit Allocator(const int align) : align{align} {}
        Allocator(const Allocator&) = delete;
        Allocator& operator=(const Allocator&) = delete;
        virtual ~Allocator();
        virtual void* malloc(std::size_t n) = 0;
        virtual void free(void* mem) = 0;
        virtual void init(void* mem, std::size_t n) = 0;
        
        [[nodiscard]] inline std::size_t round_up_align(std::size_t n) const
        {
            if(align < 2) return n;
            return ((n + align -1) / align) * align;
        }
    };

    struct CPUAllocator final : public Allocator
    {
        CPUAllocator() : Allocator{32} {}
        void* malloc(std::size_t n) override;
        void free(void* mem) override;
        void init(void* mem, std::size_t n) override;
    };

    struct SharedAllocator final : public Allocator
    {
        std::size_t size;

        SharedAllocator() : Allocator{32}, size{0} {}
        void* malloc(std::size_t n) override;
        void free(void* mem) override;
        void init(void* mem, std::size_t n) override;
    };

    #if USE_GPU
        struct GPUAllocator final : public Allocator
        {
            const int dev_id;

            explicit GPUAllocator(const int dev_id) : Allocator{256}, dev_id{dev_id} {}
            void* malloc(std::size_t n) override;
            void free(void* mem) override;
            void init(void* mem, std::size_t n) override;
        }
    #endif
}

#endif  // CORE_ALLOCATOR_HPP