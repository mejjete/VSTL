#pragma once
#include <iostream>
#include <initializer_list>
#include "iterators.hpp"
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
            };
            int count;
            Node *start;
            Node *tail;
            Node *getNode(T data);
        public:
            l_list() : count(0), start(nullptr), tail(nullptr) {};
            l_list(const l_list<T>& list);
            l_list(l_list<T>&& list);
            //~l_list();
            //explicit l_list(l_list<T>::iterator f, l_list<T>::iterator last);
            
            class iterator;
            class const_iterator;
            class reverse_iterator;
            iterator begin() { return iterator(start); }
            iterator end() { return iterator(tail); }
            const_iterator begin() const { return const_iterator(start); };
            const_iterator end() const { return const_iterator(tail); };
            iterator insert(iterator pos, T& value);
            iterator insert(iterator pos, T&& value);
            iterator insert(iterator pos, std::initializer_list<T> ilist );
            void push_back(T data);
            T pop_back();
            void insert();
            void traverse() const;
            int size() const { return count; }
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
                    self_type operator++(int junk) { ptr_++; return *this; }
                    reference operator*() { return *ptr_; }
                    pointer operator->() { return ptr_; }
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

    // template <typename T>
    // l_list<T>::~l_list()
    // {
    //     Node *prev = start;
    //     while(start->nextNode)
    //     {
    //         delete[] prev;
    //         start = start->nextNode;
    //         prev = start;
    //     }
    //     delete[] start;
    // };
    
    template <typename T>
    typename l_list<T>::Node* l_list<T>::getNode(T data)
    {
        Node *temp = new Node;
        temp->data = data;
        temp->nextNode = nullptr;
        return temp; 
    };

    template <typename T>
    void l_list<T>::push_back(T data)
    {
        Node *temp = getNode(data);
        Node *curr = start;
        if(!start)
        {
            start = temp;
            tail = start;
        }
        else
        {
            Node* curr = tail;
            tail->nextNode = temp;
            tail = temp;
        }
        count++;
    };
    
    template <typename T>
    void l_list<T>::traverse() const
    {
        for(auto i = begin(); i != end(); i++)
            std::cout << *i << " ";
        std::cout << std::endl;
    };

    template <typename T>
    T l_list<T>::pop_back()
    {
        Node *temp = start;
        Node *prev = nullptr;
        while(temp->nextNode)
        {
            prev = temp;
            temp = temp->nextNode;
        }
        T pData = T(0);
        if(prev != nullptr)
            prev->nextNode = nullptr;
        pData = temp->data;
        delete temp;
        count--;
        return pData;
    };

    template <typename T>
    typename l_list<T>::iterator::self_type l_list<T>::iterator::operator++(int)
    {
        return operator++();
    };

    template <typename T>
    typename l_list<T>::iterator::self_type l_list<T>::iterator::operator++()
    {
        if(this->ptr_->nextNode)
            this->ptr_ = this->ptr_->nextNode;
        else 
        {
            while(this->ptr_->nextNode)
                this->ptr_ = this->ptr_->nextNode;
        }
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
        if(this->ptr_->nextNode)
            this->ptr_ = this->ptr_->nextNode;
        else 
        {
            while(this->ptr_->nextNode)
                this->ptr_ = this->ptr_->nextNode;
        }
        return const_iterator(this->ptr_);
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