#ifndef VSTL_FUNCTIONALITY
    #define VSTL_FUNCTIONALITY
#include "vbinder.hpp"
#include "vinterface.hpp"
namespace vstl
{
    //Binary predicate functions

    template <typename T>
    class plus : public binary_function<T, T, T>
    {
        T operator()(T& arg1, T& arg2) { return arg1 + arg2; };
    };

    template <typename T>
    class minus : public binary_function<T, T, T> {};                       

    template <typename T>
    class multiplies : binary_function<T, T, T> {};                         

    template <typename T>
    class devides : binary_function<T, T, T> {};                  

    template <typename T>               
    class modulus {};                   

    template <typename T>
    class equal_to {};                  

    template <typename T>
    class not_equal_to {};              

    template <typename T>
    class greater {};                   

    template <typename T>
    class less {};                      

    template <typename T>
    class greate_equal {};              

    template <typename T>
    class less_equal {};                

    template <typename T>
    class logical_and {};               

    template <typename T>
    class logical_or {};                
    
    template <typename T>
    class not2 {};                      //!(UnaryPredicate(arg1, arg2))

    //Unary predicate functions

    template <typename T>
    class logical_not : unary_function<T, bool>
    {
        bool operator()(T& arg) { return !arg; };
    };

    template <typename T>
    class not1 {};                      //!(UnaryPredicate(arg))

    template <typename T>
    class negate {};                   
}
#endif