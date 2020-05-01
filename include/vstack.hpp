#pragma once
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <initializer_list>
namespace vstl
{
    template <typename T>
    class stack
    {
    public:
        stack();
        stack(T *ptr, size_t size);
        stack(const stack& st);
        stack(const std::initializer_list<T>& l);
        stack(stack&& st);
        ~stack() { delete[] data; }
        void push(T&& elem);
        void push(const T& elem);
        T& pop();
        int& size() const { return this->size; }
    private:
        T& operator=(const stack& t);
        static const int defSize = 10;
        int sSize;
        int pos;
        T *data;
    };

    template <typename T>
    stack<T>::stack() : sSize(defSize), pos(0) { data = new T[sSize]; };
    template <typename T>
    stack<T>::stack(T *ptr, size_t size) : pos(0), sSize(0)
    {
        this->sSize = size;
        pos = size;
        data = new T[sSize];
        std::memcpy(data, ptr, sizeof(T) * size);
    }
    template <typename T>
    stack<T>::stack(const std::initializer_list<T>& l) : sSize(defSize), pos(0)
    {
        size_t cElem = l.size();
        data = new T[cElem + defSize];
        for(auto i = l.begin(); i != l.end(); i++)
            this->push(*i);
    }
    template <typename T>
    void stack<T>::push(T&& elem)
    {
        if((pos + 1) > sSize)
        {
            sSize += defSize;
            T* ptr = new T[sSize];
            std::memcpy(ptr, data, sizeof(T) * sSize);
            delete[] data;
            data = ptr;
        }
        data[pos++] = elem;
    }
    template <typename T>
    void stack<T>::push(const T& elem)
    {
        if((pos + 1) > sSize)
        {
            sSize += defSize;
            T* ptr = new T[sSize];
            std::memcpy(ptr, data, sizeof(T) * sSize);
            delete[] data;
            data = ptr;
        }
        data[pos++] = elem;
    }
    template <typename T>
    T& stack<T>::pop()
    {
        if((pos - 1) != 0)
            return data[--pos];
    }
}