#ifndef VSTL_PRIMARY_TRAITS
    #define VSTL_PRIMARY_TRAITS

namespace vstl
{
    namespace detail
    {
        template <typename T>
        char test(int T::*);
        struct two { char c[2]; };
        template <typename T>
        two test(...);
    }

    template <typename T, T v>
    struct integral_constant
    {
        static constexpr T value = v;
        typedef T value_type;
        typedef integral_constant<T, v> type;
        constexpr operator T() { return v; };
    };

    typedef integral_constant<bool, true>   true_type;
    typedef integral_constant<bool, false>  false_type;

    template <typename T>
    struct remove_reference
    {
        typedef T type;
    };

    template <typename T>
    struct remove_reference<T&>
    {
        typedef T type;
    };

    template <typename T>
    struct remove_reference<T&&>
    {
        typedef T type;
    };
    
    template <typename T>
    using remove_reference_t = typename vstl::remove_reference<T>::type;

    template <typename T>
    struct remove_cv
    {
        typedef T type;
    };

    template <typename T>
    struct remove_cv<const T>
    {
        typedef T type;
    };

    template <typename T>
    struct remove_cv<volatile T>
    {
        typedef T type;
    };

    template <typename T>
    struct remove_cv<const volatile T>
    {
        typedef T type;
    };

    template <typename T>
    using remove_cv_t = typename remove_cv<T>::type;

    template <typename T>
    struct remove_const
    {
        typedef T type;
    };

    template <typename T>
    struct remove_const<const T>
    {
        typedef T type;
    };

    template <typename T>
    using remove_const_t = typename remove_const<T>::type;

    template <typename T>
    struct remove_volatile
    {
        typedef T type;
    };

    template <typename T>
    struct remove_volatile<volatile T>
    {
        typedef T type;
    };

    template <typename T>
    using remove_volatile_t = typename remove_volatile<T>::type;

    template <typename T>
    struct remove_cvref
    {
        typedef vstl::remove_cv_t<vstl::remove_reference_t<T>> type;
    };

    template <typename T>
    using remove_cvref_t = typename vstl::remove_cv<T>::type;

    template <typename T, typename U>
    struct is_same : public vstl::false_type {};

    template <typename T>
    struct is_same<T, T> : public vstl::true_type {};

    template <typename T>
    struct is_const : public vstl::false_type {};

    template <typename T>
    struct is_const<const T> : public vstl::true_type {};

    template <typename T>
    struct is_volatile : public vstl::false_type {};

    template <typename T>
    struct is_volatile<volatile T> : public vstl::true_type {};

    template <typename T>
    struct is_volatile<const volatile T> : public vstl::true_type {};

    template <typename T>
    struct is_reference : public vstl::false_type {};

    template <typename T>
    struct is_reference<T&> : public vstl::true_type {};

    template <typename T>
    struct is_reference<const T&> : public vstl::true_type {};

    template <typename T>
    struct is_member_pointer_helper : public vstl::false_type {};

    template <typename T, typename U>
    struct is_member_pointer_helper<T U::*> : public vstl::true_type {};

    template <typename T>
    struct is_member_pointer : public vstl::is_member_pointer_helper<typename vstl::remove_cv<T>::type> {};

    template <bool B, typename T = void>
    struct enable_if {};
    
    template <typename T>
    struct enable_if<true, T> { typedef T type; };

    template <bool B, typename T = void>
    using enable_if_t = typename enable_if<B, T>::type;

    template <bool b, typename T = void>
    struct disable_if {};

    template <typename T>
    struct disable_if<false, T> { typedef T type; };

    template <bool B, typename T>
    using disable_if_t = typename enable_if<B, T>::type;

    template <bool B, typename T, typename F>
    struct conditional { typedef T type; };

    template <typename T, typename F>
    struct conditional<false, T, F> { typedef F type; };

    template <bool B, typename T, typename F>
    using conditional_t = typename conditional<B, T, F>::type;

    template <typename T>
    struct is_array : public false_type {};

    template <typename T>
    struct is_array<T[]> : public true_type {};
    
    template <typename T, size_t F>
    struct is_array<T[F]> : public true_type {};

    template <typename T>
    struct is_floating_point : vstl::integral_constant<bool,
        vstl::is_same<float, typename vstl::remove_cv<T>::type>::value  ||
        vstl::is_same<double, typename vstl::remove_cv<T>::type>::value  ||
        vstl::is_same<long double, typename vstl::remove_cv<T>::type>::value> {};

    template <typename T>
    struct is_integral : public vstl::integral_constant<bool, 
        vstl::is_same<bool, typename vstl::remove_cv<T>::type>::value ||
        vstl::is_same<char, typename vstl::remove_cv<T>::type>::value ||
        vstl::is_same<char16_t, typename vstl::remove_cv<T>::type>::value ||
        vstl::is_same<char32_t, typename vstl::remove_cv<T>::type>::value ||
        vstl::is_same<wchar_t, typename vstl::remove_cv<T>::type>::value ||
        vstl::is_same<signed char, typename vstl::remove_cv<T>::type>::value ||
        vstl::is_same<short int, typename vstl::remove_cv<T>::type>::value ||
        vstl::is_same<int, typename vstl::remove_cv<T>::type>::value ||
        vstl::is_same<long int, typename vstl::remove_cv<T>::type>::value ||
        vstl::is_same<long long int, typename vstl::remove_cv<T>::type>::value ||
        vstl::is_same<unsigned char, typename vstl::remove_cv<T>::type>::value ||
        vstl::is_same<unsigned short int, typename vstl::remove_cv<T>::type>::value ||
        vstl::is_same<unsigned int, typename vstl::remove_cv<T>::type>::value ||
        vstl::is_same<unsigned long int, typename vstl::remove_cv<T>::type>::value ||
        vstl::is_same<unsigned long long int, typename vstl::remove_cv<T>::type>::value> {};

    template <typename T>
    struct is_arithmetic : vstl::integral_constant<bool, 
        vstl::is_integral<T>::value || vstl::is_floating_point<T>::value> {};

    template <typename T>
    struct is_lvalue_reference : public vstl::false_type {};

    template <typename T>
    struct is_lvalue_reference<T&> : public vstl::true_type {};

    template <typename T>
    struct is_rvalue_reference : public vstl::false_type {};

    template <typename T>
    struct is_rvalue_reference<T&&> : public vstl::true_type {};

    template <typename T>
    struct is_pointer
    {
        static const bool value = false;
    };

    template <typename T>
    struct is_pointer<T*>
    {
        static const bool value = true;
    };

    template <typename T>
    struct is_void
    {
        static const bool value = false;
    };

    template <>
    struct is_void<void>
    {
        static const bool value = true;
    };

    template <typename T>
    struct is_null_pointer
    {
        static const bool value = false;
    };

    template <>
    struct is_null_pointer<std::nullptr_t>
    {
        static const bool value = true;
    };

    template <typename T>
    struct is_function : public vstl::false_type {};

    template <typename RetType, typename... Args>
    struct is_funciton : public vstl::false_type {};

    template <typename RetType, typename... Args>
    struct is_function<RetType(Args...)> : public vstl::true_type {};
    
    template <typename RetType, typename... Args>
    struct is_function<RetType(Args...) const> : public vstl::true_type {};

    template <typename RetType, typename... Args>
    struct is_function<RetType(Args...) volatile> : public vstl::true_type {};
    
    template <typename RetType, typename... Args>
    struct is_function<RetType(Args...) const volatile> : public vstl::true_type {};

    template <typename RetType, typename... Args>
    struct is_function<RetType(Args...) &> : public vstl::true_type {};

    template <typename RetType, typename... Args>
    struct is_function<RetType(Args...) const &> : public vstl::true_type {};

    template <typename RetType, typename... Args>
    struct is_function<RetType(Args...) volatile &> : public vstl::true_type {};

    template <typename RetType, typename... Args>
    struct is_function<RetType(Args...) const volatile &> : public vstl::true_type {};

    template <typename RetType, typename... Args>
    struct is_function<RetType(Args...) &&> : public vstl::true_type {};

    template <typename RetType, typename... Args>
    struct is_function<RetType(Args...) const &&> : public vstl::true_type {};

    template <typename RetType, typename... Args>
    struct is_function<RetType(Args...) volatile &&> : public vstl::true_type {};

    template <typename RetType, typename... Args>
    struct is_function<RetType(Args...) const volatile &&> : public vstl::true_type {};

    template <typename T>
    struct is_member_function_pointer_helper : public vstl::false_type {};

    template <typename T, typename U>
    struct is_member_function_pointer_helper<T U::*> : vstl::is_function<T> {};

    template <typename T>
    struct is_member_function_pointer : vstl::is_member_function_pointer_helper
    <typename vstl::remove_cv<T>::type> {};

    template <typename T>
    struct is_member_object_pointer : public vstl::integral_constant<bool, 
        vstl::is_member_pointer<T>::value && !vstl::is_member_function_pointer<T>::value> {};

    template <typename T>
    struct is_union : public vstl::integral_constant<bool, __is_union(T)> {};

    template <typename T>
    struct is_class : public vstl::integral_constant<bool, 
        sizeof(vstl::detail::test<T>(0)) == 1 && !vstl::is_union<T>::value> {};

    // same implementation
    // template <typename T>
    // class is_class 
    // {
    //     private: 
    //         template <typename Q>
    //         static int _m(int C::* member) { return 0; };
    //         template <typename ...>
    //         static char _m(...) { return 0; };
    //     public:
    //         static const bool value = sizeof(_m<T>(0) == sizeof(int);
    // };

    template <typename T>
    struct is_enum : public vstl::integral_constant<bool, 
        !vstl::is_void<T>::value &&
        !vstl::is_integral<T>::value &&
        !vstl::is_floating_point<T>::value &&
        !vstl::is_array<T>::value &&
        !vstl::is_pointer<T>::value &&
        !vstl::is_reference<T>::value &&
        !vstl::is_member_pointer<T>::value &&
        !vstl::is_union<T>::value &&
        !vstl::is_class<T>::value &&
        !vstl::is_function<T>::value> {};

    template <typename T>
    struct is_fundamental : public vstl::integral_constant<bool, 
        is_arithmetic<T>::value ||
        is_void<T>::value ||
        is_null_pointer<T>::value> {};

    template <typename T>
    using is_fundamental_t = typename vstl::is_fundamental<T>::type;

    template <typename T>
    struct is_scalar : public vstl::integral_constant<bool, 
        vstl::is_arithmetic<T>::value ||
        vstl::is_enum<T>::value ||
        vstl::is_pointer<T>::value ||
        vstl::is_member_pointer<T>::value ||
        vstl::is_null_pointer<T>::value> {};

    template <typename T>
    struct is_object : public vstl::integral_constant<bool, 
        vstl::is_scalar<T>::value ||
        vstl::is_array<T>::value ||
        vstl::is_union<T>::value ||
        vstl::is_class<T>::value> {};

    template <typename T>
    using is_object_t = typename vstl::is_object<T>::value;

    template <typename T, bool = vstl::is_arithmetic<T>::value>
    struct is_unsigned : public std::integral_constant<bool, T(0) < T(-1)> {};

    template <typename T>
    struct is_unsigned<T, false> : public std::false_type {};

    template <typename T, bool = 
        vstl::is_arithmetic<T>::value &&
        !vstl::is_unsigned<T>::value>
    struct is_signed : public vstl::true_type {};

    template <typename T>
    struct is_signed<T, false> : public vstl::false_type {};

    template <typename T>
    struct is_empty : public vstl::integral_constant<bool,
        !vstl::is_integral<T>::value && sizeof(T) == 1> {};
}

#endif