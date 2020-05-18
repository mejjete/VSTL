#pragma once
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <initializer_list>
#include "iterators.hpp"
#include "valgorithm.hpp"
#include <climits>
namespace vstl
{
    template <typename T>
    class vector
    {
        private:
            T* m_data;
            size_t m_size;
            int m_used_size;
            void reallocate_space(int sSize);
        public:
            typedef vector<T>                                   self_type;
            typedef T                                           value_type;
            typedef const T                                     const_value_type;
            typedef T*                                          pointer;
            typedef const T*                                    const_pointer;
            typedef T&                                          reference;
            typedef const T&                                    const_reference;
            typedef pointer                                     iterator;
            typedef const_pointer                               const_iterator;
            typedef vstl::reverse_iterator<value_type>          reverse_iterator;
            typedef vstl::reverse_iterator<const_value_type>    const_reverse_iterator;

            vector();
            vector(const vector<T>& vect);
            vector(vector<T>&& vect);
            vector(const std::initializer_list<T>& list);
            template <typename Iter>
            vector(Iter first, Iter last);
            ~vector() { delete[] m_data; };

            iterator begin() { return &m_data[1]; };
            iterator end() { return &m_data[m_used_size - 1]; };
            const_iterator cbegin() const { &m_data[1]; };
            const_iterator cend() const { &m_data[m_used_size - 1]; };
            reverse_iterator rbegin() { return reverse_iterator(&m_data[m_used_size - 1]); };
            reverse_iterator rend() { return reverse_iterator(&m_data[0]); };
            const_reverse_iterator crbegin() { return const_reverse_iterator(&m_data[m_used_size - 1]); };
            const_reverse_iterator crend() { return const_reverse_iterator(&m_data[0]); };

            reference operator[](int i);
            const_reference operator[](int i) const;
            self_type& operator=(const self_type& vect);
            self_type& operator=(std::initializer_list<T>& l);
            self_type& operator=(self_type&& vect);

            void swap(self_type& vect);
            void assign(int n, const value_type& value);
            template <typename Iter>
            void assign(Iter first, Iter last);
            void assign(std::initializer_list<T>& l);
            
            size_t max_size() { return (INT_MAX / sizeof(value_type)); };
            bool empty() const { return m_used_size == 0; };
            int size() const { return m_used_size - 2; }; 
            int capacity() const { return m_size; };

            void resize(int n, const_reference value);
            void resize(int n);
            void reserve(int n);
            void set_capacity(int n);
            void shrink_to_fit();

            pointer data(int n = 0) { return m_data + n; };
            const_reference data() const noexcept;

            reference front() { return m_data[1]; };
            const_reference front() const { return m_data[1]; };
            
            reference back() { return m_data[m_used_size - 2]; };
            const_reference back() const { return m_data[m_used_size - 2]; };

            reference at(int pos);
            const_reference at(int pos) const;

            reference push_back();
            void push_back(const value_type& item);
            void newPush(const value_type& item);
            void *push_back_initializer();
            void push_back(value_type&& item) { return push_back(static_cast<const value_type&>(item)); };
            T pop_back();

            template <typename ...Argc>
            iterator emplace(const_iterator position, Argc&& ...argc);

            template <typename ...Argc>
            reference emplace(const_iterator position, Argc&& ...argc);
            
            template <typename InputIter>
            iterator insert(const_iterator position, InputIter first, InputIter last);
            iterator insert(const_iterator position, const value_type& value);
            iterator insert(const_iterator position, int n, const value_type& value);
            iterator insert(const_iterator position, value_type&& value);
            iterator insert(const_iterator position, std::initializer_list<T>& l);

            iterator erase_first(const T& item);
            iterator erase_first_unsorted(const T& item);
            reverse_iterator erase_last(const T& item);
            reverse_iterator erase_last_unsorted(const T& item);
            
            reverse_iterator erase(reverse_iterator position);
            reverse_iterator erase(reverse_iterator first, reverse_iterator last);
            reverse_iterator erase_unsorted(reverse_iterator position);

            void clear() noexcept;
            void reset_lost_memory() noexcept;
            bool validate() const noexcept;
            int validate_iterator(const_iterator iter) const noexcept;
    };

    template <typename T>
    vector<T>::vector() : m_size(15), m_used_size(2)
    {
        m_data = new T[m_size];
    }

    template <typename T>
    vector<T>::vector(const vector<T>& vect)
    {
        this->m_data = vect.m_data;
        this->m_used_size = vect.m_used_size;
        this->m_size = vect.m_size;
    }

    template <typename T>
    vector<T>::vector(vector<T>&& vect)
    {
        this->m_data = vect.m_data;
        this->m_used_size = vect.m_used_size;
        this->m_size = vect.m_size;

        vect.data = new T[15];
        vect.m_size = 15;
        vect.m_used_size = 0; 
    }

    template <typename T>
    vector<T>::vector(const std::initializer_list<T>& l) : m_size(l.size()), m_used_size(0)
    {
        m_data = new T[this->m_size];
        for(auto i = l.begin(); i != l.end(); i++)
            this->push_back(*i);
    }

    template <typename T>
    template <typename Iter>
    vector<T>::vector(Iter first, Iter last)
    {
        for(auto i = first; i != last; i++)
            this->push_back(*i);
    }

    template <typename T>
    void vector<T>::push_back(const value_type& item)
    {
        if((m_used_size + 1) > m_size)
            reallocate_space(m_size + 15);
        data(1)[m_used_size++] = item;
    };

    template <typename T>
    T vector<T>::pop_back()
    {
        if(m_used_size > 0)
            return m_data[--m_used_size];
        else 
            return T(0);
    };

    template <typename T>
    void vector<T>::reallocate_space(int sSize)
    {
        pointer ptr = new T[sSize];
        m_size = sSize;
        std::memcpy(ptr, m_data, sizeof(T) * m_size);
        delete[] m_data;
        m_data = ptr;
    }

    template <typename T>
    typename vector<T>::reference vector<T>::operator[](int n)
    {
        return const_cast<reference>(static_cast<const vector<T>&>(*this)[n]);
    }

    template <typename T>
    typename vector<T>::const_reference vector<T>::operator[](int n) const
    {
        if(n >= m_used_size)
            return m_data[0];
        return m_data[n - 1];
    }

    template <typename T>
    inline void vector<T>::resize(int n)
    {
        reallocate_space(m_size + n);
    }

    template <typename T>
    void vector<T>::resize(int n, const_reference value)
    {
        if(n > m_size)
        {
            for(int i = m_size; i < n; i++)
                m_data[m_used_size++] = T(value);       
        }
        this->resize(n);
    }

    template <typename T>
    void vector<T>::clear() noexcept
    {
        reallocate_space(15);
        m_used_size = 0;
    }

    template <typename T>
    void vector<T>::newPush(const value_type& item)
    {
        if((m_used_size + 1) > m_size)
            reallocate_space(m_size + 15);
        m_data[m_used_size - 1] = item;
        m_used_size++;
    }

    // template <typename T>
    // vector<T>& operator+(const vector<T>& vect1, const vector<T>& vect2)
    // {

    // }
}