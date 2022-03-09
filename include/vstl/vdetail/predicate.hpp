#ifndef VSTL_PREDICATE_H
#define VSTL_PREDICATE_H


#include <vstl/iterator.hpp>
#include <vstl/vdetail/interface.hpp>


namespace vstl
{

    template <typename T>
    struct plus : public binary_function<T, T, T>
    {
        T operator()(T& arg1, T& arg2) const { return arg1 + arg2; };
    };

    template <typename T>
    struct minus : public binary_function<T, T, T>
    {
        T operator()(const T& arg1, const T& arg2) const { return arg1 - arg2; };
    };

    template <typename T>
    struct multiplies : public binary_function<T, T, T> 
    {
        T operator()(const T& arg1, const T& arg2) const { return arg1 - arg2; };
    };

    template <typename T>
    struct devides : public binary_function<T, T, T>
    {
        T operator()(const T& arg1, const T& arg2) const { return arg1 / arg2; };
    };             

    template <typename T>               
    struct modulus : public binary_function<T, T, T>
    {
        T operator()(const T& arg1, const T& arg2) const { return arg1 % arg2; };
    };                 

    template <typename T>
    struct equal_to : public binary_function<T, T, bool>
    {
        bool operator()(const T& arg1, const T& arg2) const { return arg1 == arg2; };
    };     

    template <typename T>
    struct not_equal_to : public binary_function<T, T, bool>  
    {
        bool operator()(const T& arg1, const T& arg2) const { return arg1 != arg2; };
    };             

    template <typename T>
    struct greater : public binary_function<T, T, bool> 
    {
        bool operator()(const T& arg1, const T& arg2) const { return arg1 > arg2; };
    };                  

    template <typename T>
    struct less : public binary_function<T, T, bool>
    {
        bool operator()(const T& arg1, const T& arg2) const { return arg1 < arg2; };
    };

    template <typename _Tp, typename _Up>
    struct __less_aux : public binary_function<_Tp, _Up, bool>
    {
        bool operator()(const _Tp& arg1, const _Up& arg2) const { return arg1 < arg2; };
    };

    template <typename T>
    struct greate_equal : public binary_function<T, T, bool>
    {
        bool operator()(const T& arg1, const T& arg2) const { return arg1 >= arg2; };
    };

    template <typename T>
    struct less_equal : public binary_function<T, T, bool>
    {
        bool operator()(const T& arg1, const T& arg2) const { return arg1 <= arg2; };
    };

    template <typename T>
    struct logical_and : public binary_function<T, T, bool>
    {
        bool operator()(const T& arg1, const T& arg2) const { return arg1 && arg2; };
    };              

    template <typename T>
    struct logical_or : public binary_function<T, T, bool>
    {
        bool operator()(const T& arg1, const T& arg2) const { return arg1 || arg2; };
    };               
    
    template <typename T>
    struct not2 {};                      //!(UnaryPredicate(arg1, arg2))

    //Unary predicate functions

    template <typename T>
    class logical_not : public unary_function<T, bool>
    {
        bool operator()(T& arg) const { return !arg; };
    };

    template <typename T>
    class not1 {};                      //!(UnaryPredicate(arg))

    template <typename T>
    class negate {};
}
#endif