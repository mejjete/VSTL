#ifndef VSTL_PTR_TRAITS
    #define VSTL_PTR_TRAITS

#include <vtype_traits.hpp>
#include <vdetail/type_traits/libc_traits.hpp>

namespace vstl
{
    /*
     *  The reason why pointer traits is not a struct 
     *  is the inability to chech whether the Ptr have 
     *  predefined typedef's. It makes pointer_traits much more 
     *  stable and useful. If the typedef's does not exist, then 
     *  all non-existent typedef's inside the pointer_traits will be 
     *  __undefined as well
    */
    template <typename Ptr>
    class pointer_traits
    {
        private:
            template <typename T, typename = __void_t<>>
            struct __eltype
            {
                typedef __undefined type;
            };

            template <typename T>
            struct __eltype<T, vstl::__void_t<typename T::element_type*>>
            {
                typedef typename T::element_type type;
            };

            template <typename T, typename = __void_t<>>
            struct __difftype
            {
                typedef std::ptrdiff_t type;
            };

            template <typename T>
            struct __difftype<T, vstl::__void_t<typename T::difference_type*>>
            {
                typedef typename T::difference_type type;
            };

            template <typename T, typename = __void_t<>>
            struct __rebind
            {
                typedef __undefined type;
            };

            template <typename T>
            struct __rebind<T, __void_t<typename T::rebing>>
            {
                typedef typename T::rebind type;
            };


        public:
            using pointer = Ptr;
            using element_type = typename __eltype<Ptr>::type;
            using difference_type = typename __difftype<Ptr>::type;

            template <typename T>
            using rebing = typename __rebind<T>::type;

            static pointer pointer_to(element_type &r)
            {
                return static_cast<pointer>(r);
            };
    };

    template <typename T>
    struct pointer_traits<T*>
    {
        public:
            using pointer =         T*;
            using element_type =    T;
            using difference_type = std::ptrdiff_t; 

            template <typename U>
            using rebing = U*;

            static pointer pointer_to(element_type &r)
            {
                return static_cast<pointer>(r);
            };
    };
};

#endif