#ifndef VSTL_MEMORY
    #define VSTL_MEMORY


#include <vstl/vdetail/type_traits/primary_traits.hpp>
#include <vstl/vdetail/allocator.hpp>
#include <vstl/vdetail/memory/vstl_ptr_traits.hpp>
#include <vstl/vdetail/type_traits/libc_traits.hpp>
#include <vstl/vdetail/memory/vstl_alloc_traits.hpp>


namespace vstl
{
    template <typename _Tp>
    inline _Tp* addressof(_Tp& __address)
    {
        return const_cast<_Tp *>(reinterpret_cast<const _Tp*>(&reinterpret_cast<const char &>(__address)));
    };
};

#endif