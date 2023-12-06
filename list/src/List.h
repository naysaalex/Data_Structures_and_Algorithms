#pragma once

#include <cstddef> // size_t
#include <iterator> // std::bidirectional_iterator_tag
#include <type_traits> // std::is_same, std::enable_if
#include <iostream>

using namespace std;

template <class T>
class List {
    private:
    struct Node {
        Node *next, *prev;
        T data;
        explicit Node(Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev} {}
        explicit Node(const T& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{data} {}
        explicit Node(T&& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{std::move(data)} {}
    };

    template <typename pointer_type, typename reference_type>
    class basic_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = pointer_type;
        using reference         = reference_type;
    private:
        friend class List<value_type>;
        using Node = typename List<value_type>::Node;

        Node* node;

        explicit basic_iterator(Node* ptr) noexcept : node{ptr} {}
        explicit basic_iterator(const Node* ptr) noexcept : node{const_cast<Node*>(ptr)} {}

    public:
        basic_iterator() : node{nullptr} {}
        basic_iterator(const basic_iterator&) = default;
        basic_iterator(basic_iterator&&) = default;
        ~basic_iterator() = default;
        basic_iterator& operator=(const basic_iterator&) = default;
        basic_iterator& operator=(basic_iterator&&) = default;

        reference operator*() const {
            // TODO
            // cout<<"*"<<endl;
            return node->data;
        }
        pointer operator->() const {
            // TODO
            // cout<<"->"<<endl;
            return &(node->data);
        }

        // Prefix Increment: ++a
        basic_iterator& operator++() {
            // TODO
            // cout<<"pre++"<<endl;
            if(node->next!=nullptr){
            node = node->next;
            }
            return *this;
        }
        // Postfix Increment: a++
        basic_iterator operator++(int) {
            // TODO
            // cout<<"post++"<<endl;
            basic_iterator temp = *this;
            if(node->next!=nullptr){
            node = node->next;
            }
            return temp;
        }
        // Prefix Decrement: --a
        basic_iterator& operator--() {
            // TODO
            // cout<<"pre--"<<endl;
            if(node->prev!=nullptr){
            node = node->prev;
            }
            return *this;
        }
        // Postfix Decrement: a--
        basic_iterator operator--(int) {
            // TODO
            // cout<<"post--"<<endl;
            basic_iterator temp = *this;
            if(node->prev!=nullptr){
                node = node->prev;
            }
            return temp;
        }

        bool operator==(const basic_iterator& other) const noexcept {
            // TODO
            // cout<<"=="<<endl;
            return node == other.node;
        }
        bool operator!=(const basic_iterator& other) const noexcept {
            // TODO
            // cout<<"!="<<endl;
            return node != other.node;
        }
    };

public:
    using value_type      = T;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using iterator        = basic_iterator<pointer, reference>;
    using const_iterator  = basic_iterator<const_pointer, const_reference>;

private:
    Node head, tail;
    size_type _size;

public:
    List(): head(Node()), tail(Node()), _size(0) {
        // TODO - Don't forget the list initialier
        head.next = &tail;
        tail.prev = &head;
    }
    List( size_type count, const T& value ): head(Node()), tail(Node()), _size(0){
        // TODO - Don't forget the list initialier
        head.next = &tail;
        tail.prev = &head;

        for(size_type i = 0; i<count; i++)
        {
            // Node* current = new Node(value);

            push_back(value);
            // current->prev = tail.prev;
            // current->next = &tail;
            // tail.prev->next = current;
            // tail.prev = current;
        }

        // _size = count;
    }
    explicit List( size_type count ): head(Node()), tail(Node()), _size(0) {
        // TODO - Don't forget the list initialier
        head.next = &tail;
        tail.prev = &head;

        for(size_type i = 0; i<count; i++)
        {
            // Node* current = new Node();
            push_back(T());
            // current->prev = tail.prev;
            // current->next = &tail;
            // tail.prev->next = current;
            // tail.prev = current;
        }

        // _size = count;
    }
    List( const List& other ): head(Node()), tail(Node()), _size(0) {
        // TODO - Don't forget the list initialier
        if(!other.empty())
        {
            head.next = &tail;
            tail.prev = &head;
            Node* currentOther = other.head.next;

            while(currentOther != &other.tail)
            {
                push_back(currentOther->data);

                currentOther = currentOther->next;
                // _size++;
            }
            // _size = other._size
        }
    }
    List( List&& other ){
        // TODO - Don't forget the list initialier
        // this->clear();
        if(!other.empty())
        {
            head = other.head;
            tail = other.tail;
             _size = other._size;

            other.head.next->prev = &head;
            other.tail.prev->next = &tail;
        }
        else
        {
            head.next = &tail;
            tail.prev = &head;
            _size = 0;
            return;
        }
        // std::cout << _size << " " << other._size << std::endl;
        other.head = Node();
        other.tail = Node();
        other.head.next = &other.tail;
        other.tail.prev = &other.head;
        other._size = 0;
    }
    ~List() {
        // TODO
        Node* current = head.next;

        while (current != &tail){
            Node* next = current->next;

            current->next = nullptr;
            current->prev = nullptr;
            // current->data = T();
            // current->data.~T();
            delete current;

            current = next;
        }

        head.next = &tail;
        tail.prev = &head;
        _size = 0;
    }
    List& operator=( const List& other ) {
        // TODO
        if(other.empty())
        {
            clear();
            head.next = &tail;
            tail.prev = &head;
        }
        if(this!=&other)
        {
            clear();
            for(const auto& item : other)
            {
                push_back(item);
            }
        }
        // other.head = Node();
        // other.tail = Node();
        // other.head.next = &other.tail;
        // other.tail.prev = &other.head;
        // other._size = 0;
        return *this;
    }
    List& operator=( List&& other ) noexcept {
        // TODO
        if(this!=&other)
        {
            clear();
            if(!other.empty())
            {
            head = move(other.head);
            tail = move(other.tail);
            _size = other._size;
            //  for(const auto& item : other)
            // {
            //     push_back(item);
            // }
            other.head.next->prev = &head;
            other.tail.prev->next = &tail;
            }
            else
            {
                head.next = &tail;
                tail.prev = &head;
                _size = 0;
            }

            other.head = Node();
            other.tail = Node();
            other.head.next = &other.tail;
            other.tail.prev = &other.head;
            other._size = 0;
        }
        return *this;
    }

    reference front() {
        // TODO
        if(empty())
        {
            throw out_of_range("List is empty");
        }
        return head.next->data;
    }
    const_reference front() const {
        // TODO
        if(empty())
        {
            throw out_of_range("List is empty");
        }
        return head.next->data;
    }
	
    reference back() {
        // TODO
        if(empty())
        {
            throw out_of_range("List is empty");
        }
        return tail.prev->data;
    }
    const_reference back() const {
        // TODO
        if(empty())
        {
            throw out_of_range("List is empty");
        }
        return tail.prev->data;
    }
	
    iterator begin() noexcept {
        // TODO
        // if(empty())
        // {
        //     return iterator(&tail);
        // }
        return iterator(head.next);
    }
    const_iterator begin() const noexcept {
        // TODO
        // if(empty())
        // {
        //     return iterator(&tail);
        // }
        return const_iterator(head.next);
    }
    const_iterator cbegin() const noexcept {
        // TODO
        // if(empty())
        // {
        //     return const_iterator(&tail);
        // }
        return const_iterator(head.next);
    }

    iterator end() noexcept {
        // TODO
        return iterator(&tail);
    }
    const_iterator end() const noexcept {
        // TODO
        return const_iterator(&tail);
    }
    const_iterator cend() const noexcept {
        // TODO
        return const_iterator(&tail);
    }

    bool empty() const noexcept {
        // TODO
        if(_size == 0)
        {
            return true;
        }
        return false;
    }

    size_type size() const noexcept {
        // TODO
        return _size;
    }

    void clear() noexcept {
        // TODO
        Node* current = head.next;

        while(current != &tail){
            Node* next = current->next;
            delete current;
            current = next;
        }

        head.next = &tail;
        tail.prev = &head;
        _size = 0;
    }

    iterator insert( const_iterator pos, const T& value ) {
        // TODO
        Node* newNode = new Node(value);

        newNode->next = pos.node;
        newNode->prev = pos.node->prev;
        pos.node->prev->next = newNode;
        pos.node->prev = newNode;

        _size++;

        return iterator(newNode);
    }
    iterator insert( const_iterator pos, T&& value ) {
        // TODO
        Node* newNode = new Node(move(value));

        newNode->next = pos.node;
        newNode->prev = pos.node->prev;
        pos.node->prev->next = newNode;
        pos.node->prev = newNode;

        _size++;

        return iterator(newNode);
    }

    iterator erase( const_iterator pos ) {
        // TODO
        Node* nextNode = pos.node->next;
        pos.node->prev->next = pos.node->next;
        pos.node->next->prev = pos.node->prev;

        delete pos.node;
        _size--;

        return iterator(nextNode);
    }

    void push_back( const T& value ) {
        // TODO
        Node* newNode = new Node(value);

        newNode->next = &tail;
        newNode->prev = tail.prev;
        tail.prev->next = newNode;
        tail.prev = newNode;

        _size++;
    }
    void push_back( T&& value ) {
        // TODO
        Node* newNode = new Node(move(value));

        newNode->next = &tail;
        newNode->prev = tail.prev;
        tail.prev->next = newNode;
        tail.prev = newNode;

        _size++;
    }

    void pop_back() {
        // TODO
        if(empty())
        {
            throw out_of_range("List is empty");
        }

        Node* lastNode = tail.prev;
        tail.prev = lastNode->prev;
        lastNode->prev->next = &tail;
        delete lastNode;
        _size--;
    }
	
    void push_front( const T& value ) {
        // TODO
        Node* newNode = new Node(value);

        newNode->next = head.next;
        newNode->prev = &head;
        head.next->prev = newNode;
        head.next = newNode;

        _size++;
    }
	void push_front( T&& value ) {
        // TODO
        Node* newNode = new Node(move(value));

        newNode->next = head.next;
        newNode->prev = &head;
        head.next->prev = newNode;
        head.next = newNode;

        _size++;
    }

    void pop_front() {
        // TODO
        if(empty())
        {
            throw out_of_range("List is empty");
        }

        Node* lastNode = head.next;
        head.next = lastNode->next;
        lastNode->next->prev = &head;
        delete lastNode;
        _size--;
    }

    /*
      You do not need to modify these methods!
      
      These method provide the non-const complement 
      for the const_iterator methods provided above.
    */
    iterator insert( iterator pos, const T & value) { 
        return insert((const_iterator &) (pos), value);
    }

    iterator insert( iterator pos, T && value ) {
        return insert((const_iterator &) (pos), std::move(value));
    }

    iterator erase( iterator pos ) {
        return erase((const_iterator&)(pos));
    }
};


/*
    You do not need to modify these methods!

    These method provide a overload to compare const and 
    non-const iterators safely.
*/
 
namespace {
    template<typename Iter, typename ConstIter, typename T>
    using enable_for_list_iters = typename std::enable_if<
        std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::iterator, 
            Iter
        >{} && std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::const_iterator,
            ConstIter
        >{}, T>::type;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) == rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) == lhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) != rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) != lhs;
}