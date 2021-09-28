#ifndef VSTL_BITS_ITERATOR_H
#define VSTL_BITS_ITERATOR_H


#include <vstl/bits/vstl_cxx_config.hpp>
#include <vstl/vdetail/vstl_iterator_base.hpp> 


namespace __vstl_cxx VSTL_NAMESPACE_VISIBILITY(hidden) 
{
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
        
        __normal_iterator(const __normal_iterator& __rhs) noexcept(vstl::is_nothrow_copy_constructible<_Tp>::value) : __I_data(__rhs.__I_data)
        {}
        
        __normal_iterator(__normal_iterator&& __rhs) noexcept(vstl::is_nothrow_move_constructible<_Tp>::value) : __I_data(__rhs.__I_data)
        {
            __rhs.__I_data = pointer();
        }

        reference operator*() const         { return *__I_data; };
        pointer operator->() const          { return __I_data; };
        __normal_iterator operator++()      { ++__I_data; return *this; };
        __normal_iterator operator++(int)   { return __normal_iterator(++__I_data); };
        __normal_iterator operator--()      { --__I_data; return *this; };
        __normal_iterator operator--(int)   { return __normal_iterator(--__I_data); };

        __normal_iterator operator+(difference_type __diff)const{ return __normal_iterator(__I_data + __diff); };
        __normal_iterator operator-(difference_type __diff)const{ return __normal_iterator(__I_data - __diff); };

        __normal_iterator operator+=(difference_type __diff)    { *this = __normal_iterator(__I_data + __diff); return *this; };
        __normal_iterator operator-=(difference_type __diff)    { *this = __normal_iterator(__I_data + __diff); return *this; };

        bool base() { return __I_data; };    
    }; 


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
        return __lhs < __rhs;
    };

    template <typename _Iter>
    inline bool operator<(const __normal_iterator<_Iter>& __lhs, const __normal_iterator<_Iter>& __rhs)
    {
        return __lhs < __rhs;
    };

    template <typename _IterL, typename _IterR>
    inline bool operator>(const __normal_iterator<_IterL>& __lhs, const __normal_iterator<_IterR>& __rhs)
    {
        return __lhs > __rhs;
    };

    template <typename _Iter>
    inline bool operator>(const __normal_iterator<_Iter>& __lhs, const __normal_iterator<_Iter>& __rhs)
    {
        return __lhs > __rhs;
    };
};

#endif 