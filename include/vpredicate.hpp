#ifndef VSTL_PREDICATE
    #define VSTL_PREDICATE
#include "vinterface.hpp"
namespace vstl
{
    namespace pred
    {
        template <typename T>
        class LessThanComparable {};            //A class that has a less-than operator<.
        
        template <typename T>
        class Assignable {};                    //A class that has an operator() for its own type

        template <typename T>
        class EqualityComparable {};            //A class that has an equalance operator== for its own type
    };

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