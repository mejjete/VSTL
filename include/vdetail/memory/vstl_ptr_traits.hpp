#ifndef VSTL_PTR_TRAITS
    #define VSTL_PTR_TRAITS

namespace vstl
{
    /* 
     * pointer_trairs is an adapter template class
     * for certain attributes for pointer and 
     * pointer like classes
     * 
     *      STD implementation
    */

    class __undefined;

    template <typename T>
    struct __get_first_arg
    {
        using type = __undefined;
    };

    template <typename T>
    using __get_first_arg_t = typename __get_first_arg<T>::type;

    template <typename Tp>
    using __make_not_void = typename vstl::conditional<
        vstl::is_void<Tp>::value, __undefined, Tp>::type;

    template <typename Ptr>
    struct pointer_traits
    {
        using pointer = Ptr;
        using element_type = typename Ptr::element_type;
        using difference_type = std::ptrdiff_t;

        template <typename U>
        using rebing = U*;

        static Ptr pointer_to(__make_not_void<element_type> &__arg)
        { return Ptr::pointer_to(arg); };
    };

    template <typename T>
    struct pointer_traits<T*>
    {
        using pointer = T*;
        using element_type = T;
        using difference_type = std::ptrdiff_t;

        template <typename U>
        using rebing = U*;

        static pointer pointert_to()
    };
};

#endif