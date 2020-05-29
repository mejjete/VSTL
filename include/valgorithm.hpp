#ifndef VSTL_ALGORITHM
    #define VSTL_ALGORITHM
namespace vstl
{
    namespace alg
    {
        template <typename ForwardIterator>
        ForwardIterator min_elem(ForwardIterator first, ForwardIterator last)
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
                if(compare(*i, *minIt))
                    minIt = i;
            return minIt;
        };

        template <typename ForwardIterator, typename Compare>
        ForwardIterator max_element(ForwardIterator first, ForwardIterator last)
        {
            ForwardIterator maxIt = first;
            for(auto i = ++first; i != last; i++)
                if(*maxIt > *i)
                    maxIt = i;
            return maxIt;
        }

        template <typename ForwardIterator, typename Compare>
        ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp)
        {
            ForwardIterator maxIt = first;
            for(auto i = ++first; i != last; i++)
                if(compare(*maxIt, *i))
                    maxIt = i;
            return maxIt;
        };

        template <typename InputIter, typename T>
        InputIter find(InputIter first, InputIter last, const T& value)
        {
            for(auto i = first; i != last; i++)
                if(*i == value)
                    return i;
        };

        template <typename InputIter, typename T>
        InputIter find_if(InputIter first, InputIter last, const T& value)
        {
            
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

        template <typename InputIter, typename Function>
        Function for_each(InputIter first, InputIter last, Function func)
        {
            for(auto i = first; i != last; i++)
                func(*i);
            return func;
        };
    };
};
#endif