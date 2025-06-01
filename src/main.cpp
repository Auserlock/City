#include <core/memory-pool.hpp>
#include <malloc.h>

#include <cstring>
#include <iostream>
#include <cassert>

using namespace std;
using namespace city;

int main()
{
    CPUAllocator allocator{};
    MemoryPool pool("fistpool", 25, &allocator);
    return 0;
}
