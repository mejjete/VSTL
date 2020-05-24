#ifndef VSTL_FUNCTIONALITY
    #define VSTL_FUNCTIONALITY
#include "binder.hpp"
namespace vstl
{
    //Binary predicate functions

    template <typename T>
    class plus {};                      //arg1 + arg2

    template <typename T>
    class minus {};                     //arg1 - arg2

    template <typename T>
    class multiplies {};                //arg1 * arg2

    template <typename T>
    class devides {};                   //arg1 / arg2

    template <typename T>               
    class modulus {};                   //arg1 % arg2

    template <typename T>
    class equal_to {};                  //arg1 == arg2

    template <typename T>
    class not_equal_to {};              //arg1 != arg2

    template <typename T>
    class greater {};                   //arg1 > arg2

    template <typename T>
    class less {};                      //arg1 < arg2

    template <typename T>
    class greate_equal {};              //arg1 >= arg2

    template <typename T>
    class less_equal {};                //arg1 <= arg2

    template <typename T>
    class logical_and {};               //arg1 && arg2

    template <typename T>
    class logical_or {};                //arg1 || arg2 
    
    template <typename T>
    class not2 {};                      //!(UnaryPredicate(arg1, arg2))

    //Unary predicate functions

    template <typename T>
    class logical_not {};              //!arg

    template <typename T>
    class not1 {};                      //!(UnaryPredicate(arg))

    template <typename T>
    class negate {};                    //-arg
}
#endif