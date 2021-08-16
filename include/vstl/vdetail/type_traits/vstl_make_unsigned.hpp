#ifndef VSTL_MAKE_UNSIGNED
    #define VSTL_MAKE_UNSIGNED


#include <vstl/vdetail/type_traits/primary_traits.hpp>
#include <vstl/vdetail/type_traits/libc_traits.hpp>


namespace vstl
{
    namespace detail
    {
        template <typename T>
        struct __make_unsigned
        {
            typedef T type;
        };

        template <>
        struct __make_unsigned<char>
        {
            typedef unsigned char type;
        };

        template <>
        struct __make_unsigned<signed char>
        {
            typedef unsigned char type;
        };

        template <>
        struct __make_unsigned<short>
        {
            typedef unsigned short type;
        };

        template <>
        struct __make_unsigned<int>
        {
            typedef unsigned int type;
        };

        template <>
        struct __make_unsigned<long>
        {
            typedef unsigned int type;
        };

        template <>
        struct __make_unsigned<long long>
        {
            typedef unsigned long long type;
        };

        template <typename T>
        using mu = typename __make_unsigned<T>::type;
    };


    /*
     *  For a given type T, make unsigned T
     *  If VSTL_EXPERIMENTAL defined, derived type  
     *  could be __undefined
    */
    
    template <typename T>
    struct make_unsigned
    {
        private:    
            typedef typename vstl::remove_cv<T>::type   _t_no_cv ;

            static_assert(vstl::is_integral<_t_no_cv>::value,
                "make_unsigned<T>: T must be an integral type");
            
            static_assert(!vstl::is_same<_t_no_cv, bool>::value,
                "make_unsigned<T>: T must not be the bool type");


            typedef typename vstl::detail::__make_unsigned<_t_no_cv>::type   ttype;

            /* boost-like implementation */
            typedef typename vstl::conditional<
                vstl::is_unsigned<ttype>::value, 
                ttype, 
                //Not a built-in arithmetic type, get the nearest value 
                vstl::conditional_t<
                    sizeof(_t_no_cv) == sizeof(unsigned char),
                    unsigned char,
                    typename vstl::conditional<
                        sizeof(_t_no_cv) == sizeof(unsigned short),
                        unsigned short,
                        typename vstl::conditional<
                            sizeof(_t_no_cv) == sizeof(int), 
                            unsigned int, 
                            typename vstl::conditional<
                            sizeof(_t_no_cv) == sizeof(unsigned long),
                            
#ifndef VSTL_EXPERIMENTAL   
                                unsigned long, 
                                unsigned long long>
#elif
                                unsigned long,
                                typename vstl::conditional<
                                    sizeof(_t_no_cv) == sizeof(unsigned long long),
                                    unsigned long long,
                                    unsigned>::type>
#endif 

                            ::type>
                        ::type>
                    ::type>
                >::type __itype;

            /* add const qualifier back */
            typedef typename vstl::conditional<
                vstl::is_const<__itype>::value,
                typename vstl::add_const<__itype>::type,
                __itype>::type      const_itype;
            
            /* add volatile qualifier back */
            typedef typename vstl::conditional<
                vstl::is_volatile<T>::value,
                typename vstl::add_volatile<const_itype>::type,
                const_itype>::type  cv_itype;

#ifdef VSTL_EXPERIMENTAL
            static_assert(vstl::is_same_v<vstl::__undefined, __itype>,
                "make_unsigned<T>: T could not be unsigned");
#endif

        public:
            typedef cv_itype type; 
    };

    template <typename T>
    using make_unsigned_t = typename make_unsigned<T>::type;

};

#endif 