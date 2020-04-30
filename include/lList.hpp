#include <iostream>
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
            return iterator(&start->data);
        }
        iterator end()
        {
            Node *temp = start;
            while(temp->nextNode)
                temp = temp->nextNode;
            return iterator(&temp->data);
        }
        void push_back(T data);
        T& pop_back();
        void traverse();
        class iterator
        {
            public:
                typedef iterator self_type;
                typedef T value_type;
                typedef T& reference;
                typedef T* pointer;
                typedef std::forward_iterator_tag iterator_category;
                typedef int difference_type;
                iterator(pointer ptr) : ptr_(ptr) {}
                self_type operator++();
                self_type operator++(int junk);
                reference operator*() { return *ptr_; }
                const pointer operator->() { return ptr_; };
                bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_;}
                bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
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
        Node *temp = start;
        while(temp)
        {
            std::cout << temp->data << " ";
            temp = temp->nextNode;
        }
        std::cout << std::endl;
    }
}