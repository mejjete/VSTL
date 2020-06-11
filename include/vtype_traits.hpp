#ifndef VSTL_TYPE_TRAITS
    #define VSTL_TYPE_TRAITS
#include <cstddef>
namespace vstl
{
    #define VSTL_PRIMARY_TYPE_CATEGORIES
    #define VSTL_MISCELLANIOUS_TRANSFORMATION
    #define VSTL_TYPE_PROPERTIES
    #define VSTL_COMPOSITE_TYPE_CATEGORIES
    #define VSTL_TYPE_RELATIONSHIP
    #define VSTL_MISCELLANIOUS_TRANSFORMATION
    #define VSTL_CONST_VOLATILE_MODIFICATION
 
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


    #ifdef VSTL_PRIMARY_TYPE_CATEGORIES

    //is_array
    template <typename T>
    struct is_array : public false_type {};

    template <typename T>
    struct is_array<T[]> : public true_type {};
    
    template <typename T, size_t F>
    struct is_array<T[F]> : public true_type {};

    template <typename T>
    struct is_floating_point : public integral_constant<bool, 
        is_same<float, typename remove_cv_t<T>, value> || 
        is_same<double, typename removecv_t<T>, value> ||
        is_same<long double, typename remove_cv_t<T>, value>> {};

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

    template<>
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

    #endif


    #ifdef VSTL_TYPE_RELATIONSHIP

    //is same value
    template <typename T, typename U>
    struct is_same : public vstl::false_type {};

    template <typename T>
    struct is_same<T, T> : public vstl::true_type {};

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
    using remove_cv_t = remove_cv<T>::type;

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
    using remove_const_t = remove_const<T>::type;

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
    using remove_volatile_t = remove_volatile<T>::type;

    #endif
}
#endif