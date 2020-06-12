#ifndef VSTL_TYPE_TRAITS
    #define VSTL_TYPE_TRAITS
#include <cstddef>
#include <cstdint>
#include <cuchar>
namespace vstl
{
    #define VSTL_PRIMARY_TYPE_CATEGORIES
    #define VSTL_MISCELLANIOUS_TRANSFORMATION
    #define VSTL_TYPE_PROPERTIES
    #define VSTL_COMPOSITE_TYPE_CATEGORIES
    #define VSTL_TYPE_RELATIONSHIP
    #define VSTL_MISCELLANIOUS_TRANSFORMATION
    #define VSTL_CONST_VOLATILE_MODIFICATION
 
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

    #ifdef VSTL_CONST_VOLATILE_MODIFICATION

    //remove_cv
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

    //remove_const
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

    //remove volatile
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

    #endif

    #ifdef VSTL_TYPE_RELATIONSHIP

    //is-same
    template <typename T, typename U>
    struct is_same : public vstl::false_type {};

    template <typename T>
    struct is_same<T, T> : public vstl::true_type {};

    #endif

    #ifdef VSTL_TYPE_PROPERTIES

    //is const value
    template <typename T>
    struct is_const : public vstl::false_type {};

    template <typename T>
    struct is_const<const T> : public vstl::true_type {};

    //is volatile
    template <typename T>
    struct is_volatile : public vstl::false_type {};

    template <typename T>
    struct is_volatile<volatile T> : public vstl::true_type {};

    template <typename T>
    struct is_volatile<const volatile T> : public vstl::false_type {};

    #endif

    #ifdef VSTL_COMPOSITE_TYPE_CATEGORIES

    //is reference
    template <typename T>
    struct is_reference : public vstl::false_type {};

    template <typename T>
    struct is_reference<T&> : public vstl::true_type {};

    template <typename T>
    struct is_reference<const T&> : public vstl::true_type {};

    //is member pointer
    template <typename T>
    struct is_member_pointer_helper : public vstl::false_type {};

    template <typename T, typename U>
    struct is_member_pointer_helper<T U::*> : public vstl::true_type {};

    template <typename T>
    struct is_member_pointer : public vstl::is_member_pointer_helper<typename vstl::remove_cv<T>::type> {};

    #endif

    #ifdef VSTL_MISCELLANIOUS_TRANSFORMATION

    //enable if
    template <bool B, typename T = void>
    struct enable_if {};
    
    template <typename T>
    struct enable_if<true, T> { typedef T type; };

    template <bool B, typename T = void>
    using enable_if_t = typename enable_if<B, T>::type;

    //disable if
    template <bool b, typename T = void>
    struct disable_if {};

    template <typename T>
    struct disable_if<false, T> { typedef T type; };

    template <bool B, typename T>
    using disable_if_t = typename enable_if<B, T>::type;

    //conditional
    template <bool B, typename T, typename F>
    struct conditional { typedef T type; };

    template <typename T, typename F>
    struct conditional<false, T, F> { typedef F type; };

    template <bool B, typename T, typename F>
    using conditional_t = typename conditional<B, T, F>::type;

    #endif

    #ifdef VSTL_PRIMARY_TYPE_CATEGORIES

    //is_array
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
    using is_floating_point_v = typename vstl::is_floating_point<T>::value;

    //is_integral
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
    using is_integral_v = typename vstl::is_integral<T>::value;

    //is_arithmetic
    template <typename T>
    struct is_arithmetic : vstl::integral_constant<bool, 
        vstl::is_integral<T>::value || vstl::is_floating_point<T>::value> {};

    //is union
    template <typename T>
    struct is_union : public vstl::integral_constant<bool, __is_union(T)> {};

    //is class
    template <typename T>
    struct is_class : public vstl::integral_constant<bool, sizeof(vstl::detail::test<T>(0)) == 1 && !vstl::is_union<T>::value> {};

    //is pointer
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

    //is void 
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

    //is null pointer
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

    //is function
    template <typename T>
    struct is_function : public vstl::false_type {};

    template <typename RetType, typename... Args>
    struct is_funciton : public vstl::false_type {};

    template <typename RetType, typename... Args>
    struct is_function<RetType(Args...)> : public vstl::true_type {};

    //for function with const/volatile quialifiers
    
    template <typename RetType, typename... Args>
    struct is_function<RetType(Args...) const> : public vstl::true_type {};

    template <typename RetType, typename... Args>
    struct is_function<RetType(Args...) volatile> : public vstl::true_type {};
    
    template <typename RetType, typename... Args>
    struct is_function<RetType(Args...) const volatile> : public vstl::true_type {};

    //for function with const/volatile references

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

     //is member function pointer
    template <typename T>
    struct is_member_function_pointer_helper : public vstl::false_type {};

    template <typename T, typename U>
    struct is_member_function_pointer_helper<T U::*> : vstl::is_function<T> {};

    template <typename T>
    struct is_member_function_pointer : vstl::is_member_function_pointer_helper
    <typename vstl::remove_cv<T>::type> {};

    //is member object pointer
    template <typename T>
    struct is_member_object_pointer : public vstl::integral_constant<bool, 
        vstl::is_member_pointer<T>::value && !vstl::is_member_function_pointer<T>::value> {};

    #endif

}
#endif