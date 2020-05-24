#ifndef VSTL_BINDER
    #define VSTL_BINDER
#include "vinterface.hpp"
namespace vstl
{
    template <typename Operation>
    class binder1st : public unary_function<Operation, binder1st<Operation>>
    {
        private:
            Operation op;
            typename Operation::first_argument_type value;
        public:
            binder1st(const Operation& x, const typename Operation::first_argument_type& y) : op(x), value(y) {};
            typename Operation::result_type operator() (const typename Operation::second_argument_type& x) const
            {
                return op(value, x);
            };
    };
}
#endif