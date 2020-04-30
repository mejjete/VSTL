#include <iostream>
namespace vstl
{
    template <typename T>
    class link_list
    {
        class Node;
    public:
        link_list() noexcept : pNode(nullptr) {};
        class iterator;
        iterator begin() { return iterator(pNode); };
        iterator end() { return iterator(nullptr); };    
        
        //undefined methods
        void push_back(T data);
        T pop_back();

        void traverse();
        class iterator
        {
        public:
            iterator() noexcept : currentNode(pNode) {};
            iterator(const Node* pNode) noexcept : currentNode(pNode) {};
            iterator& operator=(Node *pNode)
            {
                this->currentNode = pNode;
                return *this;
            }
            iterator& operator++()
            {
                if(currentNode)
                    currentNode = currentNode->nextNode;
                return *this; 
            }
            iterator operator++(int)
            {
                iterator iter = *this;
                ++*this;
                return iter;
            }
            bool operator !=(const iterator& iterator)
            { return currentNode != iterator.currentNode; };
            int operator*() { return currentNode->data; };
        private:
            const Node *currentNode;
        };
    private:
        class Node
        {
            T data;
            Node *nextNode;
            friend class link_list;
        };
        Node *pNode;
        Node *getNode(T data)
        {
            Node *newNode = new Node;
            newNode->data = data;
            newNode->nextNode = nullptr;
            return newNode;
        };
        Node*& GetRootNode() { return pNode; }
    };
    template <typename T>
    void link_list<T>::push_back(T data)
    {
        Node *temp = getNode(data);
        if(!GetRootNode())
            GetRootNode() = temp;
        else 
        {
            Node *crawler = GetRootNode();
            while(crawler->nextNode)
                crawler = crawler->nextNode;
            crawler->nextNode = temp;
        }
    };
    template <typename T>
    void link_list<T>::traverse()
    {
        Node *crawler = GetRootNode();
        while(crawler)
        {
            std::cout << crawler->data << " ";
            crawler = crawler->nextNode;
        }
        std::cout << std::endl;
    }
};