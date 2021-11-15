#ifndef VSTL_ITERATOR_BASE
    #define VSTL_ITERATOR_BASE


#include <vstl/type_traits.hpp>


namespace vstl
{
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};


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

    
    template <typename T>
    struct is_input_iterator : public vstl::integral_constant<bool, vstl::is_same<
        typename vstl::iterator_traits<T>::iterator_category, input_iterator_tag>::value> {};

    template <typename T>
    struct is_output_iterator : public vstl::integral_constant<bool, vstl::is_same<
        typename vstl::iterator_traits<T>::iterator_category, output_iterator_tag>::value> {}; 

    template <typename T>
    struct is_forward_iterator : public vstl::integral_constant<bool, vstl::is_same<
        typename vstl::iterator_traits<T>::iterator_category, forward_iterator_tag>::value> {};

    template <typename T>
    struct is_bidirectional_iterator : public vstl::integral_constant<bool, vstl::is_same<
        typename vstl::iterator_traits<T>::iterator_category, bidirectional_iterator_tag>::value> {};

    template <typename T>
    struct is_random_access_iterator : public vstl::integral_constant<bool, vstl::is_same<
        typename vstl::iterator_traits<T>::iterator_category, vstl::random_access_iterator_tag>::value> {};
    
    template <typename _Iterator>
    using __iter_category_t = typename vstl::iterator_traits<_Iterator>::iterator_category;


    /*
     *  Instead of using is_convertible<Iterator::category, input_iterator_tag>
     *  is_assignable can be applied in the same context
    */
    template <typename _Iterator, typename _Required>
    using _RequireIter_t = enable_if_t<is_assignable<add_lvalue_reference_t<__iter_category_t<_Iterator>>, 
        add_rvalue_reference_t<_Required>>::value, _Required>;
    

    template <typename _Iter, typename _Required, typename = __void_t<>>
    struct _RequireIter_v : vstl::false_type {};

    template <typename _Iter, typename _Required>
    struct _RequireIter_v<_Iter, _Required, __void_t<_RequireIter_t<_Iter, _Required>>> : vstl::true_type {};


    template <typename _Iterator>
    using _RequireInputIter = enable_if_t<is_assignable<add_lvalue_reference_t<__iter_category_t<_Iterator>>, 
        add_rvalue_reference_t<input_iterator_tag>>::value, input_iterator_tag>;
}
#endif