#pragma once
#include <iostream>
#include <initializer_list>
//#include "iterators.hpp"
namespace vstl
{
    template <typename T>
    class l_list
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
            l_list() : count(0), start(nullptr), tail(nullptr) {};
            l_list(const l_list<T>& list);
            l_list(l_list<T>&& list);
            explicit l_list(const_iterator first, const_iterator last) {};
            ~l_list() { clear(); };

            iterator begin() { return iterator(start); }
            iterator end() { return nullptr; }
            const_iterator begin() const { return const_iterator(start); };
            const_iterator end() const { return nullptr; };
            reverse_iterator rbegin() { return reverse_iterator(tail); };
            reverse_iterator rend() { return nullptr; }; 
            iterator insert(iterator pos, T& value);
            iterator insert(iterator pos, T&& value);
            iterator insert(iterator pos, std::initializer_list<T> ilist);

            void push_back(T data);
            T pop_back();
            void traverse() const;
            int size() const { return count; }
            void clear();
            class iterator
            {
                public:
                    typedef iterator self_type;
                    typedef T value_type;
                    typedef T& reference;
                    typedef Node* pointer;
                    typedef std::forward_iterator_tag iterator_category;
                    typedef int difference_type;
                    iterator(pointer ptr) : ptr_(ptr) {}
                    self_type operator++();
                    self_type operator++(int junk);
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
                    const_iterator(pointer ptr) : ptr_(ptr) {};
                    self_type operator++();
                    self_type operator++(int junk);
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
                    reverse_iterator(pointer ptr) : ptr_(ptr) { }
                    self_type operator++();
                    self_type operator++(int junk); 
                    reference operator*() { return ptr_->data; }
                    pointer operator->() { return ptr_->data; }
                    bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
                    bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
                private:
                    pointer ptr_;
            };
    };
    
    template <typename T>
    l_list<T>::l_list(const l_list<T>& list) : start(nullptr), tail(nullptr), count(0)
    {
        for(auto i = list.begin(); i != list.end(); i++)
            this->push_back(*i);
        this->count = list.count;
    };

    template <typename T>
    l_list<T>::l_list(l_list<T>&& list) : start(nullptr), tail(nullptr), count(0)
    {
        this->start = list.start;
        this->count = list.count;
    };
    
    template <typename T>
    typename l_list<T>::Node* l_list<T>::getNode(T data)
    {
        Node *temp = new Node;
        temp->data = data;
        temp->nextNode = nullptr;
        temp->prevNode = nullptr;
        return temp; 
    };

    template <typename T>
    void l_list<T>::push_back(T data)
    {
        Node *temp = getNode(data);
        Node *prev = nullptr;
        if(!start)
        {
            start = temp;
            tail = start;
        }
        else
        {
            temp->prevNode = tail;
            tail->nextNode = temp;
            tail = temp;
        }
        count++;
    };

    template <typename T>
    T l_list<T>::pop_back()
    {
        T data = tail->data;
        Node *curr;
        if(tail->prevNode)
        {
            curr = tail;
            tail->prevNode->nextNode = nullptr;
            tail = tail->prevNode;
            delete curr;
        }
        else 
        {
            delete tail;
            start = tail = nullptr;
        }
        return data;
    };

    template <typename T>
    void l_list<T>::clear()
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
    typename l_list<T>::iterator::self_type l_list<T>::iterator::operator++(int)
    {
        return operator++();
    };

    template <typename T>
    typename l_list<T>::iterator::self_type l_list<T>::iterator::operator++()
    {
        if(ptr_)
            ptr_ = ptr_->nextNode;
        return iterator(this->ptr_);
    };

    template <typename T>
    typename l_list<T>::const_iterator::self_type l_list<T>::const_iterator::operator++(int)
    {
        return operator++();
    };

    template <typename T>
    typename l_list<T>::const_iterator::self_type l_list<T>::const_iterator::operator++()
    {
        if(ptr_)
            ptr_ = ptr_->nextNode;
        return const_iterator(this->ptr_);
    };

    template <typename T>
    typename l_list<T>::reverse_iterator::self_type l_list<T>::reverse_iterator::operator++(int)
    {
        return operator++();
    };

    template <typename T>
    typename l_list<T>::reverse_iterator::self_type l_list<T>::reverse_iterator::operator++()
    {
        if(ptr_)
            ptr_ = ptr_->prevNode;
        return reverse_iterator(this->ptr_);
    };

    template <typename T>
    typename l_list<T>::iterator l_list<T>::insert(iterator pos, T& value)
    {
        if(pos)
            *pos = value;
        return pos;
    };

    template <typename T>
    typename l_list<T>::iterator l_list<T>::insert(iterator pos, T&& value)
    {
        *pos = value;
        return pos;
    };
}