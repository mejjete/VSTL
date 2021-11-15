#ifndef VSTL_ITERATOR_FUNC_H
#define VSTL_ITERATOR_FUNC_H


#include <vstl/iterator.hpp>
#include <vstl/bits/vstl_iterator.hpp>


namespace vstl
{
    template <typename _Iter>
    using __iter_diff_t = typename vstl::iterator_traits<_Iter>::difference_type;


    template <typename _Iter>
    inline __iter_diff_t<_Iter> __distance(_Iter __pos1, _Iter __pos2, vstl::random_access_iterator_tag)
    {
        return __pos2 - __pos1;
    };


    template <typename _Iter>
    inline __iter_diff_t<_Iter> __distance(_Iter __pos1, _Iter __pos2, vstl::input_iterator_tag)
    {
        __iter_diff_t<_Iter> __diff = 0;

        for(; __pos1 != __pos2; ++__pos1)
            __diff++;
        return __diff;
    };


    /**
     *  Calculates the distance between two iterators. Both iterators have to 
     *  refer to elements of the same container. If the iterators are not 
     *  random-access iterator, __pos2 must be reachable from __pos1, thus, it must 
     *  have the same or later position.
     * 
     *  This function uses the best implementation according to the iterator category
    */
    template <typename _Iter>
    __iter_diff_t<_Iter> distance(_Iter __pos1, _Iter __pos2)
    {
        return __distance(__pos1, __pos2, __iter_category_t<_Iter>(__iter_category_t<_Iter>()));
    };


    template <typename _Iter>
    inline void __advance(_Iter& __iter, __iter_diff_t<_Iter> __n, vstl::input_iterator_tag)
    {
        static_assert(__n >= 0);
        while(--__n)
            ++__iter;
    };


    template <typename _Iter>
    inline void __advance(_Iter& __iter, __iter_diff_t<_Iter> __n, vstl::bidirectional_iterator_tag)
    {
        if(__n > 0)
        {
            while(__n--)
                ++__iter;
        }
        else 
        {
            while(__n++)
                --__iter;
        }
    };


    template <typename _Iter>
    inline void __advance(_Iter& __iter, __iter_diff_t<_Iter> __n, vstl::random_access_iterator_tag)
    {
        __iter += __n;
    };


    /**
     *  Increments iterator by __n position forward or backward, depending on the sign. 
     *  Always uses the best implementation, depending on the iterator category
    */
    template <typename _Iter>
    void advance(_Iter& __iter, __iter_diff_t<_Iter> __n)
    {
        /** 
         *  Contructor argument being passed because default constructor are not
         *  implemented for input and output iterators
        */
        __advance(__iter, __n, __iter_category_t<_Iter>(__iter_category_t<_Iter>()));
    };



    /**
     *  Yields the position to move forward from the current state
    */
    template <typename _ForwardIter>
    _ForwardIter next(_ForwardIter __iter, __iter_diff_t<_ForwardIter> __n = 1)
    {
        advance(__iter, __n);
        return __iter;
    };



    /**
     *  Yields the position to move backward from the current state
    */ 
   template <typename _BidirectionalIter>
   _BidirectionalIter prev(_BidirectionalIter __iter, __iter_diff_t<_BidirectionalIter> __n = 1)
   {
       __advance(__iter, -__n);
       return __iter;
   };
};

#endif 