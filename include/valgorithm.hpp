#pragma once
namespace vstl
{
    namespace alg
    {
        template <typename InputIter, typename T>
        InputIter find(InputIter first, InputIter last, const T& value)
        {
            for(auto i = first; i != last; i++)
                if(*i = value)
                    return i;
        }

        template <typename InputIter, typename T>
        void fill_out(InputIter first, InputIter last, T item)
        {
            for(auto i = first; i != last; i++)
                *i = item;
        }

        template <typename T>
        inline void fill_out(T& cl)
        {
            fill_out(cl.begin(), cl.end());
        }

        template <typename InputIter>
        int count(InputIter first, InputIter last)
        {
            int i;
            for(i = 0; first != last; i++, first++)
                ;
            return i;  
        }
        
        template <typename T>
        inline int count(T& cl) 
        { 
            return count(cl.begin(), cl.end()); 
        };

        //applies to unary operations
        template <typename InputInputIterator, class OutputInputIterator, class UnaryOperation>
        OutputInputIterator transform(InputInputIterator first, InputInputIterator last, OutputInputIterator result, UnaryOperation op)
        {
            while (first != last) 
            {
                *result = op(*first);
                ++result; ++first;
            }
            return result;
        };
    };
};