#ifndef VSTL_ITERATOR_BASE
    #define VSTL_ITERATOR_BASE


#include <vstl/type_traits.hpp>
#include <iterator>


namespace vstl
{
#ifdef _STL_ITERATOR_BASE_TYPES_H

    /**
     *  The reason for that awkward iterator tag hierarchy is to stay 
     *  compatible with standard iterator traits mechanism, and as result,
     *  with almost all std algorithms
     */
    struct input_iterator_tag : std::input_iterator_tag {};
    struct output_iterator_tag : std::output_iterator_tag {};
#else 

    struct input_iterator_tag {};
    struct output_iterator_tag {};

#endif 

    struct forward_iterator_tag : input_iterator_tag {};
    struct bidirectional_iterator_tag : forward_iterator_tag {};
    struct random_access_iterator_tag :  bidirectional_iterator_tag {};


    /* 
     *  Iterator traits helper
     *  Whenever _Iter is not an stl-like iterator type, than instance of 
     *  __iterator_traits is ill-formed and produce no typedefs at all
    */
    template <typename _Iter, typename = __void_t<>>
    struct __iterator_traits {};


    template <typename _Iter>
    struct __iterator_traits<_Iter, __void_t<
        typename _Iter::value_type,
        typename _Iter::difference_type,
        typename _Iter::iterator_category,
        typename _Iter::pointer,
        typename _Iter::reference>> 
    {
        typedef typename _Iter::value_type              value_type;
        typedef typename _Iter::difference_type         difference_type;
        typedef typename _Iter::iterator_category       iterator_category;
        typedef typename _Iter::pointer                 pointer;
        typedef typename _Iter::reference               reference;
    };


    // Iterator traits itself
    template <typename _Iter>
    struct iterator_traits : __iterator_traits<_Iter> {};

    // Partial specialization
    template <typename _Iter>
    struct iterator_traits<_Iter*>
    {
        typedef _Iter                               value_type;
        typedef ptrdiff_t                           difference_type;
        typedef vstl::random_access_iterator_tag    iterator_category;
        typedef _Iter*                              pointer;
        typedef _Iter&                              reference;
    };


    template <typename _Iter>
    struct iterator_traits<const _Iter*>
    {
        typedef _Iter                               value_type;
        typedef ptrdiff_t                           difference_type;
        typedef vstl::random_access_iterator_tag    iterator_category;
        typedef const _Iter*                        pointer;
        typedef const _Iter&                        reference;
    };

    
    template <typename _Iterator>
    using __iter_category_t = typename vstl::iterator_traits<_Iterator>::iterator_category;

    template <typename _Iterator>
    using _RequireInputIter = enable_if_t<is_convertible<_Iterator, vstl::input_iterator_tag>::value>;
}
#endif