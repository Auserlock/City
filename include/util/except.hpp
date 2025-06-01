#ifndef UTIL_EXCEPT_HPP
#define UTIL_EXCEPT_HPP

#include <stdexcept>
#include <sstream>

namespace city
{
    class out_of_memory final : public std::runtime_error
    {
    public:
        explicit out_of_memory(const std::string& what_arg) : std::runtime_error(what_arg) {}
    };

    class gpu_exception final: public std::runtime_error
    {
    public:
        explicit gpu_exception(const std::string& what_arg) : std::runtime_error(what_arg) {}
    };

    class gpu_not_implemented final: public std::logic_error
    {
    public:
        explicit gpu_not_implemented(const std::string& what_arg) : std::logic_error(what_arg) {}
    };
}

// skip debug information
#ifdef SKIP_DEBUG_INFO
    #define DEBUG_INFO(msg)
#else
    #define DEBUG_INFO(msg) do {                        \
        std::cerr << msg << std::endl;                       \
    } while (0);
#endif

// skip checking arguments
#ifdef SKIP_ARG_CHECK
    #define INVALID_ARG(msg)
    #define ARG_CHECK(cond, msg)
#else
    #define INVALID_ARG(msg) do {                       \
        std::ostringstream oss;                         \
        oss << msg;                                     \
        throw std::invalid_argument(oss.str());         \
    } while (0);

    #define ARG_CHECK(cond, msg) do {                   \
        if (!(cond)) {                                    \
            std::ostringstream oss;                     \
            oss << msg;                                 \
            throw std::invalid_argument(oss.str());     \
        }                                               \
    } while (0);
#endif

// skip debug assert (cause program terminate)
#ifdef SKIP_ASSERT
    #define ASSERT_ARG(expr, msg)
#else
    #define ASSERT_ARG(expr, msg) do {              \
        if (!(expr)) {                                \
            std::ostringstream oss;                 \
            oss << msg;                             \
            throw std::runtime_error(oss.str());    \
        }                                           \
    } while (0);
#endif

#define RUNTIME_ERROR(msg) do {                     \
    std::ostringstream oss;                         \
    oss << msg;                                     \
    throw std::runtime_error(oss.str());            \
} while (0);

#define CUDA_NOT_IMPLEMENT_ERROR(op) do {           \
    std::ostringstream oss;                         \
    oss << op                                       \
        << " not implemented for CUDA yet. Use CPU implementation with to_device instead.";  \
    throw std::runtime_error(oss.str());            \
} while (0);

#endif // UTIL_EXCEPT_HPP
