#ifndef VSTL_CLASS_SUPPORT_OP
    #define VSTL_CLASS_SUPPORT_OP

#include <vdetail/type_traits/primary_traits.hpp>
#include <vutility.hpp>

namespace vstl
{
    namespace detail
    {
        template <typename T> constexpr T& FUN(T& t) noexcept { return t; };

        template <typename T> void FUN(T&&) = delete;

        template <typename Base>
        vstl::true_type is_base_of_test_func(const volatile Base*);
        template <typename Base>
        vstl::false_type is_base_of_test_func(const volatile void *);

        template <typename Base, typename Derived>
        using pre_is_base_of = decltype(is_base_of_test_func<Base>(std::declval<Derived*>()));

        template <typename T, typename...Args>
        class is_constructible_base
        {
            private:
                template <typename C, typename...Kwargs>
                static decltype (T(vstl::declval<Kwargs>()...), int()) f(int) { return 0; };
                template <typename ...>
                static char f(...) { return 0; };
            public:
                static const bool value = (sizeof(decltype(f<T, Args...>(0))) == sizeof(int));
        };

        template <typename T>
        class is_nothrow_move_constructible_base
        {
            private:
                template <typename C>
                static vstl::conditional_t<noexcept(T(vstl::move(vstl::declval<T>()))), int, char> f(int) { return 0; };
                template <typename ...>
                static char f(...) { return 0; };
            public:
                static const bool value = (sizeof(decltype(f<T>(0))) == sizeof(int));
        };
    }

    template <typename Base, typename Derived>
    struct is_base_of : public vstl::conditional_t<
        vstl::is_class<Base>::value && vstl::is_class<Derived>::value, 
        vstl::detail::pre_is_base_of<Base, Derived>, vstl::false_type> {};

    template <typename Base, typename Derived>
    using is_base_of_v = typename vstl::is_base_of<Base, Derived>::value;

    template <typename T, typename... Args>
    struct is_constructible : public vstl::integral_constant<bool, detail::is_constructible_base<T, Args...>::value> {};

    template <typename T>
    using is_constructible_v = typename vstl::is_constructible<T>::value;

    template <typename T>
    struct is_default_constructible : public vstl::is_constructible<T> {};

    template <typename T>
    using is_default_constructible_v = typename vstl::is_default_constructible<T>::value;

    template <typename T>
    struct is_move_constructible : public vstl::is_constructible<T, typename vstl::add_rvalue_reference<T>::type> {};

    template <typename T>
    using is_move_constructible_v = typename vstl::is_move_constructible<T>::value; 

    template <typename T>
    struct is_copy_constructible : public vstl::is_constructible<T, typename vstl::add_lvalue_reference<
        typename vstl::add_const_t<T>>::type> {};

    template <typename T>
    using is_copy_constructible_v = typename vstl::is_copy_constructible<T>::value;

    template <typename T>
    struct is_nothrow_move_constructible : public vstl::integral_constant<bool, detail::is_nothrow_move_constructible_base<T>::value> {};

    template <typename T>
    using is_nothrow_move_constructible_v = typename vstl::is_nothrow_move_constructible<T>::value;
}

#endif 