#ifndef VSTL_UTILITY_CLASS
    #define VSTL_UTILITY_CLASS

#include <vdetail/type_traits/class_support_operations.hpp>
#include <vutility.hpp>
#include <vtype_traits.hpp>

namespace vstl
{
    template <typename T>
    typename vstl::conditional<!vstl::is_nothrow_move_constructible_v<T> && vstl::is_copy_constructible_v<T>, const T&, T&&>::type 
    move_if_noexcept(T& x) { return vstl::move(x); };
}

#endif