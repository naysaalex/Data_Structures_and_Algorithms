#include <cstddef>    // size_t
#include <functional> // std::hash
#include <ios>
#include <utility>    // std::pair
#include <iostream>

#include "primes.h"



template <typename Key, typename T, typename Hash = std::hash<Key>, typename Pred = std::equal_to<Key>>
class UnorderedMap {
    public:

    using key_type = Key;
    using mapped_type = T;
    using const_mapped_type = const T;
    using hasher = Hash;
    using key_equal = Pred;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    private:

    struct HashNode {
        HashNode *next;
        value_type val;

        HashNode(HashNode *next = nullptr) : next{next} {}
        HashNode(const value_type & val, HashNode * next = nullptr) : next { next }, val { val } { }
        HashNode(value_type && val, HashNode * next = nullptr) : next { next }, val { std::move(val) } { }
    };

    size_type _bucket_count;
    HashNode **_buckets;

    HashNode * _head;
    size_type _size;

    Hash _hash;
    key_equal _equal;

    static size_type _range_hash(size_type hash_code, size_type bucket_count) {
        return hash_code % bucket_count;
    }

    public:

    template <typename pointer_type, typename reference_type, typename _value_type>
    class basic_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = _value_type;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

    private:
        friend class UnorderedMap<Key, T, Hash, key_equal>;
        using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

        const UnorderedMap * _map;
        HashNode * _ptr;

        explicit basic_iterator(UnorderedMap const * map, HashNode *ptr) noexcept: _map(map), _ptr(ptr) { /* TODO */ }

    public:
        basic_iterator(): _map(nullptr), _ptr(nullptr) { /* TODO */ };

        basic_iterator(const basic_iterator &) = default;
        basic_iterator(basic_iterator &&) = default;
        ~basic_iterator() = default;
        basic_iterator &operator=(const basic_iterator &) = default;
        basic_iterator &operator=(basic_iterator &&) = default;
        reference operator*() const { 
            /* TODO */ 
            // if(_ptr != nullptr)
                return _ptr->val;
            // else
            //     throw std::out_of_range("Trying to dereference a null pointer.");
        }
        pointer operator->() const { 
            /* TODO */
            // if(_ptr != nullptr)
                return &_ptr->val; 
            // else
            //     throw std::out_of_range("Trying to dereference a null pointer.");
        }
        basic_iterator &operator++() { 
            /* TODO */ 

            if (_ptr != nullptr) {
                if (_ptr->next != nullptr) {
                    _ptr = _ptr->next;
                } else {
                    size_type current_bucket = _map->_bucket(_ptr->val);
                    while (++current_bucket < _map->_bucket_count) {
                        if (_map->_buckets[current_bucket] != nullptr) {
                            _ptr = _map->_buckets[current_bucket];
                            break;
                        }
                    }
                    if (current_bucket >= _map->_bucket_count) {
                        _ptr = nullptr;
                    }
                }
            }
            return *this;
        }
        basic_iterator operator++(int) { 
            /* TODO */ 
            basic_iterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator==(const basic_iterator &other) const noexcept { 
            /* TODO */ 
            return _ptr == other._ptr;
        }
        bool operator!=(const basic_iterator &other) const noexcept { 
            /* TODO */ 
            return _ptr != other._ptr;
        }
    };

    using iterator = basic_iterator<pointer, reference, value_type>;
    using const_iterator = basic_iterator<const_pointer, const_reference, const value_type>;

    class local_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::pair<const key_type, mapped_type>;
            using difference_type = ptrdiff_t;
            using pointer = value_type *;
            using reference = value_type &;

        private:
            friend class UnorderedMap<Key, T, Hash, key_equal>;
            using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

            HashNode * _node;

            explicit local_iterator( HashNode * node ) noexcept : _node(node) { /* TODO */ }

        public:
            local_iterator(){ /* TODO */
            _node = nullptr;
             }

            local_iterator(const local_iterator &) = default;
            local_iterator(local_iterator &&) = default;
            ~local_iterator() = default;
            local_iterator &operator=(const local_iterator &) = default;
            local_iterator &operator=(local_iterator &&) = default;
            reference operator*() const { 
                /* TODO */ 
                return _node->val;
            }
            pointer operator->() const { 
                /* TODO */ 
                return &_node->val;
            }
            local_iterator & operator++() { 
                /* TODO */ 
                // if(_node != nullptr)
                // {
                    _node = _node->next;
                // }
                return *this;
            }
            local_iterator operator++(int) { 
                /* TODO */ 
                local_iterator temp = *this;
                _node = _node->next;
                return temp;
            }

            bool operator==(const local_iterator &other) const noexcept { 
                /* TODO */ 
                return _node == other._node;
            }
            bool operator!=(const local_iterator &other) const noexcept { 
                /* TODO */ 
                return _node != other._node;
            }
    };

private:

    size_type _bucket(size_t code) const { 
        /* TODO */
        return code % _bucket_count;
    }
    size_type _bucket(const Key & key) const { 
        /* TODO */ 
        size_type hash_code = _hash(key);
        return _bucket(hash_code);
    }
    size_type _bucket(const value_type & val) const { 
        /* TODO */ 
        const Key& key = val.first;
        return _bucket(key);
    }

    HashNode*& _find(size_type code, size_type bucket, const Key & key) { 
        /* TODO */ 
        HashNode** current = &_buckets[bucket];

        while(*current != nullptr)
        {
            if(_equal((*current)->val.first, key))
            {
                return *current;
            }
            current = &(*current)->next;
        }
        return *current;
    }

    HashNode*& _find(const Key & key) 
    { 
        /* TODO */ 
        size_type code = _hash(key);
        size_type bucket = _bucket(key);
        return _find(code, bucket, key);
    }

    HashNode * _insert_into_bucket(size_type bucket, value_type && value) 
    { /* TODO */ 
        _buckets[bucket] = new HashNode(std::move(value), _buckets[bucket]);
        //check if global head needst o be updated
        // if _bucket(_head) greater than _bucket(new node we just inserted) we should update head
        // if(_head == nullptr || _bucket(_head->val) >= bucket)
        // {
        //     // std::cout<<"head updated"<<std::endl;
        //     _head = _buckets[bucket];
        // }

        ++_size;
        return _buckets[bucket];
    }

    void _move_content(UnorderedMap & src, UnorderedMap & dst) 
    { 
        /* TODO */ 
        dst._bucket_count = src._bucket_count;
        dst._buckets = src._buckets;
        dst._head = src._head;
        dst._head = src._head;
        dst._size = src._size;

        src._bucket_count = 0;
        src._buckets = nullptr;
        src._head = nullptr;
        src._size = 0;
    }

public:
    explicit UnorderedMap(size_type bucket_count, const Hash & hash = Hash { },
                const key_equal & equal = key_equal { }) : _bucket_count(next_greater_prime(bucket_count)), _hash(hash), _equal(equal) { 
                    /* TODO */ 
                _buckets = new HashNode*[_bucket_count];
                _head = nullptr;
                _size = 0;

                for (size_type i = 0; i < _bucket_count; ++i) {
                    _buckets[i] = nullptr;
                }
    }

    ~UnorderedMap() { 
        /* TODO */ 
        clear();
        delete[] _buckets;
    }

UnorderedMap(const UnorderedMap & other): _bucket_count(other._bucket_count), _hash(other._hash), _equal(other._equal) { 
        /* TODO */ 
        _buckets = new HashNode*[_bucket_count]();
        _head = nullptr;
        _size = 0;

        for (size_type i = 0; i < other._bucket_count; ++i) {
            HashNode* current = other._buckets[i];
            while(current != nullptr)
            {
                insert(current->val);
                // if(current->next != nullptr)
                // {
                current = current->next;
                // }
            }
        }
    }

    UnorderedMap(UnorderedMap && other): _hash(std::move(other._hash)), _equal(std::move(other._equal)), _size(other._size){ 
        /* TODO */ 
        _bucket_count = (other._bucket_count);
        // std::cout<<"bucket count: "<<_bucket_count<<std::endl;
        _buckets = new HashNode*[_bucket_count];
        // _head = other._head;
        // _buckets = std::move(other._buckets);

        for (std::size_t i = 0; i < other._bucket_count; ++i) {
            // std::cout << "Bucket " << i << ": ";
            // HashNode*& current = other._buckets[i];
            _buckets[i] = other._buckets[i];
            other._buckets[i] = nullptr;
        }

        // other._head = nullptr;
        other._size = 0;
    }

    UnorderedMap & operator=(const UnorderedMap & other) { 
        /* TODO */ 
        if (this != &other) {
            clear();
            if(_buckets)
                delete[] _buckets;
            
            _bucket_count = other._bucket_count;
            _buckets = new HashNode*[_bucket_count]();
            _hash = other._hash;
            _equal = other._equal;
            _head = nullptr;
            _size = 0;

            for(size_type i = 0; i<other._bucket_count;++i)
            {
                HashNode* current = other._buckets[i];
                while(current)
                {
                    insert(current->val);
                    current = current->next;  
                }
            }
        }
        return *this;
    }

    UnorderedMap & operator=(UnorderedMap && other) { 
        /* TODO */
        if (this != &other) {
            clear();
            if(_buckets)
                delete[] _buckets;

            _size = other._size;
            _hash = std::move(other._hash);
            _equal = std::move(other._equal);
            _bucket_count = other._bucket_count;
            _buckets = new HashNode*[_bucket_count]();
             

        for (std::size_t i = 0; i < other._bucket_count; ++i) {
            // std::cout << "Bucket " << i << ": ";
            // HashNode*& current = other._buckets[i];
            _buckets[i] = other._buckets[i];
            other._buckets[i] = nullptr;
        }
        other._size = 0;

        // other._head = nullptr;
           
        }
        return *this; 
    }

    void clear() noexcept 
    { 
        /* TODO */ 
        for (size_type i = 0; i < _bucket_count; ++i) {
            HashNode*& current = _buckets[i];
            while (current) {
                HashNode*& next = current->next;
                delete current;
                current = next;
            }
            _buckets[i] = nullptr;
        }

        _bucket_count = 0;
        // _buckets = new HashNode*[_bucket_count]();
        
        _head = nullptr;
        _size = 0;
    }

    size_type size() const noexcept { 
        /* TODO */ 
        return _size;
    }

    bool empty() const noexcept { 
        /* TODO */ 
        if(_size == 0)
        {
            return true;
        }
        return false;
    }

    size_type bucket_count() const noexcept { 
        /* TODO */ 
        return _bucket_count;
    }
iterator begin() {
    size_type bucket = 0;
    while (bucket < _bucket_count && _buckets[bucket] == nullptr) {
      bucket++;
    }
    if(bucket< _bucket_count)
        return iterator(this, _buckets[bucket]);

    return end();
  }
  iterator end() { return iterator(this, nullptr); }

  const_iterator cbegin() const {
    size_type bucket = 0;
    while (bucket < _bucket_count && _buckets[bucket] == nullptr) {
      bucket++;
    }
    if(bucket< _bucket_count)
        return const_iterator(this, _buckets[bucket]);

    return end();
  };
    const_iterator cend() const { /* TODO */ 
        return const_iterator(this, nullptr);
    };

    local_iterator begin(size_type n) { /* TODO */ 
        return local_iterator(_buckets[n]);
    }
    local_iterator end(size_type n) { /* TODO */ 
        return local_iterator(nullptr);
    }

    size_type bucket_size(size_type n) { /* TODO */ 
        if (n >= _bucket_count || _buckets[n] == nullptr) {
            return 0;
        }

        size_type count = 0;
        HashNode* current = _buckets[n];
        while (current != nullptr) {
            count++;
            current = current->next;
        }

        return count;
    }

    float load_factor() const { /* TODO */ 
        return static_cast<float>(_size) / static_cast<float>(_bucket_count);
    }

    size_type bucket(const Key & key) const { /* TODO */ 
        size_type hash_code = _hash(key);
        return _bucket(hash_code);
    }

    std::pair<iterator, bool> insert(value_type && value) { /* TODO */ 
        size_type hash_code = _hash(value.first);
        size_type bucket = _bucket(hash_code);
        // std::cout<<"1"<<std::endl;
        HashNode* found = _find(hash_code, bucket, value.first);
        // std::cout<<"1"<<std::endl;
        if (found == nullptr) {
            iterator temp_it(this, _insert_into_bucket(bucket, std::move(value)));
            return std::make_pair(temp_it, true);
        }
        else
        {
            iterator not_found(this, found);
            return std::make_pair(not_found, false);
        }
    }

    std::pair<iterator, bool> insert(const value_type & value) { /* TODO */ 
        size_type hash_code = _hash(value.first);
        size_type bucket = _bucket(hash_code);

        HashNode* found = _find(hash_code, bucket, value.first);

        if (found == nullptr) {
            // std::cout << "couldn't find the key" << std::endl;
            iterator temp_it(this, _insert_into_bucket(bucket, value_type(value)));
            return std::make_pair(temp_it, true);
        }
        else
        {
            // std::cout << "find the key" << std::endl;
            iterator not_found(this, found);
            return std::make_pair(not_found, false);
        }
    }

    iterator find(const Key & key) { /* TODO */ 
        size_type hash_code = _hash(key);
        size_type bucket = _bucket(hash_code);

        HashNode* found = _find(hash_code, bucket, key);

        if (found != nullptr) {
            return iterator(this, found);
        }

        return end();
    }

    T& operator[](const Key & key) { /* TODO */ 
        size_type hash_code = _hash(key);
        size_type bucket = _bucket(hash_code);

        HashNode* found = _find(hash_code, bucket, key);

        if (found == nullptr) {
            // value_type value_to_insert(key, T{}); 
            found = _insert_into_bucket(bucket, std::make_pair(key, T{}) );
            // return found->val.second;
        }

        return found->val.second;
    }


    iterator erase(iterator pos) { /* TODO */ 
        size_type bucket = _bucket(pos._ptr->val);
        local_iterator it = begin(bucket);

        std::cout<<"1"<<std::endl;
        if(_equal(it._node->val.first, pos._ptr->val.first))
        {
            std::cout<<"inside"<<std::endl;
            HashNode* current = it._node;
            iterator temp_it = iterator(this, current);
            ++temp_it;
            _buckets[bucket] = current->next;
            delete current;
            _size--;
            return temp_it;
        }
        std::cout<<"check"<<std::endl;
        while (it._node->next != pos._ptr) {
            std::cout<<"0"<<std::endl;
            it++;
        }

        std::cout<<"2"<<std::endl;
        if(it._node != nullptr)
        {
            std::cout<<"3"<<std::endl;
            HashNode* current = it._node->next;
            it._node->next = current->next;
            // HashNode*& returnVal = it._node->next;
            delete current;
            _size--;
            std::cout<<"4"<<std::endl;
            return ++iterator(this,it._node);
        }
        return ++pos;
    }

    size_type erase(const Key & key) { /* TODO */ 
        size_type hash_code = _hash(key);
        size_type bucket = _bucket(hash_code);

        HashNode* &found = _find(hash_code, bucket, key);

        if (found != nullptr) {
            size_type count = 1;
            HashNode* to_remove = found;
            found = found->next;
            delete to_remove;
            _size--;
            return count;
        }

        return 0;
    }

    template<typename KK, typename VV>
    friend void print_map(const UnorderedMap<KK, VV> & map, std::ostream & os);
};

template<typename K, typename V>
void print_map(const UnorderedMap<K, V> & map, std::ostream & os = std::cout) {
    using size_type = typename UnorderedMap<K, V>::size_type;
    using HashNode = typename UnorderedMap<K, V>::HashNode;

    for(size_type bucket = 0; bucket < map.bucket_count(); bucket++) {
        os << bucket << ": ";

        HashNode const * node = map._buckets[bucket];

        while(node) {
            os << "(" << node->val.first << ", " << node->val.second << ") ";
            node = node->next;
        }

        os << std::endl;
    }
}