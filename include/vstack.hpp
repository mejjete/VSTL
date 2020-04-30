#include <cstdlib>
#include <cstring>
#include <cstdio>
namespace vstl
{
    template <typename T>
    class stack
    {
    public:
        stack();
        stack(T *ptr, size_t size);
        stack(const stack& st);
        stack(stack&& st);
        void push(T&& elem);
        T& pop();
        int size() const;
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
    T& stack<T>::pop()
    {
        if((pos - 1) != 0)
            return data[--pos];
    }
}