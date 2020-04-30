namespace vstd
{
    #include <cstdlib>
    #include <cstring>
    #include <cstdio>
    template <typename T>
    class stack
    {
    public:
        stack() : pos(0), size(0), data(nullptr) {};
        stack(T *ptr, size_t size);
        stack(const stack& st);
        stack(stack&& st);
        T& push(T& elem);
    private:
        T& operator=(const stack& t);
        static const defSize;
        int size;
        int pos;
        T *data;
    };

    template <typename T>
    stack<T>::stack() : size(10) { defSize = new T[size]; };
    template <typename T>
    stack<T>::stack(T *ptr, size_t size) : pos(0), size(0)
    {
        this->size = size;
        pos = size;
        data = new T[size];
        std::memcpy(data, ptr, sizeof(T) * size);
    }
}