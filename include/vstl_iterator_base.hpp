#ifndef VSTL_ITERATOR_BASE
    #define VSTL_ITERATOR_BASE
namespace vstl
{
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

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
}
#endif