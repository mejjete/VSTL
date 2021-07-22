#ifndef VSTL_BINDER
    #define VSTL_BINDER


#include <vstl/vdetail/interface.hpp>


namespace vstl
{
    //bound first funciton parameter
    template <class Operation> 
    class binder1st : public unary_function <typename Operation::second_argument_type, typename Operation::result_type>
    {
        private:
            Operation op;
            typename Operation::first_argument_type value;
        public:
            binder1st (const Operation& x, const typename Operation::first_argument_type& y) : op(x), value(y) {};
            typename Operation::result_type
                operator() (const typename Operation::second_argument_type& x) const
                { 
                    return op(value, x); 
                };
    };

    //bound second function parameter
    template <typename Operation>
    class binder2nd : public unary_function<typename Operation::first_argument_type, typename Operation::result_type>
    {
        private:
            Operation op;
            typename Operation::second_argument_type value;
        public:
            binder2nd(const Operation& x, const typename Operation::second_argument_type& s) : op(x), value(s) {};
            typename Operation::result_type
                operator()(const typename Operation::first_argument_type& x) const 
                {
                    return op(x, value);
                };
    };
}
#endif