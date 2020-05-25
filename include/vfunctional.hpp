#ifndef VSTL_FUNCTIONALITY
    #define VSTL_FUNCTIONALITY
#include "vbinder.hpp"
#include "vpredicate.hpp"
#include "vinterface.hpp"
#include <memory>
#include <cstdlib>
#include <cassert>
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
    
    template <typename T>
    class function;

    template <typename ReturnValue, typename ...Args>
    class function<ReturnValue(Args...)>
    {
        public:
            template <typename T>
            function& operator=(T t)
            {
                callable_ = std::make_unique<CallableT<T>>(t);
                return *this;
            };
            ReturnValue operator()(Args... argc) const 
            {
                assert(callable_);
                return callable_->Invoke(argc...);
            };
        private:
            class iCallable
            {
                public:
                    virtual ~iCallable() = default;
                    virtual ReturnValue Invoke(Args...) = 0;
            };

            template <typename T>
            class CallableT : public iCallable
            {
                public:
                    CallableT(const T& t): t_(t) {};
                    virtual ~CallableT() override = default;
                    ReturnValue Invoke(Args... args) override { return t_(args...); };
                private:
                    T t_;
            };
            std::unique_ptr<iCallable> callable_;
    };
}
#endif