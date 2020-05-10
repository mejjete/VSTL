#pragma once
#include <iostream>
#include <initializer_list>
//#include "iterators.hpp"
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
            list();
            list(const list<T>& list);
            list(list<T>&& list);
            template <class Iter>
            list(Iter first, Iter last);
            list(std::initializer_list<T>& cl);
            ~list() { clear(); };

            iterator begin() { return iterator(start); }
            iterator end() { return iterator(tail); }
            const_iterator cbegin() const { return const_iterator(start); };
            const_iterator cend() const { return const_iterator(tail); };
            reverse_iterator rbegin() { return reverse_iterator(tail); };
            reverse_iterator rend() { return nullptr; }; 
            iterator insert(iterator pos, T& value);
            iterator insert(iterator pos, T&& value);
            iterator insert(iterator pos, std::initializer_list<T> ilist);

            void push_back(T& data);
            void push_back(T&& data);
            void push_front(T& data);
            void push_front(T&& data);
            T pop_front();
            T pop_back();
            void traverse() const;
            int size() const { return count; };
            void clear();
            bool empty() { return start == nullptr; };
            T& front() noexcept { return start->data; };
            T& back() noexcept { return tail->data; };
            void merge(list& l);
            void merge(list&& l);
            class iterator
            {
                public:
                    typedef iterator self_type;
                    typedef T value_type;
                    typedef T& reference;
                    typedef Node* pointer;
                    typedef std::forward_iterator_tag iterator_category;
                    typedef int difference_type;
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
                    typedef const_iterator self_type;
                    typedef const T value_type;
                    typedef const T& reference;
                    typedef Node* pointer;
                    typedef std::forward_iterator_tag iterator_category;
                    typedef int difference_type;
                    const_iterator() : ptr_(nullptr) {};
                    const_iterator(pointer ptr) : ptr_(ptr) {};
                    const_iterator& operator=(pointer ptr) { ptr_ = ptr; return *this; };
                    self_type operator++();
                    self_type operator++(int junk);
                    self_type operator--();
                    self_type operator--(int junk);
                    const reference operator*() { return ptr_->data; }
                    const value_type operator->() { return ptr_->data; };
                    bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_;}
                    bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_;}
                private:
                    pointer ptr_;
            };
            class reverse_iterator
            {
                public:
                    typedef reverse_iterator self_type;
                    typedef T value_type;
                    typedef T& reference;
                    typedef Node* pointer;
                    typedef int difference_type;
                    typedef std::forward_iterator_tag iterator_category;
                    reverse_iterator() : ptr_(nullptr) {};
                    reverse_iterator(pointer ptr) : ptr_(ptr) {}
                    reverse_iterator& operator=(pointer ptr) { ptr_ = ptr; return *this; };
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
    list<T>::list() : count(0)
    {
        start = tail = getNode(T(0));
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
    void list<T>::push_back(T& data)
    {
        Node *temp = getNode(data);
        if(!start)
        {
            start = temp;
            tail = getNode(T(0));
            tail->prevNode = start;
            start->nextNode = tail;
        }
        else
        {
            tail->prevNode->nextNode = temp;
            temp->prevNode = tail->prevNode;
            tail->prevNode = temp;
            temp->nextNode = tail;
        }
        count++;
    };

    template <typename T>
    void list<T>::push_back(T&& data)
    {
        static_cast<T&>(data);
        push_back(data);
    }

    template <typename T>
    void list<T>::push_front(T& data)
    {
        Node *temp = getNode(data);
        if(!start)
            push_back(data);
        else 
        {
            temp->nextNode = start;
            start->prevNode = temp;
            start = temp;
        }
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
        if(tail->prevNode == start)
        {
            delete start;
            delete tail;
            start = tail = nullptr;
        }
        else 
        {
            Node *prev = tail->prevNode;
            tail->prevNode = prev->prevNode;
            prev->prevNode->nextNode = tail;
            delete prev;
        }
        return data;   
    };

    template <typename T>
    T list<T>::pop_front()
    {
        T data = start->data;
        Node *curr = nullptr;
        if(start->nextNode)
        {
            curr = start;
            start->nextNode->prevNode = nullptr;
            start = start->nextNode;
        }
        else
        {
            delete start;
            delete tail;
            start = tail = nullptr;
        }
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
    void list<T>::traverse() const
    {
        for(const_iterator i = cbegin(); i != cend(); i++)
            std::cout << *i << " ";
        std::cout << std::endl;
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
    typename list<T>::iterator::self_type list<T>::iterator::operator--()
    {
        if(ptr_)
            ptr_ = ptr_->prevNode;
        return iterator(this->ptr_);
    };

    template <typename T>
    typename list<T>::iterator::self_type list<T>::iterator::operator--(int)
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