#include <iostream>
#include <map>
#include <initializer_list>
#include <stdexcept>

template<typename K, typename V>
class RBTree {
public:
    struct TreeNodeBase;
    struct TreeNode;
    struct TreeRoot;
    struct TreeIterator;
    struct TreeConstIterator;

    RBTree() : _root() {}

 
    RBTree(const RBTree& other) : _root() {
        if (other._root._root != nullptr) {
            _root._root = copyTree(other._root._root, nullptr);
            _root._count = other._root._count;
        }
    }



    RBTree& operator=(const RBTree& other) {
        if (this != &other) {
            clear();
            if (other._root._root != nullptr) {
                _root._root = copyTree(other._root._root, nullptr);
                _root._count = other._root._count;
            } else {
                _root._root = nullptr;
                _root._count = 0;
            }
        }
        return *this;
    }


    RBTree(RBTree&& other) noexcept : _root(std::move(other._root)) {
        other._root._root = nullptr;
        other._root._count = 0;
    }

    RBTree& operator=(RBTree&& other) noexcept {
        if (this != &other) {
            clear();
            _root = std::move(other._root);
            other._root._root = nullptr;
            other._root._count = 0;
        }
        return *this;
    }

    ~RBTree() {
        clear();
    }

    using pair = std::pair<K, V>;

    void insert(const pair& data) {
        TreeNode* newNode = new TreeNode(data.first, data.second);
        TreeNodeBase* parent = nullptr;
        TreeNodeBase* current = _root._root;

        while (current != nullptr) {
            parent = current;
            if (newNode->_data.first < static_cast<TreeNode*>(current)->_data.first) {
                current = current->_left;
            } else {
                current = current->_right;
            }
        }

        newNode->_parent = parent;
        if (parent == nullptr) {
            _root._root = newNode;
        } else if (newNode->_data.first < static_cast<TreeNode*>(parent)->_data.first) {
            parent->_left = newNode;
        } else {
            parent->_right = newNode;
        }
        ++_root._count;
    }

    TreeNodeBase* copyTree(const TreeNodeBase* node, TreeNodeBase* parent) {
        if (node == nullptr) {
            return nullptr;
        }

        TreeNode* newNode = new TreeNode(static_cast<const TreeNode*>(node)->_data.first, static_cast<const TreeNode*>(node)->_data.second);
        newNode->_parent = parent;
        newNode->_left = copyTree(node->_left, newNode);
        newNode->_right = copyTree(node->_right, newNode);
        return newNode;
    }

    void clear() {
        clearTree(_root._root);
        _root._root = nullptr;
        _root._count = 0;
    }

    std::size_t size() const {
        return _root._count;
    }

    bool is_empty() const {
        return _root._count == 0;
    }

    TreeIterator begin() {
        TreeNodeBase* node = _root._root;
        if (node != nullptr) {
            while (node->_left != nullptr) {
                node = node->_left;
            }
        }
        return TreeIterator(node);
    }

    TreeIterator end() {
        return TreeIterator(nullptr);
    }

    TreeConstIterator cbegin() const {
        const TreeNodeBase* node = _root._root;
        if (node != nullptr) {
            while (node->_left != nullptr) {
                node = node->_left;
            }
        }
        return TreeConstIterator(node);
    }

    TreeConstIterator cend() const {
        return TreeConstIterator(nullptr);
    }

    void erase(const K& key) {
        TreeNodeBase* node = findNode(key);
        if (node == nullptr) {
            return;
        }

        if (node->_left == nullptr && node->_right == nullptr) {
            transplant(node, nullptr);
        } else if (node->_left == nullptr) {
            transplant(node, node->_right);
        } else if (node->_right == nullptr) {
            transplant(node, node->_left);
        } else {
            TreeNodeBase* heir = minimum(node->_right);
            if (heir->_parent != node) {
                transplant(heir, heir->_right);
                heir->_right = node->_right;
                heir->_right->_parent = heir;
            }
            transplant(node, heir);
            heir->_left = node->_left;
            heir->_left->_parent = heir;
        }
        delete static_cast<TreeNode*>(node);
        --_root._count;
    }

      void clearTree(TreeNodeBase* node) {
        if (node != nullptr) {
            clearTree(node->_left);
            clearTree(node->_right);
            delete static_cast<TreeNode*>(node);
        }
    }

    void transplant(TreeNodeBase* u, TreeNodeBase* v) {
        if (u->_parent == nullptr) {
            _root._root = v;
        } else if (u == u->_parent->_left) {
            u->_parent->_left = v;
        } else {
            u->_parent->_right = v;
        }
        if (v != nullptr) {
            v->_parent = u->_parent;
        }
    }

    TreeNodeBase* minimum(TreeNodeBase* node) const {
        while (node->_left != nullptr) {
            node = node->_left;
        }
        return node;
    }

    TreeNodeBase* findNode(const K& key) const {
        TreeNodeBase* current = _root._root;
        while (current != nullptr) {
            if (key < static_cast<TreeNode*>(current)->_data.first) {
                current = current->_left;
            } else if (key > static_cast<TreeNode*>(current)->_data.first) {
                current = current->_right;
            } else {
                return current;
            }
        }
        return nullptr;
    }

private:
    TreeRoot _root;

  
};

template<typename K, typename V>
class map : public RBTree<K, V> {
public:
    using pair_1 = std::pair<K, V>;

    map() : _tree() {}

    map(const map& other) : _tree(other._tree) {}

    map(map&& other) noexcept : _tree(std::move(other._tree)) {}

    map(std::initializer_list<std::pair<const K, V>> init) : _tree() {
        for (const auto& pair_1 : init) {
            insert(pair_1);
        }
    }

    ~map() {}

    map& operator=(const map& other) {
        if (this != &other) {
            _tree = other._tree;
        }
        return *this;
    }

    map& operator=(map&& other) noexcept {
        if (this != &other) {
            _tree = std::move(other._tree);
        }
        return *this;
    }

    auto begin() {
        return _tree.begin();
    }

    auto end() {
        return _tree.end();
    }

    auto cbegin() const {
        return _tree.cbegin();
    }

    auto cend() const {
        return _tree.cend();
    }

    void insert(const pair_1& value) {
        return _tree.insert(value);
    }

    void erase(const K& key) {
        return _tree.erase(key);
    }

    void clear() {
        return _tree.clear();
    }

    std::size_t size() const {
        return _tree.size();
    }

    bool empty() const {
        return _tree.is_empty();
    }

    bool contains(const K& key) {
        return _tree.findNode(key) != nullptr; 
    }

    V& operator[](const K& key){
        auto* node = _tree.findNode(key);
        if(!node){
            _tree.insert(std::make_pair(key, V()));
            node = _tree.findNode(key);
        }
        return static_cast<typename RBTree<K,V>::TreeNode*>(node) -> _data.second;
    }

   
    V& at(const K& key){
        auto* node = _tree.findNode(key);
        if(!node){
            throw std::out_of_range("key not found");
        }
        return static_cast<typename RBTree<K,V>::TreeNode*>(node)->_data.second;
    }
private:
    RBTree<K, V> _tree;
};

template<typename K, typename V>
struct RBTree<K, V>::TreeNodeBase {
    TreeNodeBase *_left, *_right, *_parent;

    TreeNodeBase() : _left(nullptr), _right(nullptr), _parent(nullptr) {}
};

template<typename K, typename V>
struct RBTree<K, V>::TreeNode : public TreeNodeBase {
    std::pair<K, V> _data;

    TreeNode(K key, V value) : _data(std::make_pair(key, value)) {}
};

template<typename K, typename V>
struct RBTree<K, V>::TreeRoot : public TreeNodeBase {
    std::size_t _count;
    TreeNodeBase* _root;

    TreeRoot() : _count(0), _root(nullptr) {}
};

template<typename K, typename V>
struct RBTree<K, V>::TreeIterator : public TreeNodeBase {
    TreeNodeBase *_node;

    using pair = std::pair<K, V>;

    TreeIterator(TreeNodeBase* node = nullptr) : _node(node) {}

    bool operator==(const TreeIterator& _other) {
        return _node == _other._node;
    }

    bool operator!=(const TreeIterator& _other) {
        return _node != _other._node;
    }

    pair& operator*() {
        return static_cast<typename RBTree<K, V>::TreeNode*>(_node)->_data;
    }

    pair* operator->() {
        return &(static_cast<typename RBTree<K, V>::TreeNode*>(_node)->_data);
    }

    TreeIterator& operator++() { 
        if (_node->_right != nullptr) {
            _node = _node->_right;
            while (_node->_left != nullptr) {
                _node = _node->_left;
            }
        } else {
            TreeNodeBase* parent = _node->_parent;
            while (parent && _node == parent->_right) { 
                _node = parent;
                parent = parent->_parent;
            }
            _node = parent;
        }
        return *this;
    }

    TreeIterator operator++(int) { 
        TreeIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    TreeIterator& operator--() { 
        if (_node->_left != nullptr) {
            _node = _node->_left;
            while (_node->_right != nullptr) {
                _node = _node->_right;
            }
        } else {
            TreeNodeBase* parent = _node->_parent;
            while (parent && _node == parent->_left) { 
                _node = parent;
                parent = parent->_parent;
            }
            _node = parent;
        }
        return *this;
    }

    TreeIterator operator--(int) {
        TreeIterator tmp = *this;
        --(*this);
        return tmp;
    }
};

template<typename K, typename V>
struct RBTree<K, V>::TreeConstIterator : public TreeNodeBase {
    const TreeNodeBase *_node;

    using pair = std::pair<K, V>;

    TreeConstIterator(const TreeNodeBase* node = nullptr) : _node(node) {}

    bool operator==(const TreeConstIterator& _other) {
        return _node == _other._node;
    }

    bool operator!=(const TreeConstIterator& _other) {
        return _node != _other._node;
    }

    const pair& operator*() const {
        return static_cast<const typename RBTree<K, V>::TreeNode*>(_node)->_data;
    }

    const pair* operator->() const {
        return &(static_cast<const typename RBTree<K, V>::TreeNode*>(_node)->_data);
    }

    TreeConstIterator& operator++() { 
        if (_node->_right != nullptr) {
            _node = _node->_right;
            while (_node->_left != nullptr) {
                _node = _node->_left;
            }
        } else {
            const TreeNodeBase* parent = _node->_parent;
            while (parent && _node == parent->_right) { 
                _node = parent;
                parent = parent->_parent;
            }
            _node = parent;
        }
        return *this;
    }

    TreeConstIterator operator++(int) { 
        TreeConstIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    TreeConstIterator& operator--() { 
        if (_node->_left != nullptr) {
            _node = _node->_left;
            while (_node->_right != nullptr) {
                _node = _node->_right;
            }
        } else {
            const TreeNodeBase* parent = _node->_parent;
            while (parent && _node == parent->_left) { 
                _node = parent;
                parent = parent->_parent;
            }
            _node = parent;
        }
        return *this;
    }

    TreeConstIterator operator--(int) { 
        TreeConstIterator tmp = *this;
        --(*this);
        return tmp;
    }
};

int main() {
    map<int, std::string> myMap;

    myMap.insert({1, "one"});
    myMap.insert({2, "two"});
    myMap.insert({3, "three"});

    myMap[2];
    std::cout << myMap[2] << std::endl;
    for (const auto& pair : myMap) {
        std::cout << pair.first<< ": " << pair.second << std::endl;
    }
    return 0;
}


