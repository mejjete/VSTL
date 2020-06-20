#ifndef VSTL_UTILITY
    #define VSTL_UTILITY
#include "type_traits"

namespace vstl
{

    //move
    template <typename T>
    typename vstl::remove_reference<T>::type&& move(T&& arg) noexcept
    {
        return static_cast<typename vstl::remove_reference<T>::type &&>(arg);
    };

    //forward
    template <typename T>
    constexpr T&& forward(typename vstl::remove_reference<T>::type& arg) noexcept
    {
        return static_cast<T&&>(arg);
    };

    template <typename T>
    constexpr T&& forward(typename vstl::remove_reference<T>::type&& arg) noexcept
    {
        static_assert(!vstl::is_lvalue_reference<T>::value, "invalud rvalue to lvalue conversion");
        return static_cast<T&&>(arg);
    };

    //declval
    template <typename T>
    typename vstl::add_rvalue_reference<T>::type declval() noexcept;

    //tuple
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

    //pair
    template <typename T, typename B>
    struct pair
    {
        typedef T           first_arg;
        typedef B           second_arg;
        typedef pair<T, B>  self_type;

        first_arg first;
        second_arg second;
        
        template <typename F = T, typename S = B>
        pair() : first(), second() {};
        pair(const first_arg& f, const second_arg& s) : first(f), second(s) {};
        self_type& operator=(const self_type& p) { first = p.first; second = p.second; return *this; };
        bool operator==(const self_type& p) { return (first == p.first && second = p.second) ? true : false; };
        bool operator!=(const self_type& p) { return *this == p; };
    };

    template <typename T, typename B>
    vstl::pair<T, B> make_pair(T f, B s)
    {
        return pair<T, B>(f, s);  
    };
};  

#endif