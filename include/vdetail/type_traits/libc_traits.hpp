#ifndef VSTL_LIBC_TRAITS
    #define VSTL_LIBC_TRAITS

namespace vstl
{
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
};

#endif 