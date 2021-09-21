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
    template <typename _Tp>
    inline void _Destroy(_Tp *__ptr)
    {
        __ptr->~_Tp();
    };



    /*
     *  Destroy the collection of objects
     *  Exception handling intentionally ignored for performance reason
    */
    template <typename _FwdIter>
    void _Destroy(_FwdIter __fiter, _FwdIter __biter)
    {
        typedef typename vstl::iterator_traits<_FwdIter>::value_type __valtype;

        for(; __fiter != __biter; ++__fiter)
            _Destroy(&(*__fiter));    
    };

}

#endif 