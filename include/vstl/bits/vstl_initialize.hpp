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
    _FirstIter __move_with_range_a(_FirstIter __fiter, _SecIter __siter, _Size __n, _Alloc& __alloc)
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

    
    /**
     * An aux structure for calling move or copy constructor depending 
     * on what is avalaible for given type _Tp. It forces compiler to use move
     * construction if move is supported for a given type, and copy construction otherwise
     */
    template <typename _Tp>
    struct __copy_or_move_caller 
    {
        inline static _Tp& __copy_caller(_Tp& __lhs)
        { return __lhs; };

        inline static _Tp __move_caller(_Tp& __rhs)
        { return vstl::move(__rhs); };

        template <typename _Up = _Tp, typename = vstl::enable_if_t<vstl::is_copy_constructible<_Up>::value &&
            !vstl::is_move_constructible<_Up>::value, int>>
        inline static auto __call_copy_or_move(_Up& __val) -> decltype(__copy_caller(__val))
        { return __copy_caller(__val); };

        template <typename _Up = _Tp, typename = vstl::enable_if_t<vstl::is_move_constructible<_Up>::value, int>>
        inline static auto __call_copy_or_move(_Up& __val) -> decltype(__move_caller(__val))
        { return __move_caller(__val); };
    };


    /**
     * Copy or move range started with __siter to range started with __fiter
     */
    template <typename _FirstIter, typename _SecIter, typename _Size, typename _Alloc>
    _FirstIter __copy_or_move_with_range_a(_FirstIter __fiter, _SecIter __siter, _Size __n, _Alloc& __alloc)
    {
        typedef typename _Alloc::value_type value_type;
        _FirstIter __cur = __fiter;

        try
        {
            for(; __n > 0; --__n, ++__cur, ++__siter)
                __alloc.construct(&(*__cur), __copy_or_move_caller<value_type>::__call_copy_or_move(*__siter));
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