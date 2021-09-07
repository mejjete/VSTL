#ifndef VSTL_CLASS_SUPPORT_OP
    #define VSTL_CLASS_SUPPORT_OP


#include <vstl/vdetail/type_traits/primary_traits.hpp>
#include <vstl/vdetail/type_traits/libc_traits.hpp>
#include <vstl/utility.hpp>


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
        using pre_is_base_of = decltype(is_base_of_test_func<Base>(vstl::declval<Derived*>()));

        template <typename T, typename...Args>
        class is_constructible_base
        {
            private:
                template <typename C, typename...Kwargs>
                static decltype(T(vstl::declval<Kwargs>()...), int()) f(int) { return 0; };
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

    template <typename T, typename... Args>
    struct is_constructible : public vstl::integral_constant<bool, detail::is_constructible_base<T, Args...>::value> {};

    template <typename T, typename... Args>
    struct is_nothrow_contructible : vstl::conditional_t<noexcept(T(vstl::declval<Args>()...)),
        vstl::true_type, vstl::false_type> {};

    template <typename T>
    struct is_default_constructible : public vstl::is_constructible<T> {};

    template <typename T>
    struct is_nothrow_default_constructible : public vstl::is_nothrow_contructible<T> {};

    template <typename T>
    struct is_move_constructible : public vstl::is_constructible<T, typename vstl::add_rvalue_reference<T>::type> {};

    template <typename T>
    struct is_nothrow_move_constructible : public vstl::integral_constant<bool, detail::is_nothrow_move_constructible_base<T>::value> {};

    template <typename T>
    struct is_copy_constructible : public vstl::is_constructible<T, typename vstl::add_lvalue_reference<
        typename vstl::add_const_t<T>>::type> {};

    template <typename T>
    struct is_nothrow_copy_constructible : public vstl::is_nothrow_contructible<T, typename vstl::add_lvalue_reference<
        typename vstl::add_const_t<T>>::type> {}; 

    template <typename T, typename U, typename = __void_t<>>
    struct is_assignable : vstl::false_type {};

    template <typename T, typename U>
    struct is_assignable<T, U, __void_t<decltype(declval<T>() = declval<U>())>> : vstl::true_type {};

    template <typename T, typename U, typename = __void_t<>>
    struct is_nothrow_assignable : false_type {};

    template <typename T, typename U>
    struct is_nothrow_assignable<T, U, __void_t<decltype(declval<T>() = declval<U>())>> : vstl::conditional_t<
        noexcept(declval<T>() = declval<U>()), vstl::true_type, vstl::false_type> {};

    template <typename T>
    struct is_copy_assignable : public vstl::is_assignable<
        typename vstl::add_lvalue_reference<T>::type, typename vstl::add_lvalue_reference<vstl::add_const_t<T>>::type> {};

    template <typename T>
    struct is_nothrow_copy_assignable : public vstl::is_nothrow_assignable<
        typename vstl::add_lvalue_reference<T>::type, typename vstl::add_lvalue_reference<vstl::add_const_t<T>>::type> {};

    template <typename T>
    struct is_move_assignable : public vstl::is_assignable<
        typename vstl::add_lvalue_reference<T>::type, typename vstl::add_rvalue_reference_t<T>::type> {};

    template <typename T>
    struct is_nothrow_move_assignable : public vstl::is_nothrow_assignable<
        typename vstl::add_lvalue_reference<T>::type, typename vstl::add_rvalue_reference_t<T>::type> {};

    template <typename T, typename = __void_t<>>
    struct is_destructible : vstl::false_type {};

    template <typename T>
    struct is_destructible<T, __void_t<decltype(vstl::declval<T&>().~T())>> : public vstl::true_type {};    
}

#endif 