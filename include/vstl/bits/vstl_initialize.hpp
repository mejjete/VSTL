#ifndef VSTL_INITIALIZE_H
#define VSTL_INITIALIZE_H


#include <vstl/bits/vstl_construct.hpp>
#include <vstl/iterator.hpp>


namespace vstl
{

    /* 
     *  Copies the value to the range __fiter, __fiter + __n
     *  Should return the last element in the range
    */
    template <typename _FwdIter, typename _Size, typename _Tp>
    inline _FwdIter __init_with_value(_FwdIter __fiter, _Size __n, _Tp&& __val)
    {
        _FwdIter __cur = __fiter;

        for(; __n > 0; --__n, ++__cur)
            vstl::_Construct(*(&__cur), vstl::forward<_Tp>(__val));
        return __cur;
    };
    


    /* 
     * Special version of __init_with_value with additional allocator parameter
    */
    template <typename _FwdIter, typename _Size, typename _Tp, typename _Alloc>
    _FwdIter __init_with_value_a(_FwdIter __fiter, _Size __n, _Tp&& __val, _Alloc& __alloc)
    {
        _FwdIter __cur = __fiter;

        try
        {
            for(; __n > 0; --__n, ++__cur)
                __alloc.construct(&(*__cur), vstl::forward<_Tp>(__val));
        }
        catch(...)
        {
            vstl::_Destroy_a(__fiter, __cur, __alloc);
            throw;
        }

        return __cur;
    };



    /* 
     *  In-place default construction with a given range
     *  If iterator_traits<_FwdIter>::value_type is not default constructible,
     *  so using __init_with_default_value would be ill-formed
    */
    template <typename _FwdIter, typename _Size, typename _Alloc>
    inline _FwdIter __init_with_default_value_a(_FwdIter __fiter, _Size __n, _Alloc& __alloc)
    {
        _FwdIter __cur = __fiter;

        try
        {
            for(; __n > 0; --__n, ++__cur)
                __alloc.construct(&(*__cur));            
        }
        catch(...)
        {
            vstl::_Destroy_a(__fiter, __cur, __alloc);
            throw;
        }

        return __cur;
    };


    /*
     *  In-place construction with a given range
    */
    template <typename _FirstIter, typename _SecIter, typename _Size, typename _Alloc>
    inline _FirstIter __init_with_range_a(_FirstIter __fiter, _SecIter __siter, _Size __n, _Alloc& __alloc)
    {
        _FirstIter __cur = __fiter;

        try 
        {
            for(; __n > 0; --__n, ++__cur, ++__siter)
                __alloc.construct(&(*__cur), *__siter);
        }
        catch(...)
        {
            vstl::_Destroy_a(__fiter, __cur, __alloc);
            throw;
        }

        return __cur;
    };


    /*
     *  Move range started with __siter to range started with __fiter
    */
    template <typename _FirstIter, typename _SecIter, typename _Size, typename _Alloc>
    inline _FirstIter __move_with_range_a(_FirstIter __fiter, _SecIter __siter, _Size __n, _Alloc& __alloc)
    {
        _FirstIter __cur = __fiter;

        try 
        {
            for(; __n > 0; --__n, ++__cur, ++__siter)
                __alloc.construct(&(*__cur), vstl::move(*__siter));
        }
        catch(...)
        {
            vstl::_Destroy_a(__fiter, __cur, __alloc);
            throw;
        }

        return __cur;
    };
}

#endif 