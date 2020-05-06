#pragma once
namespace vstl
{
    template <typename iterator>
    int distance(iterator begin, iterator end)
    {
        int count = 0;
        for(iterator i = begin; i != end; i++)
            count++;
        return count;
    }
    
    //applies to unary operations
    template <typename InputIterator, class OutputIterator, class UnaryOperation>
    OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result, UnaryOperation op)
    {
        while (first != last) {
            *result = op(*first);
            ++result; ++first;
        }
        return result;
    }
}


/*class iterator {
    iterator(const iterator&);
    ~iterator();
    iterator& operator=(const iterator&);
    iterator& operator++(); //prefix increment
    reference operator*() const;
    friend void swap(iterator& lhs, iterator& rhs); //C++11 I think
};

class input_iterator : public virtual iterator {
    iterator operator++(int); //postfix increment
    value_type operator*() const;
    pointer operator->() const;
    friend bool operator==(const iterator&, const iterator&);
    friend bool operator!=(const iterator&, const iterator&); 
};
//once an input iterator has been dereferenced, it is 
//undefined to dereference one before that.

class output_iterator : public virtual iterator {
    reference operator*() const;
    iterator operator++(int); //postfix increment
};
//dereferences may only be on the left side of an assignment
//once an output iterator has been dereferenced, it is 
//undefined to dereference one before that.

class forward_iterator : input_iterator, output_iterator {
    forward_iterator();
};
//multiple passes allowed

class bidirectional_iterator : forward_iterator {
    iterator& operator--(); //prefix decrement
    iterator operator--(int); //postfix decrement
};

class random_access_iterator : bidirectional_iterator {
    friend bool operator<(const iterator&, const iterator&);
    friend bool operator>(const iterator&, const iterator&);
    friend bool operator<=(const iterator&, const iterator&);
    friend bool operator>=(const iterator&, const iterator&);

    iterator& operator+=(size_type);
    friend iterator operator+(const iterator&, size_type);
    friend iterator operator+(size_type, const iterator&);
    iterator& operator-=(size_type);  
    friend iterator operator-(const iterator&, size_type);
    friend difference_type operator-(iterator, iterator);

    reference operator[](size_type) const;
};

class contiguous_iterator : random_access_iterator { //C++17
}; //elements are stored contiguously in memory.
*/