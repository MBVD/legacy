#pragma once


template<typename T>
class list {
public:
    struct NodeBase;
    struct ListNode;
    struct HeadNode;
    struct ListIterator;
    struct ListConstIterator;


    using value_type = T;
    using size_type = std::size_t; 
    using reference = value_type&;
    using const_reference = const value_type;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = ListIterator;
    using const_iterator = ListConstIterator;

    list() = default;
    list(const list& other) {
        for (const auto& elem : other) {
            _insert(end(), elem);
        }
    }

    list(list&& other) noexcept {
        swap(other);
    }

    list(std::initializer_list<T> init) {
        for (const auto& elem : init) {
            _insert(end(), elem);
        }
    }

    ~list() noexcept {
        clear();
    }

    list& operator=(const list& other) {
        if (this != &other) {
            clear();
            list tmp(other);
            swap(tmp);
        }
        return *this;
    }

    list& operator=(list&& other) {
        if (this != &other) {
            clear();
            swap(other);
        }
        return *this;
    }

    iterator begin() {
        return iterator(_head._next);
    }

    const_iterator begin() const {
        return const_iterator(_head._next);
    }

    const_iterator cbegin() const {
        return const_iterator(_head._next);
    }

    iterator end() {
        return iterator(&_head);
    }

    const_iterator end() const {
        return const_iterator(&_head);
    }

    const_iterator cend() const {
        return const_iterator(&_head);
    }

    void clear() noexcept {
        while (_head->_next != _head) {
            ListNode* temp = dynamic_cast<ListNode*>(_head->_next);
            _head->_next = _head->_next->_next;
            delete temp;
        }
    }

    void swap(list& other) noexcept {
        auto temp = _head;
        _head = other._head;
        other._head = temp;
    }

    void push_back(const T& value) {
        _insert(end(), value);
    }

private:

    void _insert(iterator pos, const T& value) {
        ListNode* temp = new ListNode(value);
        auto prev_it = 
    }

    HeadNode _head;
};

template<typename T>
struct list<T>::NodeBase {
    NodeBase *_next = nullptr, *_prev = nullptr;
};

template<typename T>
struct list<T>::ListNode : public NodeBase {
    T _data;

    ListNode(const T& data) : _data(data) {}
    ListNode(T&& data) : _data(std::move(data)) { _data = T{}; }
};

template<typename T>
struct list<T>::HeadNode : public NodeBase {

    std::size_t _size = 0;

    HeadNode() {
        init();
    }

    void init() {
        this->_next = this->_prev = *this;
    }
};

template<typename T>
class list<T>::ListIterator {
public:
    using value_type = T;
    using difference_type = std::ptrdiff_t; 
    using reference = value_type&;
    using pointer = value_type*;
    using iterator_category = std::bidirectional_iterator_tag;


    using Node = list<T>::Node;
    using Self = ListIterator;

    explicit ListIterator(Node *ptr = nullptr) noexcept : _ptr(ptr) {}

    reference operator*() const {
        return _ptr->_data;
    }

    pointer operator->() const {
        return &(_ptr->_data);
    }

    Self& operator++() { //prefix
        _ptr = _ptr->_next;
        return *this;
    }

    Self operator++(int) { //postfix
        Self temp = *this;
         _ptr = _ptr->_next;
        return temp;       
    }

    Self& operator--() {
        _ptr = _ptr->_prev;
        return *this;
    }

    Self operator--(int) {
        Self temp = *this;
         _ptr = _ptr->_prev;
        return temp;       
    }

    friend bool operator==(const Self& lhs, const Self& rhs) {
        return lhs._ptr == rhs._ptr;
    }

    friend bool operator!=(const Self& lhs, const Self& rhs) {
        return lhs._ptr != rhs._ptr;
    }
private:
    Node *_ptr;
};



template<typename T>
class list<T>::ListConstIterator {
public:
    using value_type = T;
    using difference_type = std::ptrdiff_t; 
    using reference = const value_type&;
    using pointer = const value_type*;
    using iterator_category = std::bidirectional_iterator_tag;


    using Node = list<T>::Node;
    using Self = ListConstIterator;

    explicit ListConstIterator(Node *ptr = nullptr) noexcept : _ptr(ptr) {}

    reference operator*() const noexcept {
        return _ptr->_data;
    }

    pointer operator->() const noexcept {
        return &(_ptr->_data);
    }

    Self& operator++() noexcept {
        _ptr = _ptr->_next;
        return *this;
    }

    Self operator++(int) noexcept {
        Self temp = *this;
         _ptr = _ptr->_next;
        return temp;       
    }

    Self& operator--() noexcept {
        _ptr = _ptr->_prev;
        return *this;
    }

    Self operator--(int) noexcept{
        Self temp = *this;
         _ptr = _ptr->_prev;
        return temp;       
    }

    friend bool operator==(const Self& lhs, const Self& rhs) noexcept {
        return lhs._ptr == rhs._ptr;
    }

    friend bool operator!=(const Self& lhs, const Self& rhs) noexcept {
        return lhs._ptr != rhs._ptr;
    }
private:
    const Node *_ptr;
};
