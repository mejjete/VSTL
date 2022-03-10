/* 
 *  Standard Vector Class
 *
 *  Prefixes:
 *  _A_     stands for alias, like typedef or using declaration
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
#include <vstl/vdetail/tempimpl/vector.inl>


#ifndef __DEF_VSTL_VECTOR_SIZE
#define __DEF_VSTL_VECTOR_SIZE 15
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
                __rhs._M_nothrow_copy_data(__temp);
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

        const _A_alloc_type& _M_get_allocator() const { return this->__I_vimpl; };

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

        void _M_increase_space(_A_size_type __mntsize = _A_size_type());
        void _M_decrease_space(_A_size_type);

        const _A_size_type _M_size() const noexcept { return __I_vimpl.__I_finish - __I_vimpl.__I_start; };
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
            using _Base::_M_increase_space;
            using _Base::_M_decrease_space;

            vector() = default;
            ~vector();

            explicit vector(const _Alloc&);
            explicit vector(size_type, const _Alloc& __alloc = _Alloc());
            vector(size_type, const _Tp&, const _Alloc& __alloc = _Alloc());

            vector(const vector& vect);
            vector(vector&& vect) = default;
            vector(std::initializer_list<value_type>, const _Alloc& __alloc = _Alloc());            

            template <typename _InputIter, typename = _RequireInputIter<_InputIter>>
            vector(_InputIter, _InputIter, const _Alloc& __alloc = _Alloc());

            void push_back(const value_type&);
            void push_back(value_type&&);

            iterator begin()    { return iterator(__I_vimpl.__I_start); };
            iterator end()      { return iterator(__I_vimpl.__I_finish); };
            const_iterator cbegin() const   { return const_iterator(__I_vimpl.__I_start); };
            const_iterator cend() const     { return const_iterator(__I_vimpl.__I_finish); };

            reverse_iterator rbegin()   { return reverse_iterator(__I_vimpl.__I_finish); };
            reverse_iterator rend()     { return reverse_iterator(__I_vimpl.__I_start); };
            reverse_iterator crbegin() const { return const_reverse_iterator(__I_vimpl.__I_finish); };
            reverse_iterator crend() const { return const_reverse_iterator(__I_vimpl.__I_start); };

            iterator insert(iterator, const value_type&);
            iterator insert(iterator, size_type, const value_type&);

            template <typename _InputIter, typename = vstl::_RequireInputIter<_InputIter>>
            iterator insert(iterator, _InputIter, _InputIter);

            iterator insert(iterator, std::initializer_list<value_type>);

            template <typename... _Args>
            iterator emplace(iterator, _Args&&...);

            template <typename... _Args>
            void emplace_back(_Args&&...);

            void assign(size_type, const value_type&);
            void assign(std::initializer_list<value_type>);

            template <typename _InputIter, typename = vstl::_RequireInputIter<_InputIter>>
            void assign(_InputIter, _InputIter);

            size_type capacity() const noexcept { return __I_vimpl.__I_end - __I_vimpl.__I_start; };
            size_type size() const noexcept { return __I_vimpl.__I_finish - __I_vimpl.__I_start; };

            vector& operator=(const vector&);
            vector& operator=(vector&&);
            vector& operator=(std::initializer_list<value_type>);
            
            typename _Base::_A_alloc_type get_allocator() const noexcept { return _M_get_allocator(); };

            reference at(size_type);
            const_reference at(size_type) const;

            reference operator[](size_type __pos) { return *(__I_vimpl.__I_start + __pos); };
            const_reference operator[](size_type __pos) const { return const_cast<const_reference>(const_cast<const vector&>(*this).operator[](__pos)); };

            reference front() noexcept { return *__I_vimpl.__I_start; };
            const_reference front() const noexcept { return *__I_vimpl.__I_start; };

            reference back() noexcept { return --(*__I_vimpl.__I_finish); };
            const_reference back() const noexcept { return --(*__I_vimpl.__I_finish); };

            pointer data() noexcept { return __I_vimpl.__I_start; };
            const pointer data() const noexcept { return __I_vimpl.__I_start; };

            bool empty() const noexcept { return __I_vimpl.__I_start == __I_vimpl.__I_finish; };
          
            size_type max_size() const noexcept { return __max_seq_size(get_allocator()); };

            void reserve(size_type);

            void shrink_to_fit();

            void clear() noexcept;

            void erase(const_iterator, const_iterator);
            void erase(const_iterator);

            void swap(vector& other) { other.__I_vimpl._M_nothrow_swap_data(this->__I_vimpl); };

            void resize(size_type __count);
            void resize(size_type __count, const value_type& __val);
        
        private:
            bool __aux_resize(size_type);
    };



    /**
     * @brief Constructs a vector with allocator parameter
     * 
     * @param __alloc - allocator 
     */
    template <typename _Tp, typename _Alloc>
    vector<_Tp, _Alloc>::vector(const _Alloc& __alloc)
        : _Base(_M_get_allocator()) {};
    


    /**
     * @brief Default constructs a vector with given size
     * 
     * @param __sz - vector size
     * @param __alloc - optional allocator parameter
     */
    template <typename _Tp, typename _Alloc>
    vector<_Tp, _Alloc>::vector(size_type __sz, const _Alloc& __alloc)
        : _Base(__check_seq_size(__sz, __alloc), __alloc) 
    {
        __I_vimpl.__I_finish = __init_with_default_value_a(__I_vimpl.__I_start, __sz, _M_get_allocator());
    };



    /**
     * @brief Copy constructs vector from __vect source
     * 
     * @param __vect source vector 
     * @param __alloc optional allocator argument
     */
    template <typename _Tp, typename _Alloc>
    vector<_Tp, _Alloc>::vector(const vector& __vect)
        : _Base(__vect.capacity(), __vect._M_get_allocator())
    {
        __I_vimpl.__I_finish = __init_with_range_a(__I_vimpl.__I_start, __vect.cbegin(), __vect.size(), _M_get_allocator());
    };



    /**
     * @brief Constructs vector with __sz copies of __val instance
     * 
     * @param __sz count of elements to be inserted 
     * @param __val instance of element to be inserted
     * @param __alloc optional allocator argument
     */
    template <typename _Tp, typename _Alloc>
    vector<_Tp, _Alloc>::vector(size_type __sz, const _Tp& __val, const _Alloc& __alloc) 
        : _Base(__check_seq_size(__sz, __alloc), __alloc)
    {
        __I_vimpl.__I_finish = __init_with_value_a(__I_vimpl.__I_start, __sz, __val, __alloc);
    };



    /**
     * @brief Constructs vector with [__fiter, __biter) range
     * 
     * @param __fiter start of source range
     * @param __biter end of source range
     * @param __alloc optional allocator argument
     */
    template <typename _Tp, typename _Alloc>
    template <typename _InputIter, typename>
    vector<_Tp, _Alloc>::vector(_InputIter __fiter, _InputIter __biter, const _Alloc& __alloc)
        : _Base(__check_seq_size(vstl::distance(__fiter, __biter), __alloc), __alloc)
    {
        typedef typename vstl::iterator_traits<_InputIter>::difference_type __difftype;
        
        __difftype __diff = distance(__fiter, __biter);
        __I_vimpl.__I_finish = __init_with_range_a(__I_vimpl.__I_start, __fiter, __diff, __I_vimpl);
    };



    /**
     * @brief Constructs vector with initializer_list
     * 
     * @param __ilist source initalizer_list
     * @param __alloc optional allocator argument
     */
    template <typename _Tp, typename _Alloc>
    vector<_Tp, _Alloc>::vector(std::initializer_list<_Tp> __ilist, const _Alloc& __alloc) 
        : _Base(__check_seq_size(__ilist.size(), __alloc), __alloc)
    {
        __I_vimpl.__I_finish = __init_with_range_a(__I_vimpl.__I_start, __ilist.begin(), __ilist.size(), __I_vimpl);
    };



    /**
     * @brief Destroys the vector object
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
            __I_vimpl.__I_finish = __init_with_value_a(__I_vimpl.__I_finish, 1, __lhs, _M_get_allocator());
        else 
        {
            _M_increase_space();
            push_back(__lhs);
        }
    };



    /**
     * @brief Appends given element to the end of the container.

     * @param __rhs item to be added
     * Same as push_back(const value_type&) but implies on the move semantic
     */
    template <typename _Tp, typename _Alloc>
    void vector<_Tp, _Alloc>::push_back(value_type&& __rhs)
    {
        if(__I_vimpl.__I_finish != __I_vimpl.__I_end)
            __I_vimpl.__I_finish = __init_with_value_a(__I_vimpl.__I_finish, 1, vstl::move(__rhs), _M_get_allocator());
        else 
        {
            _M_increase_space();
            push_back(vstl::move(__rhs));
        }
    };



    /**
     * @brief Inserts __sz copies of __val before postion specified by __iter
     * 
     * @param __iter hint where to insert new elements 
     * @param __sz copies of element __val to insert
     * @param __val the element to be inserted  
     * 
     * @return vector<_Tp, _Alloc>::iterator
     * If insertion is successfull, returns iterator of the first inserted element 
     */
    template <typename _Tp, typename _Alloc>
    typename vector<_Tp, _Alloc>::iterator vector<_Tp, _Alloc>::insert(iterator __iter, size_type __sz, const value_type& __val)
    {   
        // save the iterator 'offset' relative to begin() because insertion might yeild reallocation
        difference_type __start_offset = __iter - this->cbegin();
        size_type __free_sz = __I_vimpl.__I_end - __I_vimpl.__I_finish; 
        size_type __to_move = (__I_vimpl.__I_finish - __iter.base());

        typedef reverse_iterator _ri;
        typedef const_reverse_iterator _cri;

        if(__free_sz > __sz)
        {
            __nothrow_move_or_copy_with_range_a(_ri(__I_vimpl.__I_finish + __sz), _ri(__I_vimpl.__I_finish), __to_move, _M_get_allocator());
            __init_with_value_a(__I_vimpl.__I_start + __start_offset, __sz, __val, _M_get_allocator());
            __I_vimpl.__I_finish += __sz;
        }
        else 
        {
            /**
             * When additional space required, insertion operation takes next steps:
             * 1. allocate new storage
             * 2. move old elements in range [start, iter) into newly allocated storage
             * 3. construct new elements
             * 4. move remaining elements within the range [iter, finish) into new storage
             * finish need not be included into final range because it points to the end, thus
             * on an invalid object
             */

            size_type __try_capacity = (this->capacity() * 3) / 2; 
            _Base __new_storage(__try_capacity < __sz ? __sz : __try_capacity, _M_get_allocator());

            __new_storage.__I_vimpl.__I_finish = __nothrow_move_or_copy_with_range_a(__new_storage.__I_vimpl.__I_start, 
                __I_vimpl.__I_start, __start_offset, _M_get_allocator());
            
            __new_storage.__I_vimpl.__I_finish = __init_with_value_a(__new_storage.__I_vimpl.__I_start + __start_offset, 
                __sz, __val, _M_get_allocator());
            
            __new_storage.__I_vimpl.__I_finish = __nothrow_move_or_copy_with_range_a(__new_storage.__I_vimpl.__I_start + __start_offset + __sz, 
                __iter.base(), __to_move, _M_get_allocator());
            
            this->__I_vimpl._M_nothrow_swap_data(__new_storage.__I_vimpl);
            
            // now __new_storage is an old pointer, we should null it out to avoid unnecessary destructor call
            _Destroy_a(__new_storage.__I_vimpl.__I_start, __new_storage.__I_vimpl.__I_finish, _M_get_allocator());
            __new_storage.__I_vimpl.__I_finish = __new_storage.__I_vimpl.__I_start;
        }

        return __I_vimpl.__I_start + __start_offset + __sz;
    };



    /**
     * @brief Inserts value before __iter
     * 
     * @param __iter hint where to insert new element
     * @param __val element to insert
     * 
     * @return vector<_Tp, _Alloc>::iterator 
     * If insertion is successfull, returns iterator of the first inserted element
     */
    template <typename _Tp, typename _Alloc>
    inline typename vector<_Tp, _Alloc>::iterator vector<_Tp, _Alloc>::insert(iterator __iter, const value_type& __val)
    {
        return this->insert(__iter, 1, __val);
    };



    /**
     * @brief Inserts all elements of initalizer list into vector before __iter position
     * 
     * @param __iter hint where to insert new elements 
     * @param __ulist source initialier_list
     * @return vector<_Tp, _Alloc>::iterator 
     * If insertion is successfull, returns iterator of the first inserted element 
     */
    template <typename _Tp, typename _Alloc>
    inline typename vector<_Tp, _Alloc>::iterator vector<_Tp, _Alloc>::insert(iterator __iter, std::initializer_list<_Tp> __ulist)
    {
        return this->insert(__iter, __ulist.begin(), __ulist.end());
    };



    /**
     * @brief Inserts range specified by [__first, __last) before __iter position
     * 
     * @param __iter hint where to insert new elements
     * @param __first source range start
     * @param __last source range end
     * 
     * @return vector<_Tp, _Alloc>::iterator
     * If insertion is successfull, returns iterator of the first inserted element.
     * Because of this function implemented in terms of another insert function,
     * thus, does not participate in memory management itself, it can yeild performance penalty
     */
    template <typename _Tp, typename _Alloc>
    template <typename _InputIter, typename>
    typename vector<_Tp, _Alloc>::iterator vector<_Tp, _Alloc>::insert(iterator __iter, _InputIter __first, _InputIter __last)
    {
        iterator __it;
        difference_type __iter_offset = __iter - this->begin();

        while(__first != __last)
        {
            /**
             * We evaluate hint to insert relative to the current begin
             * since each call to insert, potentionally, can cause memory 
             * reallocation and iterator invalidation
             */
            __it = this->insert(this->begin() + __iter_offset, 1, *__first);
            __iter_offset++;
            __first++;
        }

        return __it -= __last - __first;
    };



    /**
     * @brief Inserts a new element directly before pos
     * 
     * @param __iter hint where to insert newly constructed element 
     * @param __args arguments to constructor
     * 
     * @return vector<_Tp, _Alloc>::iterator 
     * If insertion is successfull, returns iterator of the first inserted element.
     */
    template <typename _Tp, typename _Alloc>
    template <typename... _Args>
    typename vector<_Tp, _Alloc>::iterator vector<_Tp, _Alloc>::emplace(iterator __iter, _Args&&... __args)
    {
        difference_type __start_offset = __iter - this->cbegin();
        size_type __free_sz = __I_vimpl.__I_end - __I_vimpl.__I_finish;

        if(__iter == this->end() && __free_sz > 0)
        {
            _M_get_allocator().construct(__I_vimpl.__I_finish, vstl::forward<_Args>(__args)...);
            ++__I_vimpl.__I_finish;
            return __I_vimpl.__I_finish - 1;
        }

        return this->insert(__iter, value_type(vstl::forward<_Args>(__args)...));
    };



    /**
     * @brief Inserts new element directly at the end 
     * 
     * @param __args constructor arguments
     */
    template <typename _Tp, typename _Alloc>
    template <typename... _Args>
    inline void vector<_Tp, _Alloc>::emplace_back(_Args&&... __args)
    {
        this->emplace(this->end(), __args...);
    };



    /**
     * @brief Assigns new values to the vector
     * 
     * @param __sz count of elements to be assigned 
     * @param __val instance of element to be assigned
     * Invalidates all interators
     */
    template <typename _Tp, typename _Alloc>
    inline void vector<_Tp, _Alloc>::assign(size_type __sz, const value_type& __val)
    {
        vector __temp(__sz, __val);
        __temp = vstl::move(*this);
    };



    /**
     * @brief Assigns new values from the range [__fiter, __biter)
     * 
     * @param __fiter start range
     * @param __biter end range
     */
    template <typename _Tp, typename _Alloc>
    template <typename _InputIter, typename>
    inline void vector<_Tp, _Alloc>::assign(_InputIter __fiter, _InputIter __biter)
    {
        vector __temp(__fiter, __biter);
        __temp = vstl::move(*this);
    };



    /**
     * @brief Assigns new values from initializer_list 
     * 
     * @param __ilist input initializer_list 
     */
    template <typename _Tp, typename _Alloc>
    inline void vector<_Tp, _Alloc>::assign(std::initializer_list<value_type> __ilist)
    {
        return assign(__ilist.begin(), __ilist.end());
    };



    /**
     * @brief Assigns new values to the vector
     * 
     * @param __vect source vector
     * @return vector<_Tp, _Alloc>& 
     * Invalidates all iterators
     */
    template <typename _Tp, typename _Alloc>
    inline vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(vector&& __vect)
    {
       this->__I_vimpl._M_nothrow_swap_data(__vect.__I_vimpl);
       return *this;
    };



    /**
     * @brief Assigns new values from the source vector 
     * 
     * @param __vect source vector
     * @return reference to itself
     * Invalidates all iterators 
     */
    template <typename _Tp, typename _Alloc>
    vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector& __vect)
    {
        /* Manually destroy current vector values */
        _Destroy_a(__I_vimpl.__I_start, __I_vimpl.__I_finish, _M_get_allocator());
        __I_vimpl.__I_start = __I_vimpl.__I_finish = typename _Base::_A_pointer();

        size_type __free_sz = __I_vimpl.__I_end - __I_vimpl.__I_start;
        if(__free_sz >= __vect.capacity())
            __I_vimpl.__I_finish = __init_with_range_a(__I_vimpl.__I_start, __vect.cbegin(), __vect.capacity(), _M_get_allocator());
        else 
        {
            /* Deallocate current memory */
            vector __to_destoy(vstl::move(*this));
            __to_destoy.~vector();

            vector __temp(__vect);
            *this = vstl::move(__temp);
        }

        return *this; 
    };



    /**
     * @brief Assigns new values from an initializer_list
     * 
     * @param __ilist input initializer_list 
     * @return reference to itself
     * Might invalidates all iterators
     */
    template <typename _Tp, typename _Alloc>
    inline vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(std::initializer_list<value_type> __ilist)
    {
        assign(__ilist.begin(), __ilist.end());
        return *this;
    };



    /**
     * @brief Returns the element at __pos position 
     * 
     * @param __pos position in vector
     * @return reference to element at __pos position
     */
    template <typename _Tp, typename _Alloc>
    inline typename vector<_Tp, _Alloc>::reference vector<_Tp, _Alloc>::at(size_type __pos)
    {
        return const_cast<reference>(const_cast<const vector&>(*this).at(__pos));
    };



    /**
     * @brief Returns the element at __pos position 
     * 
     * @param __pos position in vector
     * @return reference to element at __pos position
     */
    template <typename _Tp, typename _Alloc>
    inline typename vector<_Tp, _Alloc>::const_reference vector<_Tp, _Alloc>::at(size_type __pos) const
    {
        if(__pos >= this->size())
            throw std::out_of_range("vstl::vector::at - invalid offset");
        return *(__I_vimpl.__I_start + __pos);
    };



    /**
     * @brief Reserves storage 
     *  
     * @param __new_size size request
     */
    template <typename _Tp, typename _Alloc>
    void vector<_Tp, _Alloc>::reserve(size_type __new_size)
    {
        if(__new_size <= capacity())
            return;
        if(__new_size >= max_size())
            throw std::length_error("vstl::vector::reserve - invalid vector size");
        
        _M_increase_space(__new_size);
    };  



    /**
     * @brief Reduces an unused capacity 
     */
    template <typename _Tp, typename _Alloc>
    void vector<_Tp, _Alloc>::shrink_to_fit()
    {
        size_type __free_sz = __I_vimpl.__I_end - __I_vimpl.__I_finish;

        if(__free_sz == 0)
            return;
        
        _M_decrease_space(size());
    };



    /**
     * @brief Clears whole vector
     */
    template <typename _Tp, typename _Alloc>
    inline void vector<_Tp, _Alloc>::clear() noexcept
    {
        _Destroy_a(this->begin(), this->end(), _M_get_allocator());
    };



    /**
     * @brief Erases range from vector
     * 
     * @param __fiter start of range
     * @param __biter end of range
     */
    template <typename _Tp, typename _Alloc>
    inline void vector<_Tp, _Alloc>::erase(const_iterator __fiter, const_iterator __biter)
    {        
        iterator __start = begin() + (__fiter - cbegin());
        iterator __end = begin() + (__fiter - cbegin()) + (__biter - __fiter);
        _Destroy_a(__start, __end, _M_get_allocator());
        __nothrow_move_or_copy_with_range_a(__start, __end, end() - __biter, _M_get_allocator());
        __I_vimpl.__I_finish -= __biter - __fiter;
    };



    /**
     * @brief Erases one element from vector
     * 
     * @param __iter element to be erased 
     */
    template <typename _Tp, typename _Alloc>
    inline void vector<_Tp, _Alloc>::erase(const_iterator __iter)
    {        
        if(__iter == end())
            return;
        erase(__iter, next(__iter));
    };



    /*
     * Increases memory used by the vector corresponding to following algorithm:
     * if __memcnt == 0, reallocates memory according to the incore vector algorithm,
     * if __memcnt > current size, reallocates __memcnt memory and copy/move old elements,
     * if __memcnt <= current size, do nothing 
     * 
     * Provides strong exception guarantee 
     */
    template <typename _Tp, typename _Alloc>
    void _Vector_Base<_Tp, _Alloc>::_M_increase_space(_A_size_type __memcnt)
    {
        if(__memcnt <= _M_size() && __memcnt != 0)
            return;

        _A_pointer __old_start = __I_vimpl.__I_start;
        _A_pointer __old_end = __I_vimpl.__I_end;
        const _A_size_type __old_size = __old_end - __old_start;

        // return if no reallocation required
        if(__old_size == __memcnt && __memcnt != 0)
            return;

        _A_size_type __new_size = __check_seq_size(__memcnt, _M_get_allocator());
        
        if(__new_size == 0)
        {
            if(__old_size == 0)
                __new_size = __DEF_VSTL_VECTOR_SIZE;
            else 
                __new_size = __check_seq_size(((__old_size * 3) / 2), _M_get_allocator());
        }

        // creates new storage
        _Vector_Base __vbase(__new_size, _M_get_allocator());
        
        __vbase.__I_vimpl.__I_finish = __nothrow_move_or_copy_with_range_a(__vbase.__I_vimpl.__I_start, 
            __old_start, __old_size, _M_get_allocator());
        
        // swap current and new _Vector_Base to destruct old elements
        this->__I_vimpl._M_nothrow_swap_data(__vbase.__I_vimpl);
        _Destroy_a(__vbase.__I_vimpl.__I_start, __vbase.__I_vimpl.__I_finish, _M_get_allocator());
    };



    /**
     * Reduces memory used by the vector. The elements beyond the new size 
     * (e.g __memcnt) is forcibly destroyed.
     * 
     * Provides basic exception guarantee
     */
    template <typename _Tp, typename _Alloc>
    void _Vector_Base<_Tp, _Alloc>::_M_decrease_space(_A_size_type __memcnt)
    {
        _A_size_type __old_size = _M_size();
        if(__memcnt >= __old_size)
            return;
        
        _Vector_Base __vbase(__memcnt, _M_get_allocator());

        __vbase.__I_vimpl.__I_finish = __nothrow_move_or_copy_with_range_a(__vbase.__I_vimpl.__I_start, 
            __I_vimpl.__I_start, __memcnt, _M_get_allocator());
        
        // swap current and new _Vector_Base to destruct old elements
        this->__I_vimpl._M_nothrow_swap_data(__vbase.__I_vimpl);
        _Destroy_a(__vbase.__I_vimpl.__I_start, __vbase.__I_vimpl.__I_finish, _M_get_allocator());
    };



    /**
     * Aux function which reallocates new memory if so is required.
     * Returns true if reallocation has been performed
     */
    template <typename _Tp, typename _Alloc>
    inline bool vector<_Tp, _Alloc>::__aux_resize(size_type __memcnt)
    {
        if(__memcnt > capacity())
        {
            _M_increase_space(__memcnt);
            return true;
        }
        
        return false;
    };



    /**
     * @brief Resizes the vector to __count elements
     * If __count is greater than current size, rest elements
     * will be copy constructed with __val
     * If __count is less than current size, elements beyond the new capacity
     * will be destroyed
     * 
     * @param __count vector capacity to resize
     * @param __val additional values to be appended (if needed)
     */
    template <typename _Tp, typename _Alloc>
    void vector<_Tp, _Alloc>::resize(size_type __count, const value_type& __val)
    {
        size_type __old_size = size();

        if(__aux_resize(__count))   // reallocation has been performed if resize() need more memory
        {
            size_type __to_construct = capacity() - __old_size;
            __I_vimpl.__I_finish = __init_with_value_a(__I_vimpl.__I_finish, __to_construct, __val, _M_get_allocator());
        }
        else 
        {
            _Destroy_a(rbegin() + __count, rbegin(), _M_get_allocator());
            __I_vimpl.__I_finish -= __old_size - __count;
        }
    };



    /**
     * @brief Resizes the vector to __count elements
     * If __count is greater than current size, rest elements will be 
     * default constructed (if needed)
     * 
     * @param __count resize request
     */
    template <typename _Tp, typename _Alloc>
    void vector<_Tp, _Alloc>::resize(size_type __count)
    { 
        size_type __old_size = size();

        if(__aux_resize(__count))   // reallocation has been performed if resize() need more memory
        {
            size_type __to_construct = capacity() - __old_size;

            // default in-place construct new elements
            __I_vimpl.__I_finish = __emplace_with_value_a(__I_vimpl.__I_finish, __to_construct, _M_get_allocator());
        }
        else 
        {
            _Destroy_a(rbegin() + __count, rbegin(), _M_get_allocator());
            __I_vimpl.__I_finish -= __old_size - __count;
        }
    };



    template <typename _Tp, typename _Alloc>
    inline bool operator==(const vector<_Tp, _Alloc>& __v1, const vector<_Tp, _Alloc>& __v2)
    { 
        return (__v1.size() == __v2.size() && vstl::equal(__v1.cbegin(), __v1.cend(), __v2.cbegin()));
    };


    template <typename _Tp, typename _Alloc>
    inline bool operator!=(const vector<_Tp, _Alloc>& __v1, const vector<_Tp, _Alloc>& __v2)
    { 
        return !(__v1 == __v2);
    };


    template <typename _Tp, typename _Alloc>
    inline bool operator<(const vector<_Tp, _Alloc>& __v1, const vector<_Tp, _Alloc>& __v2)
    { 
        return vstl::lexicographical_compare(__v1.cbegin(), __v1.cend(), __v2.cbegin(), __v2.cend());
    };


    template <typename _Tp, typename _Alloc>
    inline bool operator>(const vector<_Tp, _Alloc>& __v1, const vector<_Tp, _Alloc>& __v2)
    { 
        return __v2 < __v1;
    };


    template <typename _Tp, typename _Alloc>
    inline bool operator<=(const vector<_Tp, _Alloc>& __v1, const vector<_Tp, _Alloc>& __v2)
    { 
        return !(__v1 > __v2);
    };


    template <typename _Tp, typename _Alloc>
    inline bool operator>=(const vector<_Tp, _Alloc>& __v1, const vector<_Tp, _Alloc>& __v2)
    { 
        return !(__v1 < __v2);
    };
}
#endif