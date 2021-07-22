#ifndef VSTL_TUPLE
    #define VSTL_TUPLE

namespace vstl
{
    template <typename... Ts>
    struct tuple {};

    template <typename T, typename... Ts>
    struct tuple<T, Ts...> : tuple<Ts...> 
    {
        tuple(T t, Ts... ts) : tuple<Ts...>(ts...), tail(t) {};
        T tail;
    };

    template <size_t, typename>
    struct elem_type_holder;

    template <typename T, typename... Ts>
    struct elem_type_holder<0, tuple<T, Ts...>> 
    {
        typedef T type;
    };

    template <size_t k, typename T, typename... Ts>
    struct elem_type_holder<k, tuple<T, Ts...>>
    {
        typedef typename elem_type_holder<k - 1, tuple<Ts...>>::type type;
    };

    template <size_t k, typename... Ts>
    typename vstl::enable_if<k == 0, 
        typename elem_type_holder<0, tuple<Ts...>>::type&>::type
    get(tuple<Ts...>&t)
    {
        return t.tail;
    };

    template <size_t k, typename T, typename... Ts>
    typename vstl::enable_if<k != 0,
        typename elem_type_holder<k, tuple<T, Ts...>>::type&>::type
    get(tuple<T, Ts...>& t)
    {
        tuple<Ts...>& base = t;
        return get<k - 1>(base); 
    };
}

#endif