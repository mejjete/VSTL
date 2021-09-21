#ifndef VSTL_INITIALIZE_H
#define VSTL_INITIALIZE_H


#include <vstl/bits/vstl_construct.hpp>
#include <vstl/iterator.hpp>


namespace vstl
{

    /* 
     *  Copies the value to the range __fiter, __fiter + __n
     *  Exception handling intentionally ignored
     * 
     *  Should return the last element in the range
    */
    template <typename _FwdIter, typename _Size, typename _Tp>
    inline _FwdIter __init_with_value(_FwdIter __fiter, _Size __n, const _Tp& __val)
    {
        _FwdIter __cur = __fiter;

        for(; __n > 0; --__n, ++__cur)
            vstl::_Construct(__cur, __val);
        return __cur;
    };
    


    /* 
     * Special version of __init_with_value with additional allocator parameter
    */
    template <typename _FwdIter, typename _Size, typename _Tp, typename _Alloc>
    _FwdIter __init_with_value_a(_FwdIter __fiter, _Size __n, const _Tp& __val, _Alloc& __alloc)
    {
        typedef typename vstl::allocator_traits<_Alloc>     _Alloc_Traits;
        typedef typename vstl::iterator_traits<_FwdIter>::value_type __valtype;
        
        _FwdIter __cur = __fiter;

        for(; __n > 0; --__n, ++__cur)
            _Alloc_Traits::construct(__cur, __val);
        return __cur;
    };



    /* 
     *  Copies the default value to the range __fiter, __fiter + __n
     *  If iterator_traits<_FwdIter>::value_type is not default construtible,
     *  so using __init_with_default_value would be ill-formed
    */
    template <typename _FwdIter, typename _Size>
    inline _FwdIter __init_with_default_value(_FwdIter __fiter, _Size __n)
    {
        typedef typename vstl::iterator_traits<_FwdIter>::value_type __valtype;
        return __init_with_value(__fiter, __n, __valtype()); 
    };
}

#endif 