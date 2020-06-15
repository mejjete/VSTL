#ifndef VSTL_ALGORITHM
    #define VSTL_ALGORITHM
#include "viterator.hpp"
namespace vstl
{
    namespace alg
    {

        template <typename ForwardIterator>
        ForwardIterator min_element(ForwardIterator first, ForwardIterator last)
        {
            ForwardIterator minIt = first;
            for(auto i = ++first; i != last; i++)
                if(*i < *minIt)
                    minIt = i;
            return minIt;
        };

        template <typename ForwardIterator, typename Compare>
        ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp)
        {
            ForwardIterator minIt = first;
            for(auto i = ++first; i != last; i++)
                if(comp(*i, *minIt))
                    minIt = i;
            return minIt;
        };

        template <typename T>
        const T& min(const T& a, const T& b) { return a < b ? a : b; };

        template <typename T, typename Compare>
        const T& min(const T& a, const T& b, Compare comp) { return comp(a, b) ? a : b; };

        template <typename T>
        T min(std::initializer_list<T> ilist) { return *min_element(ilist.begin(), ilist.end()); };

        template <typename T, typename Compare>
        T min(std::initializer_list<T> ilist, Compare comp) { return *min_element(ilist.begin(), ilist.end(), comp); }; 

        template <typename ForwardIterator>
        ForwardIterator max_element(ForwardIterator first, ForwardIterator last)
        {
            ForwardIterator maxIt = first;
            for(auto i = ++first; i != last; i++)
                if(*i > *maxIt)
                    maxIt = i;
            return maxIt;
        }

        template <typename ForwardIterator, typename Compare>
        ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp)
        {
            ForwardIterator maxIt = first;
            for(auto i = ++first; i != last; i++)
                if(comp(*i, *maxIt))
                    maxIt = i;
            return maxIt;
        };

        template <typename T>
        const T& max(const T& a, const T& b) { return a > b ? a : b; };

        template <typename T, typename Compare>
        const T& max(const T& a, const T& b, Compare comp) { return comp(a, b) ? a : b; };

        template <typename T>
        T max(std::initializer_list<T> ilist) { return *max_element(ilist.begin(), ilist.end()); };

        template <typename T, typename Compare>
        T max(std::initializer_list<T> ilist, Compare comp) { return *max_element(ilist.begin(), ilist.end(), comp); }; 


        template <typename InputIter, typename Predicate>
        inline bool all_of(InputIter first, InputIter last, Predicate pred)
        {
            for(; first != last; first++)
                if(!pred(*first))
                    return false;
            return true;
        };

        template <typename InputIter, typename Predicate>
        inline bool any_of(InputIter first, InputIter last, Predicate pred)
        {
            for(; first != last; first++)
            {
                if(pred(*first))
                    return true;
            }
            return false;
        };

        template <typename InputIter, typename Predicate>
        inline bool none_of(InputIter first, InputIter last, Predicate pred)
        {
            for(; first != last; first++)
            if(pred(*first))
                return false;
            return true;
        }

        template <typename InputIter, typename T>
        InputIter find(InputIter first, InputIter last, const T& value)
        {
            for(auto i = first; i != last; i++)
                if(*i == value)
                    return i;
            return last;
        };
        
        template <typename InputIter, typename T, typename BinaryPredicate>
        InputIter find(InputIter first, InputIter last, const T& value, BinaryPredicate pred)
        {
            while((first != last) && !pred(*first, value))
			    ++first;
		    return first;
        };

        template <typename InputIter, typename T, typename UnaryPredicate>
        InputIter find_if(InputIter first, InputIter last, const T& value, UnaryPredicate pred)
        {
            while((first != last) && !pred(*first))
                first++;
            return first;
        };

        template <typename ForwardIterator, typename T>
        ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value)
        {
            ForwardIterator result = first;
            while(first != last)
            {
                if(!(*first == value))
                {
                    *result = std::move(*first);
                    ++result;
                }
                ++first;
            }
            return result;
        };

        // template <typename ForwardIterator, typename UnaryPredicate>
        // ForwardIterator remove(ForwardIterator first, ForwardIterator last, UnaryPredicate pred)
        // {
        //     ForwardIterator result = first;
        //     while(first != last)
        //     {
        //         if(!pred(*first))
        //         {
        //             *result = std::move(*first);
        //             ++result;
        //         }
        //         ++first;
        //     }
        //     return result;
        // };

        template <typename InputIterator, typename UnaryPredicate>
        inline InputIterator find_if_not(InputIterator first, InputIterator last, UnaryPredicate predicate)
        {
            for(; first != last; ++first)
            {
                if(!predicate(*first))
                    return first;
            }
            return last;
        }

        template <typename InputIter, typename T>
        void fill_out(InputIter first, InputIter last, T item)
        {
            for(auto i = first; i != last; i++)
                *i = item;
        };

        template <typename T>
        inline void fill_out(T& cl)
        {
            fill_out(cl.begin(), cl.end());
        };

        template <typename InputIter>
        int distance(InputIter first, InputIter last)
        {
            int i;
            for(i = 0; first != last; i++, first++)
                ;
            return i;  
        };
        
        template <typename InputIter>
        int count(InputIter first, InputIter last)
        {
            int c = 0;
            for(auto i = first; i != last; i++)
                c++;
            return c;
        };

        template <typename T>
        inline int count(T& cl) 
        { 
            return count(cl.begin(), cl.end()); 
        };
        
        template <typename InputIter, typename Expr>
        int count_if(InputIter first, InputIter last, Expr funct)
        {
            int cEx = 0;
            for(auto i = first; i != last; i++)
                if(funct(*i))
                    cEx++;
            return cEx;
        };

        //applies to unary operations
        template <typename InputIterator, class OutputIterator, class UnaryOperation>
        OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result, UnaryOperation op)
        {
            while (first != last) 
            {
                *result = op(*first);
                ++result; ++first;
            }
            return result;
        };

        //applies to binary operation
        template <typename InputIterator, class OutputIterator, class BinaryOperation>
        OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation op);

        template <typename InputIter, typename Function>
        Function for_each(InputIter first, InputIter last, Function func)
        {
            for(auto i = first; i != last; i++)
                func(*i);
            return func;
        };
    };
}
#endif