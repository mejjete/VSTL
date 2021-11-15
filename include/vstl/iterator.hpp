#ifndef VSTL_ITERATOR_H
#define VSTL_ITERATOR_H


#include <vstl/vdetail/vstl_iterator_base.hpp>
#include <vstl/bits/vstl_iterator.hpp>
#include <vstl/bits/vstl_iterator_funcs.hpp>


namespace vstl
{   
    //base iterator for most common container supported by the stl 
    template <typename T>
    class __base_iterator
    {
        public:
            typedef __base_iterator                         self_type;
            typedef T                                       container_type;
            typedef int                                     difference_type;
            typedef typename T::iterator::iterator_category iterator_category;
            typedef typename T::iterator::value_type        value_type;
            typedef typename T::iterator                    iterator_type;
            __base_iterator(T& cont) : container(cont), iter(cont.begin()) {};
            self_type& operator=(const self_type& i) { container = i.container; iter = i.iter; return *this; };
            self_type& begin()   { iter = container.begin(); return *this; };
            self_type& end()     { iter = container.end(); return *this; };
            self_type& operator++() { iter = container.next(iter); return *this; };
            self_type& operator++(int) { return operator++(); };

            decltype(container_type().operator[](0)) operator[](int n) { return container.operator[](n); };

            value_type& operator*() { return *iter; };
            bool operator==(self_type& i) { return this->iter == i.iter; };
            bool operator!=(self_type& i) { return this->iter != i.iter; };
        private:
            container_type& container;
            iterator_type iter;
    };


    template <typename T>
    using reverse_iterator = __base_iterator<T>;

    template <typename Container>
    auto begin(Container& c) -> decltype(c.begin())
    {
        return c.begin();
    };

    template <typename Container>
    auto end(Container& c) -> decltype (c.end())
    {
        return c.end();
    };

    template <typename Container>
    auto cbegin(const Container& c) -> decltype(c.cbegin())
    {
        return c.cbegin();
    }

    template <typename Container>
    auto cend(const Container& c) -> decltype(c.cend())
    {
        return c.cend();
    };

    template <typename Container>
    auto rbegin(Container& c) -> decltype(c.rbegin())
    {
        return c.rbegin();
    };

    template <typename Container>
    auto rend(Container& c) -> decltype(c.rend())
    {
        return c.rend();
    };

    template <typename Container>
    auto crbegin(const Container& c) -> decltype(c.crbegin())
    {
        return c.crbegin();
    };

    template <typename Container>
    auto crend(const Container& c) -> decltype(c.crend())
    {
        return c.crend();
    };
}
#endif