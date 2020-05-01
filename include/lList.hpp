#pragma once
#include <iostream>
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
        Node *getNode(T data)
        {
            Node *temp = new Node;
            temp->data = data;
            temp->nextNode = nullptr;
            return temp; 
        }
    public:
        class iterator;
        l_list() : count(0), start(nullptr) {};
        iterator begin()
        {
            return iterator(start);
        }
        iterator end()
        {
            Node *temp = start;
            while(temp->nextNode)
                temp = temp->nextNode;
            return iterator(temp);
        }
        void push_back(T data);
        T pop_back();
        void traverse();
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
                bool operator==(const self_type& rhs) { return &ptr_->data == &rhs.ptr_->data;}
                bool operator!=(const self_type& rhs) { return &ptr_->data != &rhs.ptr_->data;}
            private:
                pointer ptr_;
        };
    };
    
    template <typename T>
    void l_list<T>::push_back(T data)
    {
        Node *temp = getNode(data);
        if(!start)
            start = temp;
        else
        {
            Node *inter = start;
            while(inter->nextNode)
                inter = inter->nextNode;
            inter->nextNode = temp;
            count++;
        }
    };
    
    template <typename T>
    void l_list<T>::traverse()
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
    }

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
}