#ifndef VSTL_UTILITY
    #define VSTL_UTILITY


#include <vstl/vdetail/type_traits/complite_traits.hpp>
#include <vstl/tuple.hpp>


namespace vstl
{
    template <typename T>
    typename vstl::add_rvalue_reference<T>::type declval() noexcept;

    template <typename T>
    constexpr typename vstl::remove_reference<T>::type&& move(T&& arg) noexcept
    {
        return static_cast<typename vstl::remove_reference<decltype(arg)>::type&&>(arg);
    };

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

    // template <typename T>
    // class tuple_size;

    // template <typename... T>
    // class tuple_size <vstl::tuple<T...>>
    //     : public vstl::integral_constant<std::size_t, sizeof(T...)>;

    // template <typename T>
    // class tuple_size<const T> : 
    //     public vstl::integral_constant<std::size_t, vstl::tuple_size<T>::value> {};

    // template <typename T>
    // class tuple_size<volatile T> : 
    //     public vstl::integral_constant<std::size_t, vstl::tuple_size<T>::value> {};

    // template <typename T>
    // class tuple_size<const volatile T> : 
    //     public vstl::integral_constant<std::size_t, vstl::tuple_size<T>::value> {};

    // template <typename T>
    // inline std::size_t tuple_size_v = tuple_size<T>::value;

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
        template <typename F = T, typename S = B>
        pair(F&& f, S&& s) : first(vstl::forward<F>(f)), second(vstl::forward<S>(s)) {};

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