#ifndef VSTL_LIST
    #define VSTL_LIST
#include <iostream>
#include <initializer_list>
#include "iterators.hpp"
namespace vstl
{
    template <typename T>
    class list
    {
        private:
            struct Node
            {
                T data;
                Node *nextNode;
                Node *prevNode;
            };
            int count;
            Node *start;
            Node *tail;
            Node *getNode(T data);
        public:
            class iterator;
            class const_iterator;
            class reverse_iterator;
            class const_reverse_iterator;
            list();
            list(const list<T>& list);
            list(list<T>&& list);
            template <class Iter>
            list(Iter first, Iter last);
            list(std::initializer_list<T>& cl);
            ~list() { clear(); };

            iterator begin() { return iterator(start->nextNode); }
            iterator end() { return iterator(tail); }
            const_iterator cbegin() const { return const_iterator(start->nextNode); };
            const_iterator cend() const { return const_iterator(tail); };
            reverse_iterator rbegin() { return reverse_iterator(tail->prevNode); };
            reverse_iterator rend() { return start; }; 
            const_reverse_iterator crbegin() { return const_reverse_iterator(tail->prevNode); };
            const_reverse_iterator crend() const { return start; }; 

            iterator insert(iterator pos, T& value);
            iterator insert(iterator pos, T&& value);
            iterator insert(iterator pos, std::initializer_list<T> ilist);

            void push_back(const T& data);
            void push_back(T&& data);
            void push_front(const T& data);
            void push_front(T&& data);
            T pop_front();
            T pop_back();
            int size() const { return count - 2; };
            size_t max_size() const { return (INT_MAX / sizeof(Node)); };
            int capacity() const { return count; };
            void clear();
            bool empty() { return start == nullptr; };
            T& front() noexcept { return start->data; };
            T& back() noexcept { return tail->data; };
            void merge(list& l);
            void merge(list&& l);

            iterator next(iterator iter = begin())  { return ++iter; };
            iterator prev(iterator iter = end())    { return --iter; };

            class iterator
            {
                public:
                    typedef iterator                    self_type;
                    typedef T                           value_type;
                    typedef T&                          reference;
                    typedef Node*                       pointer;
                    typedef vstl::forward_iterator_tag  iterator_category;
                    typedef int                         difference_type;
                    iterator() : ptr_(nullptr) {};
                    iterator(pointer ptr) : ptr_(ptr) {}
                    iterator& operator=(pointer ptr) { ptr_ = ptr; return *this; };
                    self_type operator++();
                    self_type operator++(int junk);
                    self_type operator--();
                    self_type operator--(int junk);
                    reference operator*() { return ptr_->data; }
                    value_type operator->() { return ptr_->data; };
                    bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_;}
                    bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_;}
                private:
                    pointer ptr_;
            };
            class const_iterator
            {
                public:
                    typedef const_iterator              self_type;
                    typedef const T                     value_type;
                    typedef const T&                    reference;
                    typedef Node*                       pointer;
                    typedef vstl::forward_iterator_tag  iterator_category;
                    typedef int                         difference_type;
                    const_iterator() : ptr_(nullptr) {};
                    const_iterator(pointer ptr) : ptr_(ptr) {};
                    const_iterator& operator=(pointer ptr) { ptr_ = ptr; return *this; };
                    self_type operator++();
                    self_type operator++(int junk);
                    self_type operator--();
                    self_type operator--(int junk);
                    reference operator*() { return ptr_->data; }
                    value_type operator->() { return ptr_->data; };
                    bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_;}
                    bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_;}
                private:
                    pointer ptr_;
            };
            class reverse_iterator
            {
                public:
                    typedef reverse_iterator            self_type;
                    typedef T                           value_type;
                    typedef T&                          reference;
                    typedef Node*                       pointer;
                    typedef int                         difference_type;
                    typedef vstl::forward_iterator_tag  iterator_category;
                    reverse_iterator() : ptr_(nullptr) {};
                    reverse_iterator(pointer ptr) : ptr_(ptr) {}
                    self_type& operator=(pointer ptr) { ptr_ = ptr; return *this; };
                    self_type operator++();
                    self_type operator++(int junk); 
                    self_type operator--();
                    self_type operator--(int junk);
                    reference operator*() { return ptr_->data; }
                    pointer operator->() { return ptr_->data; }
                    bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
                    bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
                private:
                    pointer ptr_;
            };

            class const_reverse_iterator
            {
                public:
                    typedef const_reverse_iterator      self_type;
                    typedef const T                     value_type;
                    typedef const T&                    reference;
                    typedef Node*                       pointer;
                    typedef int                         difference_type;
                    typedef vstl::forward_iterator_tag  iterator_category;
                    const_reverse_iterator() : ptr_(nullptr) {};
                    const_reverse_iterator(pointer ptr) : ptr_(ptr) {}
                    self_type& operator=(pointer ptr) { ptr_ = ptr; return *this; };
                    self_type operator++();
                    self_type operator++(int junk); 
                    self_type operator--();
                    self_type operator--(int junk);
                    reference operator*() { return ptr_->data; }
                    pointer operator->() { return ptr_->data; }
                    bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
                    bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
                private:
                    pointer ptr_;
            };
    };
    
    template <typename T>
    list<T>::list() : count(2)
    {
        start = getNode(T());
        tail = getNode(T());
        start->nextNode = tail;
        tail->prevNode = start;
    }

    template <typename T>
    list<T>::list(const list<T>& list) : start(nullptr), tail(nullptr), count(0)
    {
        for(auto i = list.begin(); i != list.end(); i++)
            this->push_back(*i);
        this->count = list.count;
    };

    template <typename T>
    list<T>::list(list<T>&& list) : start(nullptr), tail(nullptr), count(0)
    {
        this->start = list.start;
        this->tail = list.tail;
        this->count = list.count;
    };

    template <typename T>
    template <class Iter>
    list<T>::list(Iter first, Iter last) : start(nullptr), tail(nullptr), count(0)
    {
        for(auto i = first; i != last; i++)
            push_back(*i);
    }

    template <typename T>
    list<T>::list(std::initializer_list<T>& l)
    {
        for(auto i = l.begin(); i != l.end(); i++)
            push_back(*i);
    }
    
    template <typename T>
    typename list<T>::Node* list<T>::getNode(T data)
    {
        Node *temp = new Node;
        temp->data = data;
        temp->nextNode = nullptr;
        temp->prevNode = nullptr;
        return temp; 
    };

    template <typename T>
    void list<T>::push_back(const T& data)
    {
        Node *temp = getNode(data);
        tail->prevNode->nextNode = temp;
        temp->prevNode = tail->prevNode;
        temp->nextNode = tail;
        tail->prevNode = temp;
        count++;
    }

    template <typename T>
    void list<T>::push_back(T&& data)
    {
        push_back(static_cast<T&>(data));
    }

    template <typename T>
    void list<T>::push_front(const T& data)
    {
        Node *temp = getNode(data);
        Node *data_start = start->nextNode;
        data_start->prevNode->nextNode = temp;
        temp->prevNode = data_start->prevNode;
        temp->nextNode = data_start;
        data_start->prevNode = temp;
        count++;
    };

    template <typename T>
    void list<T>::push_front(T&& data)
    {
        static_cast<T&>(data);
        push_front(data);
    }

    template <typename T>
    T list<T>::pop_back()
    {
        T data = tail->prevNode->data;
        Node *prevNode = tail->prevNode;
        if(prevNode == start)
            return T();
        prevNode->prevNode->nextNode = tail;
        tail->prevNode = prevNode->prevNode;
        delete prevNode;
        count--;
        return data;
    };

    template <typename T>
    T list<T>::pop_front()
    {
        T data = start->data;
        Node *current = start->nextNode;
        if(current == tail)
            return T();
        current->nextNode->prevNode = start;
        start->nextNode = current->nextNode;
        delete current;
        return data;
    }

    template <typename T>
    void list<T>::clear()
    {
        Node *prev = start;
        while(start)
        {
            start = start->nextNode;
            delete prev;
            prev = start;
        }
    }

    template <typename T>
    typename list<T>::iterator::self_type list<T>::iterator::operator++(int)
    {
        return operator++();
    };

    template <typename T>
    typename list<T>::iterator::self_type list<T>::iterator::operator++()
    {
        if(ptr_)
            ptr_ = ptr_->nextNode;
        else 
            return nullptr;
        return iterator(this->ptr_);
    };

    template <typename T>
    typename list<T>::iterator list<T>::iterator::operator--()
    {
        if(ptr_)
            ptr_ = ptr_->prevNode;
        return iterator(this->ptr_);
    };

    template <typename T>
    typename list<T>::iterator list<T>::iterator::operator--(int)
    {
        return operator++();
    };

    template <typename T>
    typename list<T>::const_iterator::self_type list<T>::const_iterator::operator++(int)
    {
        return operator++();
    };

    template <typename T>
    typename list<T>::const_iterator::self_type list<T>::const_iterator::operator++()
    {
        if(ptr_)
            ptr_ = ptr_->nextNode;
        return const_iterator(this->ptr_);
    };

    template <typename T>
    typename list<T>::reverse_iterator::self_type list<T>::reverse_iterator::operator++(int)
    {
        return operator++();
    };

    template <typename T>
    typename list<T>::reverse_iterator::self_type list<T>::reverse_iterator::operator++()
    {
        if(ptr_)
            ptr_ = ptr_->prevNode;
        return reverse_iterator(this->ptr_);
    };

    template <typename T>
    typename list<T>::iterator list<T>::insert(iterator pos, T& value)
    {
        if(pos)
            *pos = value;
        return pos;
    };

    template <typename T>
    typename list<T>::iterator list<T>::insert(iterator pos, T&& value)
    {
        *pos = value;
        return pos;
    };
}
#endif