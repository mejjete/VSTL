#ifndef VSTL_COMPLITE_TRAITS
    #define VSTL_COMPLITE_TRAITS

#include <vstl/vdetail/type_traits/primary_traits.hpp>

namespace vstl
{
    namespace detail
    {
        template <typename T>
        struct type_identity { using type = T; };

        template <typename T>
        auto try_add_lvalue_reference(int) -> type_identity<T&>;

        template <typename T>
        auto try_add_lvalue_reference(...) -> type_identity<T>;

        template <typename T>
        auto try_add_rvalue_reference(int) -> type_identity<T&&>;

        template <typename T>
        auto try_add_rvalue_reference(...) -> type_identity<T>;

        template <typename T>
        auto try_add_pointer(int) -> type_identity<typename vstl::remove_reference_t<T>*>;

        template <typename T>
        auto try_add_pointer(...) -> type_identity<T>;
    }

    template <typename T>
    struct remove_extent 
    {
        typedef T type;
    };

    template <typename T>
    struct remove_extent<T[]>
    {
        typedef T type;
    };

    template <typename T, size_t N>
    struct remove_extent<T[N]>
    {
        typedef T type;
    };

    template <typename T>
    using remove_extent_t = typename vstl::remove_extent<T>::type;

    template <typename T>
    struct remove_all_extents
    {
        typedef T type;
    };

    template <typename T>
    struct remove_all_extents<T[]>
    {
        typedef typename vstl::remove_all_extents<T>::type type;
    };

    template <typename T, size_t N>
    struct remove_all_extents<T[N]>
    {
        typedef typename vstl::remove_all_extents<T>::type tye;
    };

    template <typename T>
    using remove_all_extents_t = typename vstl::remove_all_extents<T>;

    template <typename T>
    struct remove_pointer
    {
        typedef T type;
    };

    template <typename T>
    struct remove_pointer<T*>
    {
        typedef T type;
    };

    template <typename T>
    struct remove_pointer<T* const>
    {
        typedef T type;
    };

    template <typename T>
    struct remove_pointer<T* volatile>
    {
        typedef T type;
    };

    template <typename T>
    struct remove_pointer<T* const volatile>
    {
        typedef T type;
    };

    template <typename T>
    using remove_pointer_t = typename vstl::remove_pointer<T>::type;

    template <typename T>
    struct add_pointer : decltype(detail::try_add_pointer<T>(0)) {};

    template <typename T>
    using add_pointer_t = typename vstl::add_pointer<T>::type;

    template <typename T>
    struct add_lvalue_reference : decltype(detail::try_add_lvalue_reference<T>(0)) {};

    template <typename T>
    using add_lvalue_reference_t = typename vstl::add_lvalue_reference<T>::type;

    template <typename T>
    struct add_rvalue_reference : decltype(detail::try_add_rvalue_reference<T>(0)) {};

    template <typename T>
    using add_rvalue_reference_t = typename vstl::add_rvalue_reference<T>::type;

    template <typename T>
    struct add_const
    {
        typedef const T type;
    };

    template <typename T>
    using add_const_t = typename vstl::add_const<T>::type;
}

#endif