#ifndef VSTL_CONSTRUCT_H
    #define VSTL_CONSTRUCT_H


#include <vstl/utility.hpp>


namespace vstl
{
    /* Plain construction of object */
    template <typename _Tp, typename... _Args>
    inline void _Construct(_Tp *__ptr, _Args&&... __args)
    {
        ::new(static_cast<void*>(__ptr)) _Tp(std::forward<_Args>(__args)...);
    };



    /* Plain destruction of object */
    template <typename _Tp, typename _Alloc>
    inline void _Destroy_a(_Tp *__ptr, _Alloc& __alloc)
    {
        if(__ptr)
            __alloc.destroy(__ptr);
    };



    /*
     *  Destroy the collection of objects
     *  Exception handling intentionally ignored for performance reason
    */
    template <typename _FwdIter, typename _Alloc>
    void _Destroy_a(_FwdIter __fiter, _FwdIter __biter, _Alloc& __alloc)
    {
        typedef typename vstl::iterator_traits<_FwdIter>::value_type __valtype;

        for(; __fiter != __biter; ++__fiter)
            _Destroy_a(&(*__fiter), __alloc);    
    };

}

#endif 