#pragma once
#include <iostream>
#include <cstring>
#include "iterators.hpp"
namespace vstl
{
    template <typename T>
    class vector
    {
        private:
            T* data;
            size_t size;
            int used_size;
            void reallocate_space();
        public:
            typedef vector<T>                   self_type;
            typedef T                           value_type;
            typedef T*                          pointer;
            typedef const T*                    const_pointer;
            typedef T&                          reference;
            typedef const T&                    const_reference;
            typedef T*                          iterator;
            typedef const_pointer               const_iterator;
            typedef vstl::reverse_iterator<T>   reverse_iterator;

            vector();
            vector(const vector<T>& vect);
            vector(vector<T>&& vect);
            self_type& operator+(const self_type& rhs);

            iterator begin() { return &data[0]; };
            iterator end() { return &data[used_size]; };
            const_iterator cbegin() const { &data[0]; };
            const_iterator cend() const { &data[used_size]; };
            reverse_iterator rbegin() { reverse_iterator(data); };
            reverse_iterator rend() { reverse_iterator(&data[used_sizes]); };

            void push_back(const T& item);
            T pop_back();
    };

    template <typename T>
    vector<T>::vector() : size(15), used_size(0)
    {
        data = new T[size];
        std::memset(data, 0, size * sizeof(T));
    }

    template <typename T>
    void vector<T>::push_back(const T& item)
    {
        if((used_size + 1) > size)
            reallocate_space();
        data[used_size++] = item;
    };

    template <typename T>
    T vector<T>::pop_back()
    {
        if(used_size > 0)
            return data[--used_size];
        else 
            return T(0);
    };

    template <typename T>
    void vector<T>::reallocate_space()
    {
        pointer ptr = new T[size + 15];
        std::memcpy(ptr, data, sizeof(T) * size);
        size += 15;
        delete[] data;
        data = ptr;
    }
}