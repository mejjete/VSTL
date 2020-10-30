#ifndef VSTL_ALLOCATOR
    #define VSTL_ALLOCATOR
#include <cstddef>
#include <limits>
#include <vdetail/type_traits/primary_traits.hpp>
#include <vutility.hpp>

namespace vstl
{
    template <typename T>
    class __vstl_alloc
    {
        public:
            __vstl_alloc() {};
    
            using allocator_type = __vstl_alloc<T>;
            using value_type = T;
            using pointer = T*;
            using const_pointer = const T *;
            using void_pointer = void *;
            using const_void_pointer = const void *;
            using reference = T&;
            using const_reference = const T&;
            using size_type = std::size_t;
            using different_type = std::ptrdiff_t;
            using propagate_on_container_move_assignment = vstl::true_type;
            using is_always_equal = vstl::true_type;

            template <typename U>
            struct rebind
            {
                using other = __vstl_alloc<U>;   
            };

            pointer allocate(size_type numObjects)
            {
                return static_cast<pointer>(operator new(sizeof(T) * numObjects));
            };

            void deallocate(pointer p, size_type numObject)
            {
                operator delete(p);
            };

            template <typename U, typename... Args>
            void construct(U* p, Args&& ...args)
            {
                new(p) T(vstl::forward<Args>(args)...);
            };

            template <typename U>
            void destroy(U *p) { p->~U(); };
            
            size_type max_size() const
            {
                return std::numeric_limits<size_type>::max();
            };

    };

    template <typename T>
    using allocator = vstl::__vstl_alloc<T>;
}

#endif