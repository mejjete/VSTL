#ifndef VSTL_MEMORY
    #define VSTL_MEMORY


#include <vstl/vdetail/type_traits/primary_traits.hpp>
#include <vstl/vdetail/allocator.hpp>
#include <vstl/vdetail/memory/vstl_ptr_traits.hpp>
#include <vstl/vdetail/type_traits/libc_traits.hpp>


namespace vstl
{
    template <typename T>
    typename vstl::enable_if<vstl::is_object<T>::value, T*>::type
    addressof(T& arg) 
    { 
        return reinterpret_cast<T*>(
        &const_cast<char&>(reinterpret_cast<char&>(arg)));
    }   


    
    template <typename Alloc>
    struct allocator_traits
    {
        template <typename T, typename = void>
        struct cvalue
        {
            typedef const typename T::value_type type;
        };
        
        template <typename T>
        struct cvalue<T, vstl::__void_t<typename T::const_value_type*>>
        {
            typedef typename T::const_value_type type;
        };

        template <typename T, typename = void>
        struct ptr 
        {
            typedef typename T::value_type* type;
        };

        template <typename T>
        struct ptr<T, vstl::__void_t<typename T::pointer*>>
        {
            typedef typename T::pointer type;
        };

        template <typename T, typename = void>
        struct vptr 
        {
            typedef void* type;
        };

        template <typename T>
        struct vptr<T, vstl::__void_t<typename T::void_pointer*>>
        {
            typedef typename T::void_pointer type;
        };

        template <typename T, typename = void>
        struct cvptr 
        {
            typedef const void* type;
        };

        template <typename T>
        struct cvptr<T, vstl::__void_t<typename T::const_void_pointer*>>
        {
            typedef const typename T::const_void_pointer type;
        };

        template <typename T, typename = void>
        struct dftype
        {
            typedef ptrdiff_t type;
        };

        template <typename T>
        struct dftype<T, vstl::__void_t<typename T::difference_type*>>
        {
            typedef typename T::difference_type type;
        };

        template <typename T, typename = void>
        struct stype 
        {
            typedef size_t type;
        };

        template <typename T>
        struct stype<T, vstl::__void_t<typename T::size_type*>>
        {
            typedef typename T::size_type type;
        };

        typedef Alloc                           allocator_type;
        typedef typename Alloc::value_type      value_type;
        typedef typename cvalue<Alloc>::type    const_value_type;
        typedef typename ptr<Alloc>::type       pointer;
        typedef typename vptr<Alloc>::type      void_poiner;
        typedef typename cvptr<Alloc>::type     const_void_pointer;
        typedef typename dftype<Alloc>::type    difference_type;
        typedef typename stype<Alloc>::type     size_type;
        // typedef typename possa::value_type           propagate_on_container_copy_assignment;
        // typedef typename posma::value_type           propagate_on_container_move_assignment;
        // typedef typename pocs::value_type            propagate_on_container_swap;
        // typedef typename isa::value_type             is_always_equal;
    };

};

#endif