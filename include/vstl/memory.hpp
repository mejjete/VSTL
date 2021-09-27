#ifndef VSTL_MEMORY
    #define VSTL_MEMORY


#include <vstl/vdetail/type_traits/primary_traits.hpp>
#include <vstl/vdetail/allocator.hpp>
#include <vstl/vdetail/memory/vstl_ptr_traits.hpp>
#include <vstl/vdetail/type_traits/libc_traits.hpp>
#include <vstl/vdetail/memory/vstl_alloc_traits.hpp>


namespace vstl
{
    template <typename T>
    typename vstl::enable_if<vstl::is_object<T>::value, T*>::type
    addressof(T& arg) 
    { 
        return reinterpret_cast<T*>(
        &const_cast<char&>(reinterpret_cast<char&>(arg)));
    }   
};

#endif