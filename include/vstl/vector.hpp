/* 
 * Ve.......
 *
 *  Prefixes:
 *  _A_     stands for alias, like typedef or using 
 *  _I_     stands for item, like varible or similar
 *  _M_     stands for method    
*/
#ifndef VSTL_VECTOR
    #define VSTL_VECTOR


#include <cstring>
#include <stdexcept>
#include <initializer_list>
#include <climits>


#include <vstl/iterator.hpp>
#include <vstl/algorithm.hpp>
#include <vstl/utility.hpp>
#include <vstl/memory.hpp>


namespace vstl
{

    /* 
     *  According to the standard, all memory allocations perform in the base part of the container to 
     *  simplify exception handling. This class only allocates/deallocates memory for the container.
     *  The actual construction takes place in the container itself
    */
    template <typename T, typename Alloc>
    struct _Vector_Base 
    {
        typedef typename Alloc::template rebind<T>::other 
            _A_alloc_type;

        typedef typename vstl::allocator_traits<_A_alloc_type>::allocator_type
            _A_allocator;

        typedef typename vstl::allocator_traits<Alloc>::pointer
            _A_pointer; 


        /* 
         *  STD-like implementation
         *  Contains actual pointers to the data
         *  _I_start and _I_finish specify used chunk of data
         *  _I_end specify end of the memory area
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


        /*  */
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

        _A_pointer _M_allocate(size_t __n)
        {   
            return __n != 0 ? _A_allocator::allocate(__n) : _A_pointer();
        };

        void _M_deallocate(_A_pointer __p, size_t __n)
        {
            if(__p)
                __I_vimpl.deallocate(__p,__n);
        };

        _Vector_Base() = default;

        _Vector_Base(size_t __n) : __I_vimpl()
        {
            _M_create_storage(__n);
        };

        _Vector_Base(_Vector_Base&& __rhs) = default;

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
    };


    /* ********** */

    template <typename T, typename Allocator = vstl::allocator<T>>
    class vector : private vstl::_Vector_Base<T, Allocator>
    {
        /* Assertions */

        static_assert(vstl::is_same<vstl::remove_cv_t<T>, T>::value, 
            "vstl::vector<T, Alloc>: T must be a non-const, non-volatile type");
        static_assert(vstl::is_move_constructible<T>::value,
            "vstl::vector<T, Alloc>: T must be a move-constructible type");
        static_assert(vstl::is_same<typename vstl::allocator_traits<Allocator>::value_type, T>::value,
            "vstl:vector<T, Alloc>: T must be the same as Allocator value type");
        
        private:
            typedef _Vector_Base<T, Allocator>                      _Base;
            typedef vstl::allocator_traits<Alloc>                   _Alloc_Traits;

        public:         
            typedef T                                                       value_type;
            typedef typename _Alloc_Traits::pointer                         pointer;
            typedef typename _Alloc_Traits::const_pointer                   const_pointer;
            typedef typename _Alloc_Traits::reference                       reference;
            typedef typename _Alloc_Traits::const_reference                 const_reference;
            typedef typename __vstl_cxx::normal_iterator<pointer, vector>   iterator;
            typedef typename __vstl_cxx::normal_iterator<const_pointer, vector> const_iterator;
            typedef typename vstl::reverse_iterator<iterator>               reverse_iterator;
            typedef typename vstl::reverse_iterator<const_iterator>         const_reverse_iterator;
            typedef size_t                                                  size_type;
            typedef ptrdiff_t                                               difference_type;
            typedef Allocator                                               allocator_type;


            class iterator
            {
                public:
                    typedef iterator                                    self_type;
                    typedef T                                           value_type;
                    typedef T&                                          reference;
                    typedef T*                                          pointer;
                    typedef const T*                                    const_pointer;
                    typedef const T&                                    const_reference;
                    typedef int                                         difference_type;
                    typedef vstl::random_access_iterator_tag            iterator_category;
                    iterator() : m_vector(nullptr), m_index(0) {};
                    iterator(vstl::vector<T>* vect, int i = 0) : m_vector(vect), m_index(i) {};
                    iterator(const self_type& iter) : m_vector(iter.m_vector), m_index(iter.m_index) {};
                    self_type& operator=(const vstl::vector<T, Allocator>* vect) { m_vector = vect; m_index = vect.m_index; return *this; }; 
                    self_type& operator=(const vstl::vector<T, Allocator>::iterator& iter) { m_vector = iter.m_vector; m_index = iter.m_index; return *this; };
                    self_type operator++()          { return self_type(m_vector, ++m_index); };
                    self_type operator++(int)       { return operator++(); };
                    self_type operator--()          { return self_type(m_vector, --m_index); };
                    self_type operator--(int)       { return operator--(); };
                    reference operator*()           { return m_vector->operator[](m_index); };
                    value_type operator->()         { return m_vector->operator[](m_index); };
                    bool operator==(const self_type& rhs) { return m_index == rhs.m_index; };
                    bool operator!=(const self_type& rhs) { return m_index != rhs.m_index; };
                    const int position() { return m_index; };
                    friend self_type& operator+(self_type &lhs, int i) { lhs.m_index += i; return lhs; };
                    friend self_type& operator-(self_type &lhs, int i) { lhs.m_index -= i; return lhs; };
                private:
                    vstl::vector<T> *m_vector;
                    int m_index;
            };

            class const_iterator
            {
                public:
                    typedef const_iterator                              self_type;
                    typedef T                                           value_type;
                    typedef T&                                          reference;
                    typedef T*                                          pointer;
                    typedef const T*                                    const_pointer;
                    typedef const T&                                    const_reference;
                    typedef int                                         difference_type;
                    typedef vstl::random_access_iterator_tag            iterator_category;
                    const_iterator() : m_vector(nullptr), m_index(0) {};
                    const_iterator(vstl::vector<T, Allocator> *vect, int i = 0) : m_vector(vect), m_index(i) {};
                    const_iterator(const self_type& iter) : m_vector(iter.m_vector), m_index(iter.m_index) {};
                    self_type& operator=(const vstl::vector<T, Allocator>* vect) { m_vector = vect; m_index = vect.m_index; return *this; };  
                    self_type operator++()          { return self_type(m_vector, ++m_index); };
                    self_type operator++(int)       { return operator++(); };
                    self_type operator--()          { return self_type(m_vector, --m_index); };
                    self_type operator--(int)       { return operator--(); };
                    const_reference operator*()     { return m_vector->operator[](m_index); };
                    const value_type operator->()   { return m_vector->operator[](m_index); };
                    bool operator==(const self_type& rhs) { return ((m_vector == &rhs) && m_index == rhs.m_index); };
                    bool operator!=(const self_type& rhs) { return !operator==(rhs); };
                    friend self_type& operator+(self_type &lhs, int i) { lhs.m_index += i; return lhs; };
                    friend self_type& operator-(self_type &lhs, int i) { lhs.m_index -= i; return lhs; };
                private:
                    vstl::vector<T> *m_vector;
                    int m_index;
            };

            vector();
            vector(int a, T&& val);
            vector(const vector& vect);
            vector(vector&& vect);
            vector(const std::initializer_list<T>& list);
            

            template <typename Iter>
            vector(Iter first, Iter last);
            

            ~vector() { ::operator delete[] (m_data); };


            iterator begin() { return iterator(this, 0); };
            iterator end()   { return iterator(this, m_used_size - 2); };
            const_iterator cbegin() { return const_iterator(this, 0); };
            const_iterator cend()   { return const_iterator(this, m_used_size - 2); };
            const_iterator cbegin() const { return const_iterator(this, 0); };
            const_iterator cend()   const { return const_iterator(this, m_used_size - 2); };
            reverse_iterator rbegin() { return reverse_iterator(&m_data[m_used_size - 1]); };
            reverse_iterator rend()   { return reverse_iterator(&m_data[0]); };
            const_reverse_iterator crbegin() const { return const_reverse_iterator(&m_data[m_used_size - 1]); };
            const_reverse_iterator crend()   const { return const_reverse_iterator(&m_data[0]); };

            
            T& operator[](int i);
            const T& operator[](int i) const;


            vector<T>& operator=(const vector<T>& vect);
            vector<T>& operator=(std::initializer_list<T>& l);
            vector<T>& operator=(vector<T>&& vect);

            void swap(vector<T>& vect);

            template <typename Iter>
            void assign(Iter first, Iter last);

            template <typename OutputIter>
            void assign(iterator first, iterator last, OutputIter output);

            void assign(int n, const T& value);
            inline void assign(std::initializer_list<T>& l);
            
            size_t max_size() { return (INT_MAX / sizeof(T)); };
            bool empty() const { return m_used_size == 2; };
            int size() const { return m_used_size - 2; }; 
            int capacity() const { return m_size; };

            void resize(int n, const T& value);
            void resize(int n);
            void reserve(int n);

            T* data(int n = 0) { return m_data + n; };
            const T& data() const noexcept;

            T& front() { return m_data[1]; };
            const T& front() const { return m_data[1]; };
            
            T& back() { return m_data[m_used_size - 2]; };
            const T& back() const { return m_data[m_used_size - 2]; };

            T& at(int pos);
            const T& at(int pos) const;

            // void push_back(const T& item);
            void push_back(T&& item); /*{ return push_back(static_cast<const T&>(item)); };*/
            void pop_back();

            template <typename ...Argc>
            iterator emplace(const_iterator position, Argc&& ...argc);

            template <typename ...Argc>
            T& emplace(const_iterator position, Argc&& ...argc);
            
            template <typename InputIter>
            iterator insert(const_iterator position, InputIter first, InputIter last);
            iterator insert(const_iterator position, const T& value);
            iterator insert(const_iterator position, int n, const T& value);
            iterator insert(const_iterator position, T&& value);
            iterator insert(const_iterator position, std::initializer_list<T>& l);

            iterator erase_first(const T& item);
            iterator erase_first_unsorted(const T& item);
            reverse_iterator erase_last(const T& item);
            reverse_iterator erase_last_unsorted(const T& item);
            
            reverse_iterator erase(reverse_iterator position);
            reverse_iterator erase(reverse_iterator first, reverse_iterator last);
            reverse_iterator erase_unsorted(reverse_iterator position);

            void clear() noexcept;
            void reset_lost_memory() noexcept;
            bool validate() const noexcept;
            int validate_iterator(const_iterator iter) const noexcept;

            template <typename U>
            friend vector<T> operator+(const vector<T>& v1, const vector<T>& v2) {};
    };

    

#ifdef 0

    template <typename T, typename Allocator>
    vector<T, Allocator>::vector() : m_size(15), m_used_size(2)
    {
        m_data = static_cast<T*>(::operator new(sizeof(T) * m_size));
    }

    template <typename T, typename Allocator>
    vector<T, Allocator>::vector(int a, T&& val) : m_used_size(2)
    {
        m_size = (a + m_used_size) <= 15 ? 15 : a;
        m_data = static_cast<T*>(::operator new(sizeof(T) * m_size));
        m_used_size += a;
        for(int i = 0; i < a; i++)
            new (m_data + i + 1) T(vstl::forward<T>(val));
    }

    //placeholder for copy constructor
    template <typename T, typename Allocator>
    vector<T, Allocator>::vector(const vector<T, Allocator>& vect)
    {
        this->m_data = vect.m_data;
        this->m_used_size = vect.m_used_size;
        this->m_size = vect.m_size;
    }

    template <typename T, typename Allocator>
    vector<T, Allocator>::vector(vector<T, Allocator>&& vect)
    {
        this->m_data = vect.m_data;
        this->m_used_size = vect.m_used_size;
        this->m_size = vect.m_size;

        vect.m_data = static_cast<T*>(::operator new(sizeof(T) * 15));
        vect.m_size = 15;
        vect.m_used_size = 2; 
    }

    template <typename T, typename Allocator>
   vector<T, Allocator>::vector(const std::initializer_list<T>& l) : m_size(l.size()), m_used_size(2)
    {
        m_data = static_cast<T*>(::operator new(sizeof(T) * m_size));
        for(auto i = l.begin(); i != l.end(); i++)
            this->push_back(*i);
    }

    template <typename T, typename Allocator>
    template <typename Iter>
    vector<T, Allocator>::vector(Iter first, Iter last) : m_size(last - first), m_used_size(2)
    {
        reallocate_space(m_size);
        for(auto i = first; i != last; i++)
            this->push_back(*i);
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::push_back(T&& item)
    {
        if((m_used_size + 1) < m_size)
        {
            new (m_data + m_used_size - 1) T(vstl::forward<T>(item));
            m_used_size++;
            return;
        } 
        reallocate_space(m_size * 2);
    };

    template <typename T, typename Allocator>
    void vector<T, Allocator>::pop_back()
    {
        if(m_used_size > 2)
            m_used_size--;
    };

    template <typename T, typename Allocator>
    void vector<T, Allocator>::reallocate_space(int sSize)
    {
        T *ptr = static_cast<T*>(::operator new(sizeof(T) * sSize));
        std::memcpy(ptr, m_data, sizeof(T) * m_size);
        m_size = sSize;
        ::operator delete[] (m_data);
        m_data = ptr;
    }

    template <typename T, typename Allocator>
    T& vector<T, Allocator>::operator[](int n)
    {
        return const_cast<T&>(static_cast<const vector<T>&>(*this)[n]);
    }

    template <typename T, typename Allocator>
    const T& vector<T, Allocator>::operator[](int n) const
    {
        if(n >= m_used_size)
            return m_data[0];
        return m_data[n + 1];
    }

    template <typename T, typename Allocator>
    inline void vector<T, Allocator>::resize(int n)
    {
        reallocate_space(m_size + n);
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::resize(int n, const T& value)
    {
        if(n > m_size)
        {
            for(int i = m_size; i < n; i++)
                m_data[m_used_size++] = T(value);       
        }
        this->resize(n);
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::clear() noexcept
    {
        reallocate_space(15);
        m_used_size = 2;
    }
    
    template <typename T, typename Allocator>
    void vector<T, Allocator>::assign(int n, const T& value)
    {
        m_used_size = 2;
        for(int i = 0; i < n; i++)
            this->push_back(value);
    }

    template <typename T, typename Allocator>
    template <typename Iter>
    void vector<T, Allocator>::assign(Iter first, Iter last)
    {
        for(auto i = first; i != last; i++) 
            this->push_back(*i);
    }

    template <typename T, typename Allocator>
    inline void vector<T, Allocator>::assign(std::initializer_list<T>& l)
    {
        this->assign(l.begin(), l.end());
    }

    template <typename T, typename Allocator>
    template <typename OutputIter>
    void vector<T, Allocator>::assign(iterator first, iterator last, OutputIter output)
    {
        for(auto i = first; i != last; i++)
            *i = *output++;
    };

    template <typename T, typename Allocator>
    vector<T, Allocator> operator+(vector<T, Allocator>& v1, vector<T, Allocator>& v2)
    {
        vector<T, Allocator> result(v1.begin(), v1.end());
        result.assign(v2.begin(), v2.end());
        return result;
    };
}
#endif 

#endif