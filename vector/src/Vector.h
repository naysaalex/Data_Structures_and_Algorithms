#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm> // std::random_access_iterator_tag
#include <cstddef> // size_t
#include <stdexcept> // std::out_of_range
#include <type_traits> // std::is_same
#include <iostream>

using namespace std;

template <class T>
class Vector {
public:
    class iterator;
private:
    T* array;
    size_t _capacity, _size;

    // You may want to write a function that grows the vector
    void grow() { 
        // cout<<"grow"<<endl;
        if(_capacity == 0)
        {
            _capacity++;
        }
        else {
            _capacity*=2;
        }

        T* new_arr = new T[capacity()];

        for(size_t i=0; i<size(); i++)
        {
            new_arr[i] = move(array[i]);
        }

        delete[] array;
        array = new_arr;
     }

public:
    //create an empty vector
    Vector() noexcept : _capacity(0), _size(0), array(nullptr){ }

    //constructs the container with count copies of elements with value
    //count is the size of the vector to create
    //value is the element to fill the vector with
    Vector(size_t count, const T& value) : _capacity(count), _size(count), array(new T[count]) { 
        for(size_t i=0; i<_size;i++)
        {
            array[i] = value;
        }
    }

    //constructs the container with count default-inserted instances of T. No copies are made
    //count is the size of the vector to create
    explicit Vector(size_t count) : _capacity(count), _size(count), array(new T[count]) 
    {
        for (size_t i = 0; i < _size; i++) {
            array[i] = T(); // Default-initialize each element
        }
    }

    //constructs the container with the copy of the contents of other
    Vector(const Vector& other) : _capacity(other._capacity), _size(other._size), array(new T[other._capacity]) { 
        copy(other.array, other.array + _size, array);
     }

    //constructs the container with the contents of other using move semantics. 
    //after the move, other is guaranteed to be empty()
    //other is the vector to move the contents of into a new vector. this
    //should be left empty after this function
    Vector(Vector&& other) noexcept :array(other.array), _capacity(other._capacity), _size(other._size) { 
        other.array = nullptr;
        other._capacity = 0;
        other._size = 0;
     }

    //destructs the vector. the destructor of the elements are called, and
    //the used storage is deallocated. note that if the elements are pointers,
    //the pointed-to-objects are not destroyed
    ~Vector() { 
        delete[] array; 
    }

    //replaces the contents with a copy of the contents of other
    Vector& operator=(const Vector& other) { 
        if(this == &other)
        {
            return *this;
        }

        delete[] array;

        _capacity = other._capacity;
        _size = other._size;
        array = new T[_capacity];


        for (size_t i = 0; i < _size; i++) 
        {
            array[i] = other.array[i];
        }

        return *this;
     }

    //replaces the contents with those of other using move semantics (i.e.
    //the data in other is moved from other into this container). after the move, 
    //other is guaranteed to be empty()
    Vector& operator=(Vector&& other) noexcept {
        if (this == &other)
        {
            return *this;
        }

        delete[] array;
        _capacity = other._capacity;
        _size = other._size;
        array = other.array;
        other.array = nullptr;
        other._capacity = 0;
        other._size = 0;
        return *this;
    }

    //gets an iterator to the first element in the container. if the container is empty, this should be equivalent to end()
    iterator begin() noexcept { 
        return iterator(array);
     }

    //gets an iterator past the last element in the container. if the container is
    //empty, this should be equivalent to begin()
    iterator end() noexcept { 
        return iterator(array+_size);
     }

    //checks whether the container is empty
    [[nodiscard]] bool empty() const noexcept { 
        if (_size == 0) {
            return true;
        }
        return false;
     }

    //returns the number of elements in the vector
    size_t size() const noexcept { 
        return _size;
     }

    //returns the number of elements that can be held in currently allocated storage
    size_t capacity() const noexcept { 
        return _capacity;
     }

    //access element at index pos with bounds checking. throws 
    //std::out_of_range if pos is out of bounds
    //pos is the index to access
    T& at(size_t pos) { 
        if (pos>=_size)
        {
            throw out_of_range("Index out of bounds");
        }
        return array[pos];
     }

    //access element at index pos with bounds checking. Throws
    //std::out_of_range if pos is out of bounds
    const T& at(size_t pos) const { 
        if (pos>=_size)
        {
            throw out_of_range("Index out of bounds");
        }
        return array[pos];
     }

    //acccess element at index pos without bounds checking
    T& operator[](size_t pos) { 
        return array[pos];
     }

    //access element at index pos without bounds checking
    const T& operator[](size_t pos) const { 
        return array[pos];
     }

    //access the first element and returns a reference
    //to the first element in the vector
    T& front() { 
        if(empty())
        {
            throw out_of_range("Vector is empty");
        }
        return array[0];
     }

    //access the first element and returns a const reference to the
    //first element in the vector
    const T& front() const { 
        if(empty())
        {
            throw out_of_range("Vector is empty");
        }
        return array[0];
    }

    //access the last element and returns a reference
    //to the last eleemnt in the vector
    T& back() { 
        if(empty())
        {
            throw out_of_range("Vector is empty");
        }
        return array[_size -1];
     }

    //access the last element and returns a const reference
    //to the last element in the vector
    const T& back() const { 
        if(empty())
        {
            throw out_of_range("Vector is empty");
        }
        return array[_size -1];
     }


    //adds value to the end of the vector
    void push_back(const T& value) {
        // cout<<"push"<<endl;
        if (_size >= _capacity) {
        // If the size equals capacity, double the capacity
            grow();
        }
        array[_size] = value;
        _size++;
    }

    void push_back(T&& value) {
        // cout<<"back"<<endl;
        if (_size >= _capacity) {
        // If the size equals capacity, double the capacity
            grow();
        }
        array[_size] = move(value);
        _size++;
    }


     void pop_back() { 
        if (_size > 0) {
            _size--;
            if (_size < _capacity / 2) {
                // If the size becomes significantly smaller than capacity, you can shrink the capacity.
                size_t newCapacity = _capacity / 2;
                if (newCapacity < 1) {
                    newCapacity = 1;
                }
                T* newArray = new T[newCapacity];

                for (size_t i = 0; i < _size; i++) {
                    newArray[i] = move(array[i]);
                }

                delete[] array;
                _capacity = newCapacity;
                array = newArray;
            }
        }
    }
    
    //inserts value at position pos and iterator pointing to the 
    //inserted element 
    iterator insert(iterator pos, const T& value) { 
        size_t index = pos - begin();

        if(_size >=_capacity)
        {
            grow();
        }
        
        if(index > _size)
        {
            index = _size;
        }

        for (size_t i = _size; i > index; --i) 
        {
            array[i] = move(array[i - 1]);
        }

        array[index] = move(value);
        _size++;
        return begin()+index;
     }

    //inserts value at position pos and returns an iterator
    //pointing to the inserted element
    iterator insert(iterator pos, T&& value) { 
        size_t index = pos - begin();
        
        if(_size >=_capacity)
        {
            grow();
        }
        
        if(index > _size)
        {
            index = _size;
        }

        for (size_t i = _size; i > index; --i) 
        {
            array[i] = move(array[i - 1]);
        }

        array[index] = move(value);
        _size++;
        return begin()+index;
     }

    //inserts count copies of value at position pos and returns
    //an iterator pointing to the first element inserted
    iterator insert(iterator pos, size_t count, const T& value) {
        if (count == 0) {
            return pos;
        }

        size_t index = pos - begin();  
        size_t requiredCapacity = _size + count;
        if (requiredCapacity > _capacity) {
            while (_capacity < requiredCapacity) {
                grow();
            }
        }

        _capacity = requiredCapacity;
        for (size_t i = _size; i > index; --i) {
            array[i + count - 1] = array[i - 1];
        }

        for (size_t i = 0; i < count; ++i) {
            array[index + i] = value;
        }

        _size += count;
        return begin() + index;
    }


    //erases element at position pos and returns an iterator pointing
    //to the element following the one that was erased. if the
    //last element was erased, should return end()
    iterator erase(iterator pos) {
        if (empty() || pos == end()) {
            throw out_of_range("Vector is empty or iterator is at end()");
        }

        size_t index = pos - begin();
        if (index >= _size) {
            throw out_of_range("Iterator is out of bounds");
        }

        if (pos == end() - 1) {
            // If erasing the last element, no need to shift elements
            _size--;
            return end();
        }

        // Use std::move to efficiently shift elements
        for (size_t i = index; i < _size - 1; ++i) {
            array[i] = move(array[i + 1]);
        }

        _size--;
        return begin() + index;
    }

    //erases elements in the range [first,last], including first and
    //exclusing last and returns an iterator pointing to the element following
    //the last erased element. if the last element was erased, should return end()
    iterator erase(iterator first, iterator last) {
        if (empty()) {
            throw out_of_range("Vector is empty");
        }

        size_t firstIndex = first - begin();
        size_t lastIndex = last - begin();
        if (firstIndex >= _size || lastIndex > _size || firstIndex > lastIndex) {
            throw invalid_argument("Iterator(s) are out of bounds or invalid range");
        }

        size_t numElementsToErase = lastIndex - firstIndex;

        // Use std::move to efficiently shift elements
        for (size_t i = firstIndex; i + numElementsToErase < _size; ++i) {
            array[i] = move(array[i + numElementsToErase]);
        }

        _size -= numElementsToErase;

        // Check if the last element was erased
        if (lastIndex == _size) {
            return end();
        }

        return begin() + firstIndex;
    }




    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    private:
        // Add your own data members here
        // HINT: For random_access_iterator, the data member is a pointer 99.9% of the time
        pointer current;
    public:
        iterator() : current(nullptr) { }
        // Add any constructors that you may need
        explicit iterator(pointer ptr) : current(ptr) {} 

        // This assignment operator is done for you, please do not add more
        iterator& operator=(const iterator&) noexcept = default;

         [[nodiscard]] reference operator*() const noexcept { 
            return *current; 
        }

        // This is used to access data members of the underlying elements
        // It should return the address of the element the iterator points to
        [[nodiscard]] pointer operator->() const noexcept { 
            return current;
         }

        // Prefix Increment: ++a
        iterator& operator++() noexcept { 
            ++current;
             return *this;
         }

        // Postfix Increment: a++
        // The value returned should be the iterator before it was moved forward in the container
        iterator operator++(int) noexcept { 
            iterator temp = *this;
            ++current;
             return temp;
         }

        // Prefix Decrement: --a
        // The value returned should be the iterator moved backwards in the container
        iterator& operator--() noexcept { 
            --current;
             return *this;
         }

        // Postfix Decrement: a--
        // The value returned should be the iterator before it was moved backwards in the container
        iterator operator--(int) noexcept { 
            iterator temp = *this;
            --current;
             return temp;
         }

        iterator& operator+=(difference_type offset) noexcept { 
            current+=offset;
             return *this;
         }
        
        // Should return a copy of the iterator moved forward by an offset
        // NOTE: The iterator itself should not be moved, only the copy
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept { 
            iterator temp = *this;
            temp+=offset;
            return temp;
         }
        
        // Should move the iterator backwards by an offset and return the new iterator
        iterator& operator-=(difference_type offset) noexcept { 
            current -= offset;
             return *this;
         }

        // Should return a copy of the iterator moved backwards by an offset
        // NOTE: The iterator itself should not be moved, only the copy
        [[nodiscard]] iterator operator-(difference_type offset) const noexcept { 
            iterator temp = *this;
            temp -= offset;
            return temp;
         }

        // Should return the difference between two iterators. This should be the offset betweent the
        // elements in memory.
        [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept { 
            return current - rhs.current;
         }

        // Should return the element at the location offset from the iterator
        [[nodiscard]] reference operator[](difference_type offset) const noexcept { 
            return *(current+offset);
         }

        // Used for comparing iterators
        // NOTE: This should be used to compare the iterators, not the elements they point to
        // HINT: Because iterators mimic pointers, many if not all of these operations can be done
        //       on the underlying pointers
        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept { 
            return current == rhs.current;
         }
        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept { 
            return current != rhs.current;
         }
        [[nodiscard]] bool operator<(const iterator& rhs) const noexcept { 
            return current < rhs. current;
         }
        [[nodiscard]] bool operator>(const iterator& rhs) const noexcept { 
            return current> rhs.current;
         }
        [[nodiscard]] bool operator<=(const iterator& rhs) const noexcept { 
            return current <= rhs.current;
         }
        [[nodiscard]] bool operator>=(const iterator& rhs) const noexcept { 
            return current >= rhs.current;
         }
    };


    void clear() noexcept {
        delete[] array;
        // _capacity = 1;
        _size = 0;
        array = nullptr; // Reallocate a small buffer to maintain vector usability
    }
};

// This ensures at compile time that the deduced argument _Iterator is a Vector<T>::iterator
// There is no way we know of to back-substitute template <typename T> for external functions
// because it leads to a non-deduced context
namespace {
    template <typename _Iterator>
    using is_vector_iterator = std::is_same<typename Vector<typename _Iterator::value_type>::iterator, _Iterator>;
}

// Change the operator+ overload as follows:
template <typename _Iterator, bool _enable = is_vector_iterator<_Iterator>::value>
[[nodiscard]] _Iterator operator+(typename _Iterator::difference_type offset, _Iterator const& iterator) noexcept {
    // Check if the iterator is suitable for addition.
    static_assert(_enable, "Iterator is not suitable for addition.");

    // Advance the iterator by the specified offset.
    _Iterator result = iterator;
    result += offset;
    return result;
}

#endif