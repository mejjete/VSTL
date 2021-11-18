#ifndef VSTL_BITS_ITERATOR_H
#define VSTL_BITS_ITERATOR_H


#include <vstl/bits/vstl_cxx_config.hpp>
#include <vstl/vdetail/vstl_iterator_base.hpp> 


namespace __vstl_cxx VSTL_NAMESPACE_VISIBILITY(hidden) 
{
    /**
     *  Normal iterator for standard sequence containers 
     */
    template <typename _Tp>
    struct __normal_iterator
    {
    private:

        typedef vstl::iterator_traits<_Tp>  __IterTraits;
        typename __IterTraits::pointer      __I_data;

    public:

        typedef typename __IterTraits::value_type           value_type;
        typedef typename __IterTraits::pointer              pointer;
        typedef typename __IterTraits::reference            reference;
        typedef typename __IterTraits::difference_type      difference_type;
        typedef typename __IterTraits::iterator_category    iterator_category;


        __normal_iterator(pointer __ptr = pointer()) : __I_data(__ptr) {};
        
        template <typename _Up, typename = typename vstl::is_same<typename vstl::iterator_traits<_Up>::pointer, pointer>::type>
        __normal_iterator(const __normal_iterator<_Up>& __lhs) noexcept(vstl::is_nothrow_copy_constructible<_Tp>::value) 
            : __I_data(__lhs.base()) 
        {};


        reference operator*() const                                 { return *__I_data; };
        pointer operator->() const                                  { return __I_data; };
        reference operator[](difference_type __diff) const noexcept { return __I_data + __diff; };

        __normal_iterator operator++()      { ++__I_data; return *this; };
        __normal_iterator operator++(int)   { return __normal_iterator(++__I_data); };
        __normal_iterator operator--()      { --__I_data; return *this; };
        __normal_iterator operator--(int)   { return __normal_iterator(--__I_data); };

        __normal_iterator& operator=(const __normal_iterator& __lhs)  { __I_data = __lhs.__I_data; return *this; };
        
        __normal_iterator operator+(difference_type __diff)  { return __normal_iterator(__I_data + __diff); };

        __normal_iterator& operator+=(difference_type __diff)    { __I_data += __diff; return *this; };
        __normal_iterator& operator-=(difference_type __diff)    { __I_data -= __diff; return *this; };

        pointer base() const { return __I_data; };    
    }; 


    /**
     *  In this concept, both iterators are allowed to vary in types, 
     *  conceptually in cv qualification
    */
    template <typename _IterL, typename _IterR>
    inline bool operator==(const __normal_iterator<_IterL>& __lhs, const __normal_iterator<_IterR>& __rhs) 
    {
        return __lhs.base() == __rhs.base();
    };

    template <typename _Iter>
    inline bool operator==(const __normal_iterator<_Iter>& __lhs, const __normal_iterator<_Iter>& __rhs)
    {
        return __lhs.base() == __rhs.base();
    };

    template <typename _IterL, typename _IterR>
    inline bool operator!=(const __normal_iterator<_IterL>& __lhs, const __normal_iterator<_IterR>& __rhs)
    {
        return !(__lhs == __rhs);
    };

    template <typename _Iter>
    inline bool operator!=(const __normal_iterator<_Iter>& __lhs, const __normal_iterator<_Iter>& __rhs)
    {
        return !(__lhs == __rhs);
    };

    template <typename _IterL, typename _IterR>
    inline bool operator<(const __normal_iterator<_IterL>& __lhs, const __normal_iterator<_IterR>& __rhs)
    {
        return __lhs.base() < __rhs.base();
    };

    template <typename _Iter>
    inline bool operator<(const __normal_iterator<_Iter>& __lhs, const __normal_iterator<_Iter>& __rhs)
    {
        return __lhs.base() < __rhs.base();
    };

    template <typename _IterL, typename _IterR>
    inline bool operator>(const __normal_iterator<_IterL>& __lhs, const __normal_iterator<_IterR>& __rhs)
    {
        return __lhs.base() > __rhs.base();
    };

    template <typename _Iter>
    inline bool operator>(const __normal_iterator<_Iter>& __lhs, const __normal_iterator<_Iter>& __rhs)
    {
        return __lhs.base() > __rhs.base();
    };

    template <typename _IterL, typename _IterR>
    inline auto operator-(const __normal_iterator<_IterL>& __lhs, const __normal_iterator<_IterR>& __rhs)
        noexcept -> decltype(__lhs.base() - __rhs.base())
    {
        return __lhs.base() - __rhs.base();
    };

    template <typename _Iter>
    inline auto operator-(const __normal_iterator<_Iter>& __lhs, const __normal_iterator<_Iter>& __rhs)
        noexcept -> decltype(__lhs.base() - __rhs.base())
    {
        return __lhs.base() - __rhs.base();
    };

    template <typename _IterL, typename _IterR>
    inline auto operator+(const __normal_iterator<_IterL>& __lhs, const __normal_iterator<_IterR>& __rhs)
        noexcept -> decltype(__lhs.base() - __rhs.base())
    {
        return __lhs.base() + __rhs.base();
    };

    template <typename _Iter>
    inline auto operator+(const __normal_iterator<_Iter>& __lhs, const __normal_iterator<_Iter>& __rhs)
        noexcept -> decltype(__lhs.base() + __rhs.base())
    {
        return __lhs.base() + __rhs.base();
    };



        /**
     *  Reverse iterator for standard sequence container 
     *  
     *  Since the __reverse_iterator impelemted in terms of std compatibility,
     *  the access evaluates as (current_position - 1)
    */
    template <typename _Tp>
    class __reverse_iterator
    {
    private:

        typedef vstl::iterator_traits<_Tp>  __IterTraits;
        typename __IterTraits::pointer      __I_data;

    public:

        typedef typename __IterTraits::value_type           value_type;
        typedef typename __IterTraits::pointer              pointer;
        typedef typename __IterTraits::reference            reference;
        typedef typename __IterTraits::difference_type      difference_type;
        typedef typename __IterTraits::iterator_category    iterator_category;


        __reverse_iterator(pointer __ptr = pointer()) : __I_data(__ptr) {};
        
        template <typename _Up, typename = typename vstl::is_same<_Up, _Tp>::type>
        __reverse_iterator(const __reverse_iterator<_Up>& __rhs) noexcept(vstl::is_nothrow_copy_constructible<_Tp>::value) : __I_data(__rhs.__I_data)
        {}
        

        reference operator*() const                                 { return *(__I_data - 1); };
        pointer operator->() const                                  { return __I_data - 1; };
        reference operator[](difference_type __diff) const noexcept { return __I_data - __diff - 1; };

        __reverse_iterator operator++()      { --__I_data; return *this; };
        __reverse_iterator operator++(int)   { return __reverse_iterator(--__I_data); };
        __reverse_iterator operator--()      { ++__I_data; return *this; };
        __reverse_iterator operator--(int)   { return __reverse_iterator(++__I_data); };

        __reverse_iterator& operator=(const __reverse_iterator& __lhs)  { __I_data = __lhs.__I_data; return *this; };

        __reverse_iterator operator+(difference_type __diff)  { return __reverse_iterator(__I_data + __diff); };

        __reverse_iterator& operator+=(difference_type __diff)    { __I_data += __diff; return *this; };
        __reverse_iterator& operator-=(difference_type __diff)    { __I_data -= __diff; return *this; };

        const pointer base() const { return __I_data; };  
    };


    /**
     *  Hint class for compatibility with standard interface 
     */
    template <typename _Tp>
    using reverse_iterator = __vstl_cxx::__reverse_iterator<_Tp>;



    /* same as for the __normal_iterator */
    template <typename _IterL, typename _IterR>
    inline bool operator==(const __reverse_iterator<_IterL>& __lhs, const __reverse_iterator<_IterR>& __rhs) 
    {
        return __lhs.base() == __rhs.base();
    };

    template <typename _Iter>
    inline bool operator==(const __reverse_iterator<_Iter>& __lhs, const __reverse_iterator<_Iter>& __rhs)
    {
        return __lhs.base() == __rhs.base();
    };

    template <typename _IterL, typename _IterR>
    inline bool operator!=(const __reverse_iterator<_IterL>& __lhs, const __reverse_iterator<_IterR>& __rhs)
    {
        return !(__lhs == __rhs);
    };

    template <typename _Iter>
    inline bool operator!=(const __reverse_iterator<_Iter>& __lhs, const __reverse_iterator<_Iter>& __rhs)
    {
        return !(__lhs == __rhs);
    };

    template <typename _IterL, typename _IterR>
    inline bool operator<(const __reverse_iterator<_IterL>& __lhs, const __reverse_iterator<_IterR>& __rhs)
    {
        return __lhs.base() < __rhs.base();
    };

    template <typename _Iter>
    inline bool operator<(const __reverse_iterator<_Iter>& __lhs, const __reverse_iterator<_Iter>& __rhs)
    {
        return __lhs.base() < __rhs.base();
    };

    template <typename _IterL, typename _IterR>
    inline bool operator>(const __reverse_iterator<_IterL>& __lhs, const __reverse_iterator<_IterR>& __rhs)
    {
        return __lhs.base() > __rhs.base();
    };

    template <typename _Iter>
    inline bool operator>(const __reverse_iterator<_Iter>& __lhs, const __reverse_iterator<_Iter>& __rhs)
    {
        return __lhs.base() > __rhs.base();
    };

    template <typename _IterL, typename _IterR>
    inline auto operator-(const __reverse_iterator<_IterL>& __lhs, const __reverse_iterator<_IterR>& __rhs)
        noexcept -> decltype(__lhs.base() - __rhs.base())
    {
        return __lhs.base() - __rhs.base();
    };

    template <typename _Iter>
    inline auto operator-(const __reverse_iterator<_Iter>& __lhs, const __reverse_iterator<_Iter>& __rhs)
        noexcept -> decltype(__lhs.base() - __rhs.base())
    {
        return __lhs.base() - __rhs.base();
    };

    template <typename _IterL, typename _IterR>
    inline auto operator+(const __reverse_iterator<_IterL>& __lhs, const __reverse_iterator<_IterR>& __rhs)
        noexcept -> decltype(__lhs.base() - __rhs.base())
    {
        return __lhs.base() + __rhs.base();
    };

    template <typename _Iter>
    inline auto operator+(const __reverse_iterator<_Iter>& __lhs, const __reverse_iterator<_Iter>& __rhs)
        noexcept -> decltype(__lhs.base() + __rhs.base())
    {
        return __lhs.base() + __rhs.base();
    };
};
#endif 