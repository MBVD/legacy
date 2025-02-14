#include<bits/stdc++.h>

enum Color {
  Red, Black
};

template<typename KeyType, typename ValueType>
struct node {
  struct node *left, *right, *parent;
  Color color;
  KeyType key;
  ValueType value;
  node(node* left = nullptr, node* right = nullptr, node* parent = nullptr, Color color = Black, KeyType key = KeyType(), ValueType value = ValueType())
    : left(left), right(right), parent(parent), color(color), key(key), value(value) {}
};

template <typename KeyType, typename ValueType>
class RBTree{
public:
  RBTree(node<KeyType, ValueType>* head = nullptr) : _head(head){}

  node<KeyType, ValueType>* get_root(){
    return _head;
  }

  void insert (KeyType key, ValueType value){
    if (_head == nullptr){
      _head = new node<KeyType, ValueType>(nullptr, nullptr, nullptr, Black, key, value);
      return;
    }
    in(_head, nullptr, key, value);
  }

  void erase(KeyType key){
    node<KeyType, ValueType>* z = nullptr;
    try{
      z = findNode(_head, key);
    } catch (std::runtime_error){
      return;
    }
    node<KeyType, ValueType>* y = z;
    Color y_original_color = y->color;
    node<KeyType, ValueType>* x;
    if (z->left == nullptr){
      x = z->right;
      replace(z, z->right);
    }else if (z->right == nullptr){
      x = z->left;
      replace(z, z->left);
    } else{
      y = minRightTree(z->right);
      y_original_color = y->color;
      x = y->right;
      if (y->parent == z){
        if (x) x->parent = y;
      } else {
        replace(y, y->right);
        y->right = z->right;
        y->right->parent = y;
      }
      replace(z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
    }
    delete(z);
    if (y_original_color == Black){
      deletionFixup(x);
    }
  }

  const ValueType& find (KeyType key){
    try{
      auto tmp = findNode(_head, key);
      return tmp -> value;
    } catch (std::runtime_error){
      return ValueType{};
    }
  }

private:
  node<KeyType, ValueType>* _head;
  void in(node<KeyType, ValueType>* root, node<KeyType, ValueType>* parent, KeyType key, ValueType value){
    if (root == nullptr){
      node<KeyType, ValueType>* newNode = new node<KeyType, ValueType>(nullptr, nullptr, parent, Red, key, value);
      if (parent == nullptr) {
        _head = newNode;
      } else if (key > parent->key) {
        parent->right = newNode;
      } else {
        parent->left = newNode;
      }
      insertFixup(newNode);
      return;
    }
    if (key > root->key){
      in(root->right, root, key, value);
    }else if (key < root->key){
      in(root->left, root, key, value);
    }
  }

  void insertFixup(node<KeyType, ValueType>* node){
    while (node != _head && node->parent->color == Red) {
      if (node->parent == node->parent->parent->left) {
        auto* uncle = node->parent->parent->right;
        if (uncle != nullptr && uncle->color == Red) {
          node->parent->color = Black;
          uncle->color = Black;
          node->parent->parent->color = Red;
          node = node->parent->parent;
        } else {
          if (node == node->parent->right) {
            node = node->parent;
            rotateLeft(node);
          }
          node->parent->color = Black;
          node->parent->parent->color = Red;
          rotateRight(node->parent->parent);
        }
      } else {
        auto* uncle = node->parent->parent->left;
        if (uncle != nullptr && uncle->color == Red) {
          node->parent->color = Black;
          uncle->color = Black;
          node->parent->parent->color = Red;
          node = node->parent->parent;
        } else {
          if (node == node->parent->left) {
            node = node->parent;
            rotateRight(node);
          }
          node->parent->color = Black;
          node->parent->parent->color = Red;
          rotateLeft(node->parent->parent);
        }
      }
    }
    _head->color = Black;
  }

  void rotateRight(node<KeyType, ValueType>*& node){
    auto *tmp = node->left;
    node->left = tmp->right;
    if (tmp->right != nullptr)
      tmp->right->parent = node;
    tmp->parent = node->parent;
    if (node->parent == nullptr)
      _head = tmp;
    else if (node == node->parent->right)
      node->parent->right = tmp;
    else
      node->parent->left = tmp;
    tmp->right = node;
    node->parent = tmp;
  }

  void rotateLeft (node<KeyType, ValueType>*& node){
    auto *tmp = node->right;
    node->right = tmp->left;
    if (tmp->left != nullptr)
      tmp->left->parent = node;
    tmp->parent = node->parent;
    if (node->parent == nullptr)
      _head = tmp;
    else if (node == node->parent->left)
      node->parent->left = tmp;
    else  
      node->parent->right = tmp;
    tmp->left = node;
    node->parent = tmp;
  }

  node<KeyType, ValueType>* findNode(node<KeyType, ValueType>* node, KeyType key){
    if (node == nullptr){
      throw std::runtime_error("element not found");
    }
    if (node->key == key){
      return node;
    }
    if (node->key > key){
      return findNode(node->left, key);
    }else if (node->key < key){
      return findNode(node->right, key);
    }
  }

  void replace(node<KeyType, ValueType>* u, node<KeyType, ValueType>* v) {
    if (u->parent == nullptr) {
      _head = v;
    } else if (u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
    if (v != nullptr) {
      v->parent = u->parent;
    }
  }

  node<KeyType, ValueType>* minRightTree(node<KeyType, ValueType>* node){
    auto* tmp = node;
    while (tmp->left != nullptr){
      tmp = tmp->left;
    }
    return tmp;
  }

  void deletionFixup(node<KeyType, ValueType>* x) {
    while (x != _head && x->color == Black) {
      node<KeyType, ValueType>* w;
      if (x == x->parent->left) {
        w = x->parent->right;
        if (w->color == Red) {
          w->color = Black;
          x->parent->color = Red;
          rotateLeft(x->parent);
          w = x->parent->right;
        }
        if (w->left->color == Black && w->right->color == Black) {
          w->color = Red;
          x = x->parent;
        } else {
          if (w->right->color == Black) {
            w->left->color = Black;
            w->color = Red;
            rotateRight(w);
            w = x->parent->right;
          }
          w->color = x->parent->color;
          x->parent->color = Black;
          w->right->color = Black;
          rotateLeft(x->parent);
          x = _head;
        }
      } else {
        w = x->parent->left;
        if (w->color == Red) {
          w->color = Black;
          x->parent->color = Red;
          rotateRight(x->parent);
          w = x->parent->left;
        }
        if (w->right->color == Black && w->left->color == Black) {
          w->color = Red;
          x = x->parent;
        } else {
          if (w->left->color == Black) {
            w->right->color = Black;
            w->color = Red;
            rotateLeft(w);
            w = x->parent->left;
          }
          w->color = x->parent->color;
          x->parent->color = Black;
          w->left->color = Black;
          rotateRight(x->parent);
          x = _head;
        }
      }
    }
    if (x != nullptr){
      x->color = Black;
    }
  }
};

template<typename KeyType, typename ValueType>
class OrderedMap {
public:
  class MapIterator;
  OrderedMap() : _tree(){}

  void insert(KeyType key, ValueType value){
    _tree.insert(key, value);
  }

  void erase(KeyType key){
    _tree.erase(key);
  }

  const ValueType& operator[] (KeyType key){
    return _tree.find(key);
  }

  MapIterator begin(){
    auto tmp = _tree.get_root();
    while (tmp -> left != nullptr){
      tmp = tmp -> left;
    }
    return MapIterator(tmp);
  }

  MapIterator end(){
    return MapIterator(nullptr);
  }

private:
  RBTree<KeyType, ValueType> _tree;
};

template<typename KeyType, typename ValueType>
class OrderedMap<KeyType, ValueType>::MapIterator{
public:
  MapIterator(node<KeyType, ValueType>* root) : _root(root) {}

  std::pair<KeyType, ValueType> operator* () const{
    return std::make_pair(_root->key, _root->value);
  } 

  MapIterator& operator++ (){
    if (_root -> right == nullptr){
      _root = _root-> parent;
    }else{
      _root = _root -> right;
    }
    return *this;
  }

  bool operator== (const MapIterator& mp) const{
    return _root == mp._root; 
  }

  bool operator!= (const MapIterator& mp) const {
    return !(*this == mp);
  }
private:
  node<KeyType, ValueType>* _root;
};

int main(){
  OrderedMap<std::string, int> map;
  map.insert("hello", 1);
  map.insert("alo", 2);
  for (auto el : map){// for (auto el = map.begin(); el != map.end(); el++)
    std::cout<<el.first<<" "<<el.second<<"\n";
  }
}