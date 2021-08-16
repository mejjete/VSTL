#ifndef VSTL_ALLOCATOR_TRAITS
    #define VSTL_ALLOCATOR_TRAITS


#include <vstl/vdetail/memory/vstl_ptr_traits.hpp>
#include <vstl/vdetail/type_traits/vstl_make_unsigned.hpp>


namespace vstl
{

    template <typename Alloc>
    struct alloc_traits_base
    {
        private:
            template <typename T, typename = __void_t<>>
            struct __ptr 
            {
                typedef typename T::value_type* type;
            };

            template <typename T>
            struct __ptr<T, __void_t<typename T::pointer*>>
            {
                typedef typename T::pointer type;
            };

        public: 
            typedef Alloc                           all_t;
            typedef typename Alloc::value_type      val_t;
            typedef typename __ptr<Alloc>::type     ptr_t;
    };


    template <typename Alloc>
    struct allocator_traits : public alloc_traits_base<Alloc>
    {
        typedef alloc_traits_base<Alloc> allb;

        typedef typename allb::all_t    allocator_type;
        typedef typename allb::val_t    value_type;
        typedef typename allb::ptr_t    pointer;

        private:
            template <typename T, typename = __void_t<>>
            struct cptr
            {
                typedef typename std::pointer_traits<pointer>::
                template rebind<const value_type> type;             /* template keyword idicate compiler that rebind is a tempalate */
            };

            template <typename T>
            struct cptr<T, __void_t<typename T::const_pointer>>
            {
                typedef typename T::const_pointer type;
            };

            template <typename T, typename = __void_t<>>
            struct vptr 
            {
                typedef typename std::pointer_traits<pointer>::
                template rebind<const value_type> type;
            };

            template <typename T>
            struct vptr<T, __void_t<typename T::void_pointer*>>
            {
                typedef typename T::void_pointer type;
            };

            template <typename T, typename = __void_t<>>
            struct cvptr 
            {
                typedef typename std::pointer_traits<pointer>::
                template rebind<const void> type;
            };

            template <typename T>
            struct cvptr<T, __void_t<typename T::const_void_pointer*>>
            {
                typedef const typename T::const_void_pointer type;
            };

            template <typename T, typename = __void_t<>>
            struct dftype
            {
                typedef typename std::pointer_traits<
                    pointer>::difference_type type;
            };

            template <typename T>
            struct dftype<T, __void_t<typename T::difference_type*>>
            {
                typedef typename T::difference_type type;
            };

            template <typename T, typename = __void_t<>>
            struct stype 
            {
                typedef typename vstl::make_unsigned<
                    typename dftype<T>::type>::type type;
            };

            template <typename T>
            struct stype<T, __void_t<typename T::size_type*>>
            {
                typedef typename T::size_type type;
            };

            template <typename T, typename = __void_t<>>
            struct possa 
            {
                typedef vstl::false_type type;
            };

            template <typename T>
            struct possa<T, __void_t<typename T::propagate_on_container_copy_assignment*>>
            {
                typedef typename T::propagate_on_container_copy_assignment type;
            };

            template <typename T, typename = __void_t<>>
            struct posma 
            {
                typedef vstl::false_type type;
            };

            template <typename T>
            struct posma<T, __void_t<typename T::propagate_on_container_move_assignment*>>
            {
                typedef typename T::propagate_on_container_move_assignment type;
            };


            template <typename T, typename = __void_t<>>
            struct pocs 
            {
                typedef vstl::false_type type;
            };

            template <typename T>
            struct pocs<T, __void_t<typename T::propagate_on_container_swap*>>
            {
                typedef typename T::propagate_on_container_swap type;
            };

            template <typename T, typename = __void_t<>>
            struct isa 
            {
                typedef typename vstl::is_empty<T>::type type;
            };

            template <typename T>
            struct isa<T, __void_t<typename T::is_always_equal*>>
            {
                typedef typename T::is_always_equal type;
            };


        public:
            typedef typename cptr<Alloc>::type      const_pointer;
            typedef typename vptr<Alloc>::type      void_poiner;
            typedef typename cvptr<Alloc>::type     const_void_pointer;
            typedef typename dftype<Alloc>::type    difference_type;
            typedef typename stype<Alloc>::type     size_type;

            typedef typename possa<Alloc>::type     propagate_on_container_copy_assignment;
            typedef typename posma<Alloc>::type     propagate_on_container_move_assignment;
            typedef typename pocs<Alloc>::type      propagate_on_container_swap;
            typedef typename isa<Alloc>::type       is_always_equal;
    };
};

#endif 