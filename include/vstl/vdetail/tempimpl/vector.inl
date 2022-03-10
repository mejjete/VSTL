#ifndef VSTL_VECTOR_TMPIMPL_INL
#define VSTL_VECTOR_TMPIMPL_INL


#include <limits>
#include <vstl/memory.hpp>
#include <vstl/algorithm.hpp>


namespace vstl
{
    template <typename _Tp>
    using _A_size_type = typename _Tp::size_type;

    template <typename _Tp>
    using _A_diff_type = typename _Tp::difference_type;



    /**
     * @brief Returns the maximum avalaible size for particular sequences container
     * 
     * @return maximum avalaible size
     */
    template <typename _Alloc>
    inline _A_size_type<_Alloc> __max_seq_size(const _Alloc& __alloc) noexcept
    {
        /**
         * std::ptrdiff_t is said to be a type for storing maximum difference between pointers,
         * hence, we can't store value greater than std::numeric_limist<std::ptrdiff_t>::max even if 
         * allocator says we can
         */
        auto __max_ptr_size = std::numeric_limits<std::ptrdiff_t>::max();
        auto __max_alloc_size = vstl::allocator_traits<_Alloc>::max_size();
        return vstl::__aux_min(__max_ptr_size, __max_alloc_size);    
    };



    /**
     * @brief Checks initial size in sequences containers 
     * 
     * @param __sz size to check
     * @return initial size
     * If requested size is greater than maximum avalaible size for that container, 
     * exception std::length_error is thrown
     */
    template <typename _Tp, typename _Alloc>
    inline _Tp __check_seq_size(_Tp __sz, const _Alloc& __alloc)
    {
        if(__sz > __max_seq_size(__alloc))
            throw std::length_error("__check_seq_size error : invalid length of sequence container");
        return __sz;
    };
}

#endif 