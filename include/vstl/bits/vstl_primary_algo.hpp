#ifndef VSTL_PRIMARY_ALGORITHM_H
#define VSTL_PRIMARY_ALGORITHM_H


#include <vstl/utility.hpp>
#include <vstl/vdetail/predicate.hpp>
#include <vstl/vdetail/vstl_iterator_base.hpp>


namespace vstl
{
    template <typename _Tp, typename _Up>
    struct __greater_type
    {
        typedef vstl::conditional_t<(sizeof(_Tp) > sizeof(_Up)), _Tp, _Up> type;
    };


    template <typename T>
    const T& min(const T& a, const T& b) { return a < b ? a : b; };


    template <typename _Tp, typename _Up>
    const typename __greater_type<_Tp, _Up>::type __aux_min(const _Tp& __a, const _Up& __b) 
    { return __a < __b ? __a : __b; };


    template <typename T, typename Compare>
    const T& min(const T& a, const T& b, Compare comp) { return comp(a, b) ? a : b; };


    template <typename T>
    T min(std::initializer_list<T> ilist) { return *min_element(ilist.begin(), ilist.end()); };


    template <typename T, typename Compare>
    T min(std::initializer_list<T> ilist, Compare comp) { return *min_element(ilist.begin(), ilist.end(), comp); }; 


    template <typename _Iter1, typename _Iter2, typename = vstl::_RequireIter<_Iter1, vstl::random_access_iterator_tag>, 
        typename = vstl::_RequireIter<_Iter2, vstl::random_access_iterator_tag>>
    auto __newlast1(_Iter1 __first1, _Iter1 __last1, _Iter1 __first2, _Iter2 __last2) 
        -> decltype(vstl::__aux_min(vstl::declval<__iter_diff_t<_Iter1>>(), vstl::declval<__iter_diff_t<_Iter2>>()))
    {
        __iter_diff_t<_Iter1> __diff1 = __last1 - __first1;
        __iter_diff_t<_Iter2> __diff2 = __last2 - __first2;
        return __aux_min(__diff1, __diff2);
    };



    /**
     * @brief Checks if the first range [__first1, __last1) is lexicographically 
     * less than the second range [__first2, __last2)
     * 
     * @param __first1 first range start
     * @param __last1 first range end
     * @param __first2 second range start
     * @param __last2 second range end
     * @param __comp comparator
     */
    template <typename _Iter1, typename _Iter2, typename _Comp>
    bool lexicographical_compare(_Iter1 __first1, _Iter1 __last1, _Iter2 __first2, _Iter2 __last2, _Comp __comp)
    {
        auto __less_diff = __newlast1(__first1, __last1, __first2, __last2);
        _Iter1 __head1 = __first1;
        _Iter2 __head2 = __first2;

        for(; __less_diff != 0; --__less_diff, ++__head1, ++__head2)
        {
            if(__comp(*__head1, *__head2))
                return true;
            if(__comp(*__head2, *__head1))
                return false;
        }

        return (__head1 == __last2) && (__head2 != __last2); 
    };



    /**
     * @brief Checks if the first range [__first1, __last1) is lexicographically 
     * less than the second range [__first2, __last2)
     */
    template <typename _Iter1, typename _Iter2>
    inline bool lexicographical_compare(_Iter1 __first1, _Iter1 __last1, _Iter2 __first2, _Iter2 __last2)
    {
        return lexicographical_compare(__first1, __last1, __first2, __last2, 
            vstl::__less_aux<vstl::__iter_value_t<_Iter1>, vstl::__iter_value_t<_Iter1>>()); 
    };



    /**
     * @brief Compares range [__first1, __last1) to range started with __first2 
     * 
     * @param __first1 first range start
     * @param __last1 first range end
     * @param __first2 second range start
     */
    template <typename _Iter>
    inline bool equal(_Iter __first1, _Iter __last1, _Iter __first2)
    {
        _Iter __curr = __first1;
        for(; __curr != __last1; ++__curr, ++__first2)
            if(*__curr != *__first2)
                return false;
        return true;
    };
};


#endif 