#ifndef VSTL_INTERFACE
    #define VSTL_INTERFACE

namespace vstl
{
    //Function object interface

    template <typename Arg1, typename Result>
    struct unary_function
    {
        typedef Arg1 first_argument_type;
        typedef Result result_type;
    };

    template <typename Arg1, typename Arg2, typename Result>
    struct binary_function : public unary_function<Arg1, Result>
    {
        typedef Arg2 second_argumen_type;
    };
}
#endif