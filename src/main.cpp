#include <core/allocator.hpp>
#include <malloc.h>

#include <cstring>
#include <iostream>
#include <cassert>

using namespace std;
using namespace city;

#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <cassert>
#include <unistd.h> // sleep

void writer(void* mem) {
    const char* msg = "Hello from writer thread!";
    std::strcpy(static_cast<char*>(mem), msg);
    std::cout << "[Writer] Wrote message.\n";
}

void reader(void* mem) {
    sleep(1); // 等 writer 先写入
    char* p = static_cast<char*>(mem);
    std::cout << "[Reader] Read message: " << p << std::endl;
    assert(std::string(p) == "Hello from writer thread!");
}

int main() {
    SharedAllocator allocator;
    std::size_t size = 1024;
    void* mem = allocator.malloc(size);

    // 启动两个线程，一个写一个读
    std::thread t1(writer, mem);
    std::thread t2(reader, mem);

    t1.join();
    t2.join();

    allocator.free(mem);
    std::cout << "[Main] Shared memory freed.\n";

    return 0;
}
