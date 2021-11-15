/* 
 *  Standard Vector Class
 *
 *  Prefixes:
 *  _A_     stands for alias, like typedef or using 
 *  _I_     stands for item, like variable or similar
 *  _M_     stands for method    
*/
#ifndef VSTL_VECTOR_H
#define VSTL_VECTOR_H


#include <cstring>
#include <stdexcept>
#include <initializer_list>
#include <climits>


#include <vstl/iterator.hpp>
#include <vstl/algorithm.hpp>
#include <vstl/utility.hpp>
#include <vstl/memory.hpp>
#include <vstl/bits/vstl_initialize.hpp>
#include <vstl/bits/vstl_iterator.hpp>


#ifndef __DEF_VECT_SIZE
#define __DEF_VECT_SIZE 15
#endif 


namespace vstl
{
    /* 
     *  According to the standard, all memory allocations perform in the base part of the container to 
     *  simplify exception handling. This class only allocates/deallocates memory for the container.
     *  The actual construction takes place in the container itself
    */
    template <typename _Tp, typename _Alloc>
    struct _Vector_Base 
    {
        typedef typename _Alloc::template rebind<_Tp>::other 
            _A_alloc_type;

        typedef typename vstl::allocator_traits<_A_alloc_type>::allocator_type
            _A_allocator;

        typedef typename vstl::allocator_traits<_Alloc>::pointer
            _A_pointer; 

        typedef typename vstl::allocator_traits<_Alloc>::size_type
            _A_size_type;


        /* 
         *  STD-like implementation
         *  Contains actual pointers to the data. __I_start and __I_finish specify used chunk of data,
         *  __I_end specify end of the memory area.
        */
        struct _Vector_Impl_Data
        {
            _A_pointer  __I_start;
            _A_pointer  __I_finish;
            _A_pointer  __I_end;

            /* default constructor */
            _Vector_Impl_Data() : __I_start(), __I_finish(), __I_end() {}; 

            /* move constructor */
            _Vector_Impl_Data(_Vector_Impl_Data&& __rhs) noexcept : 
                __I_start(__rhs.__I_start), __I_finish(__rhs.__I_finish), __I_end(__rhs.__I_end) 
            {
                __rhs.__I_start = __rhs.__I_finish = __rhs.__I_end = _A_pointer();
            };

            /* shallow copy */
            void _M_nothrow_copy_data(const _Vector_Impl_Data& __rhs) noexcept
            {
                __I_start = __rhs.__I_start;
                __I_finish = __rhs.__I_finish;
                __I_end = __rhs.__I_end;
            };

            /* swap */
            void _M_nothrow_swap_data(_Vector_Impl_Data& __rhs) noexcept 
            {
                _Vector_Impl_Data __temp;
                __temp._M_nothrow_copy_data(*this);
                this->_M_nothrow_copy_data(__rhs);
                __rhs->_M_nothrow_copy_data(__temp);
            };
        };


        /**
         *  _Vector_Base proxy class which manage allocator/deallocation
         *  since it shares the allocator interface
         */
        struct _Vector_Impl : public _A_allocator, public _Vector_Impl_Data
        {
            _Vector_Impl() noexcept(vstl::is_nothrow_default_constructible<_A_allocator>::value)
            {};


            _Vector_Impl(const _A_allocator& __a) noexcept(vstl::is_nothrow_copy_constructible<_A_allocator>::value) 
                : _A_allocator(__a) {};


            _Vector_Impl(_A_allocator&& __arhs, _Vector_Impl&& __vrhs) noexcept(vstl::is_nothrow_move_constructible<_A_allocator>::value)
                : _A_allocator(vstl::move(__arhs)), _Vector_Impl_Data(vstl::move(__vrhs)) {};
        };

        _Vector_Impl __I_vimpl;

        _A_alloc_type& _M_get_allocator() { return this->__I_vimpl; };

        _Vector_Base() = default;

        _Vector_Base(size_t __n) : __I_vimpl()
        {
            _M_create_storage(__n);
        };

        _Vector_Base(size_t __n, const _A_alloc_type& __alloc) : __I_vimpl(__alloc)
        {
            _M_create_storage(__n);
        };

        _Vector_Base(_Vector_Base&& __rhs) = default;

        _Vector_Base(const _Alloc& __alloc) : __I_vimpl(__alloc) {};

        ~_Vector_Base()
        {
            _M_deallocate(__I_vimpl.__I_start, __I_vimpl.__I_end - __I_vimpl.__I_start);
        };

    protected:

        void _M_create_storage(size_t __n)
        {
            this->__I_vimpl.__I_start = _M_allocate(__n);
            this->__I_vimpl.__I_finish = this->__I_vimpl.__I_start;
            this->__I_vimpl.__I_end = this->__I_vimpl.__I_start + __n;
        };

        _A_pointer _M_allocate(size_t __n)
        {   
            return __n != 0 ? __I_vimpl.allocate(__n) : _A_pointer();
        };

        void _M_deallocate(_A_pointer __p, size_t __n)
        {
            if(__p)
                __I_vimpl.deallocate(__p, __n);
        };

        /**
         * @brief Reallocate the vector storage
         * 
         * Invalidates all iterator
         * 
         * It's not part of the vector class since the vector itself does not 
         * participate in the allocation/deallocation/reallocation
         */
        void _M_reallocate();
    };


    /**
     * @brief VSTL implementation of the std::vector 
     * 
     * @tparam _Tp - type of object to store
     * @tparam _Alloc - memory model
     */
    template <typename _Tp, typename _Alloc = vstl::allocator<_Tp>>
    class vector : protected vstl::_Vector_Base<_Tp, _Alloc>
    {
        private:

            /**
             *  Basic requirements on the type
             */
            static_assert(vstl::is_same<vstl::remove_cv_t<_Tp>, _Tp>::value, 
                "vstl::vector<T, Alloc>: T must be a non-const, non-volatile type");
            static_assert(vstl::is_move_constructible<_Tp>::value,
                "vstl::vector<T, Alloc>: T must be a move-constructible type");

        
            typedef _Vector_Base<_Tp, _Alloc>                        _Base;
            typedef vstl::allocator_traits<_Alloc>                   _Alloc_Traits;


            /**
             *  The vector class and all sequence containers which are part of the VSTL,
             *  implements an iterator as class rather than raw poiner. The reason for that 
             *  is awkward plain data type behavior, which implies that all fundamental 
             *  data types are not allowed to modify their temporary values. However, doing
             *  so is totally allowed on structures and classes
             */
            template <typename _Iterator>
            using _Iter = __vstl_cxx::__normal_iterator<_Iterator>;

            template <typename _Iterator>
            using _rIter = __vstl_cxx::__reverse_iterator<_Iterator>;


        public:         
            typedef _Tp                                                     value_type;
            typedef typename _Alloc_Traits::pointer                         pointer;
            typedef typename _Alloc_Traits::const_pointer                   const_pointer;
            typedef typename _Alloc_Traits::value_type&                     reference;
            typedef const typename _Alloc_Traits::value_type&               const_reference;
            typedef _Iter<pointer>                                          iterator;
            typedef _Iter<const_pointer>                                    const_iterator;
            typedef _rIter<pointer>                                         reverse_iterator;
            typedef _rIter<const_pointer>                                   const_reverse_iterator;
            typedef size_t                                                  size_type;
            typedef ptrdiff_t                                               difference_type;
            typedef _Alloc                                                  allocator_type;


            using _Base::_M_create_storage;
            using _Base::_M_get_allocator;
            using _Base::_M_allocate;
            using _Base::_M_deallocate;
            using _Base::__I_vimpl;
            using _Base::_M_reallocate;


            vector() = default;
            ~vector();


            vector(size_type, const _Alloc& __alloc = _Alloc());
            vector(size_type, const _Tp&, const _Alloc& __alloc = _Alloc());


            vector(const vector& vect);
            vector(vector&& vect);
            vector(std::initializer_list<_Tp>, const _Alloc& __alloc = _Alloc());
            

            template <typename _InputIter, typename = _RequireInputIter<_InputIter>>
            vector(_InputIter, _InputIter, const _Alloc& __alloc = _Alloc());


            void push_back(const value_type&);
            void push_back(value_type&&);


            iterator begin()    { return iterator(__I_vimpl.__I_start); };
            iterator end()      { return iterator(__I_vimpl.__I_finish); };
            reverse_iterator rbegin()   { return reverse_iterator(__I_vimpl.__I_finish); };
            reverse_iterator rend()     { return reverse_iterator(__I_vimpl.__I_start); };


            size_type capacity() const noexcept { return __I_vimpl.__I_end - __I_vimpl.__I_start; };
        private:
            void _M_realloc();
    };


    /**
     *  Create a vector with __sz elements and default initialze them
    */
    template <typename _Tp, typename _Alloc>
    vector<_Tp, _Alloc>::vector(size_type __sz, const _Alloc& __alloc)
        : _Base(__sz, __alloc)
    {
        __I_vimpl.__I_finish = __init_with_default_value_a(__I_vimpl.__I_start, __sz, __alloc);
    };


    /**
     *  Create vector with __sz size and initialize it with __sz copies 
     *  of the __val items
    */
    template <typename _Tp, typename _Alloc>
    vector<_Tp, _Alloc>::vector(size_type __sz, const _Tp& __val, const _Alloc& __alloc) 
        : _Base(__sz, __alloc)
    {
        __I_vimpl.__I_finish = __init_with_value_a(__I_vimpl.__I_start, __sz, __val, __alloc);
    };



    /**
     *  Create and initialize vector with [__fiter, __biter) range
    */
    template <typename _Tp, typename _Alloc>
    template <typename _InputIter, typename>
    vector<_Tp, _Alloc>::vector(_InputIter __fiter, _InputIter __biter, const _Alloc& __alloc)
        : _Base(vstl::alg::distance(__fiter, __biter), __alloc)
    {
        typedef typename vstl::iterator_traits<_InputIter>::difference_type __difftype;
        using vstl::alg::distance;
        
        __difftype __diff = distance(__fiter, __biter);
        __I_vimpl.__I_finish = __init_with_range_a(__I_vimpl.__I_start, __fiter, __diff, __I_vimpl);
    };



    /**
     *  Create and initialize vector with initializer_list
    */
    template <typename _Tp, typename _Alloc>
    vector<_Tp, _Alloc>::vector(std::initializer_list<_Tp> __ilist, const _Alloc& __alloc) 
        : _Base(__alloc)
    {
        __I_vimpl.__I_finish = __init_with_range_a(__I_vimpl.__I_start, __ilist.begin(), __ilist.size(), __I_vimpl);
    };



    /** 
     *  Destroy vector 
    */
    template <typename _Tp, typename _Alloc>
    vector<_Tp, _Alloc>::~vector()
    {
        _Destroy_a(this->__I_vimpl.__I_start, this->__I_vimpl.__I_finish, _M_get_allocator());
    };



    /**
     * @brief Appends given element to the end of the container 
     *  
     * @param __lhs item to be added
     */
    template <typename _Tp, typename _Alloc>
    void vector<_Tp, _Alloc>::push_back(const value_type& __lhs)
    {
        if(__I_vimpl.__I_finish != __I_vimpl.__I_end)
            __I_vimpl.__I_finish = __init_with_value(__I_vimpl.__I_finish, 1, __lhs);
        else 
        {
            _M_reallocate();
            push_back(__lhs);
        }
    };



    /**
     * @brief Same as push_back(const value_type&) but implies on the move semantic
     *   
     * @param __rhs Item to be added
     */
    template <typename _Tp, typename _Alloc>
    void vector<_Tp, _Alloc>::push_back(value_type&& __rhs)
    {
        if(__I_vimpl.__I_finish != __I_vimpl.__I_end)
            __I_vimpl.__I_finish = __init_with_value(__I_vimpl.__I_finish, 1, vstl::move(__rhs));
        else 
        {
            _M_reallocate();
            push_back(vstl::move(__rhs));
        }
    };



    /**
     * @brief Do basic reallocation
     * 
     * Allocate new space and copy-construct old elements
     * As a side-effect, it invalidates all iterators
     */
    template <typename _Tp, typename _Alloc>
    void _Vector_Base<_Tp, _Alloc>::_M_reallocate()
    {
        _A_pointer __old_start = __I_vimpl.__I_start;
        _A_pointer __old_end = __I_vimpl.__I_end;
        const _A_size_type __old_size = __old_end - __old_start;

        _A_size_type __new_size = ((__old_end - __old_start) * 3) / 2;
        if(__new_size == 0)
            __new_size = __DEF_VECT_SIZE;

        _A_pointer __new_start = _M_allocate(__new_size);
        _A_pointer __new_finish;
        _A_pointer __new_end = __new_start + __new_size;

        try 
        {  
            /* copy construct old elements */
            __new_finish = __init_with_range_a(__new_start, __old_start, __old_size, __I_vimpl);
        }
        catch(...)
        {
            _M_deallocate(__new_start, __new_finish - __new_start);
            throw;
        }

        __I_vimpl.__I_start = __new_start;
        __I_vimpl.__I_finish = __new_finish;
        __I_vimpl.__I_end = __new_end;
    };
}
#endif