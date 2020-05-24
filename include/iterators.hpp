#ifndef VSTL_ITERATORS
    #define VSTL_ITERATORS
namespace vstl
{   
    //iterator for sequence container
    template <typename T>
    class reverse_iterator
    {
        public:
            typedef reverse_iterator<T> self_type;
            typedef T                   value_type;
            typedef T*                  pointer;
            typedef T&                  reference;
            reverse_iterator();
            reverse_iterator(T* data) : data_(data) {};
            self_type& operator=(pointer ptr) { data_ = ptr; };
            self_type operator++() { return reverse_iterator(data_ = data_ - 1); };
            self_type operator++(int) { return operator++(); };
            reference operator*() { return *data_; };
            bool operator==(const self_type& rhs) { return data_ == rhs.data; };
            bool operator!=(const self_type& rhs) { return data_ != rhs.data_; };
        private:
            pointer data_;
    };
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

#endif