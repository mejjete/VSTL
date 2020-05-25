#ifndef VSTL_FUNCTIONALITY
    #define VSTL_FUNCTIONALITY
#include "vbinder.hpp"
#include "vpredicate.hpp"
#include "vinterface.hpp"
namespace vstl
{
    //Binders
    template <class Operation, class T>
    binder1st<Operation> bind1st(const Operation& op, const T& x)
    {
        return binder1st<Operation>(op, typename Operation::first_argument_type(x));
    };

    template <class Operation, class T>
    binder2nd<Operation> bind2nd(const Operation& op, const T& x)
    {
        return binder2nd<Operation>(op, typename Operation::second_argument_type(x));
    };
}
#endif