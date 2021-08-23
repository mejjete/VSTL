#ifndef VSTL_LIBC_TRAITS
    #define VSTL_LIBC_TRAITS

namespace vstl
{
    /* used for undefined entities */
    class __undefined;

    template <typename ...>
    using __void_t = void;

    #define VSTL_HAS_NESTED_TYPE(_NTYPE)                            \
    template<typename _Tp, typename = __void_t<>>                   \
    struct __has_##_NTYPE : public vstl::false_type {};             \
    template<typename _Tp>                                          \
    struct __has_##_NTYPE<_Tp, __void_t<typename _Tp::_NTYPE>>      \
    : public vstl::true_type {};                                         

    /*
        VSTL_NESTED_TYPE returns true if 
        _TYPE::_SUBTYPE is exist, otherwise false
        Must be used after VSTL_HAS_NESTED_TYPE 
    */
    #define VSTL_NESTED_TYPE(_TYPE, _SUBTYPE)   \
    __has_##_SUBTYPE<_TYPE>::value

        //primary template
    //negative case 
    template <  typename Default, class, 
                template<typename...> class Op,
                typename... Args>
    struct detector 
    {
        constexpr static bool value = false;
        using type = Default;
    };


    //specialization recognizes and handles only types supporting Op
    //positive case
    template <  typename Default, 
                template<typename...> class Op,
                typename... Args>
    struct detector<Default, __void_t<Op<Args...>>, Op, Args...>
    {
        constexpr static bool value = true;
        using type = Op<Args...>;
    };


    template <template<typename...> class Op, typename... Args>
    using is_detected = detector<void, void, Op, Args...>;


    template <template<typename...> class Op, typename... Args>
    using detected_t = typename is_detected<Op, Args...>::type;


    template <typename Default, template <typename...> class Op, typename... Args>
    using detected_or = detector<Default, void, Op, Args...>;


    template <typename Default, template <typename...> class Op, typename... Args>
    using detected_or_t = typename detected_or<Default, Op, Args...>::type;
};

#endif 