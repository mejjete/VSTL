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

    //test sample for stupid tasks in my mind
    struct base_iterator_tag;
    
    //iterator traits

    template <typename T>
    struct iterator_traits
    {
        typedef typename T::value_type              value_type;
        typedef typename T::difference_type         difference_type;
        typedef typename T::iterator_category       iterator_category;
        typedef typename T::pointer                 pointer;
        typedef typename T::reference               reference;
    };

    template <typename T>
    struct iterator_traits<T*>
    {
        typedef T                                   value_type;
        typedef ptrdiff_t                           difference_type;
        typedef vstl::random_access_iterator_tag    iterator_category;
        typedef T*                                  pointer;
        typedef T&                                  reference;
    };

    
    template <typename T>
    struct is_input_iterator : public vstl::integral_constant<bool, vstl::is_same<
        typename vstl::iterator_traits<T>::iterator_category, input_iterator_tag>::value> {};
    
    template <typename T>
    using is_input_iterator_v = typename vstl::is_input_iterator<T>::value;

    template <typename T>
    struct is_output_iterator : public vstl::integral_constant<bool, vstl::is_same<
        typename vstl::iterator_traits<T>::iterator_category, output_iterator_tag>::value> {}; 

    template <typename T>
    using is_output_iterator_v = typename vstl::is_output_iterator<T>::value;

    template <typename T>
    struct is_forward_iterator : public vstl::integral_constant<bool, vstl::is_same<
        typename vstl::iterator_traits<T>::iterator_category, forward_iterator_tag>::value> {};
    
    template <typename T>
    using is_forward_iterator_v = typename vstl::is_forward_iterator<T>::value;

    template <typename T>
    struct is_bidirectional_iterator : public vstl::integral_constant<bool, vstl::is_same<
        typename vstl::iterator_traits<T>::iterator_category, bidirectional_iterator_tag>::value> {};
    
    template <typename T>
    using is_bidirectional_iterator_v = typename vstl::is_bidirectional_iterator<T>::value;

    template <typename T>
    struct is_random_access_iterator : public vstl::integral_constant<bool, vstl::is_same<
        typename vstl::iterator_traits<T>::iterator_category, vstl::random_access_iterator_tag>::value> {};
    
    template <typename T>
    using is_random_access_iterator_v = typename vstl::is_random_access_iterator<T>::value;
}
#endif