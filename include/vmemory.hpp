#ifndef VSTL_MEMORY
    #define VSTL_MEMORY

#include <vdetail/type_traits/primary_traits.hpp>
#include <vdetail/vallocator.hpp>
#include <vdetail/memory/vstl_ptr_traits.hpp>

namespace vstl
{
    template <typename T>
    typename vstl::enable_if<vstl::is_object<T>::value, T*>::type
    addressof(T& arg) { return reinterpret_cast<T*>(
        &const_cast<char&>(reinterpret_cast<char&>(arg)));
    }   

    template <typename T>
    struct allocator_traits
    {
        using allocator_type =      typename T::allocator_type;
        using value_type =          typename T::value_type;
        using pointer =             typename T::pointer;
        using const_pointer =       typename T::const_pointer;
        using void_pointer =        typename T::void_pointer;
        using const_void_pointer =  typename T::const_void_pointer;
        using difference_type =     typename T::difference_type;
        using size_type =           typename T::size_type;
    };

};

#endif