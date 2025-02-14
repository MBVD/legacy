#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

template<typename T>
class Hasher {
public:
  static const int _r = 256;
  Hasher(int key_range = 10000, int collision_level = 3) : _key_range(key_range), _collision_level(collision_level) {
    _p = _key_range / _collision_level;
    int array[] = {7, 19, 47, 97, 241, 499, 997, 2477, 4999, 9973};
    for (auto k : array) {
      if (_p < k) {
        _p = k;
        break;
      }
    }
  }

  long long get_hash(const T& s) const {
    return text_to_num(s) % _p;
  }

  int get_key_range() const {
    return _key_range;
  }

private:
  long long _p;
  int _key_range;
  int _collision_level;

  ll text_to_num(const T& s) const {
    ll result = 0;
    int i = 0, n = s.size();
    for (const auto c : s) {
      result += c * pow(_r, n - 1 - i);
      ++i;
    }
    return result;
  }
};

template<typename KeyType, typename ValueType>
class HashTable {
public:
  HashTable(int capacity = 10000) : _capacity(capacity), _hasher(capacity) {
    _table.resize(_capacity);
  }

  HashTable(const HashTable& ht1) : _hasher(ht1._hasher), _capacity(ht1._capacity) {
    _table.resize(_capacity);
    _table = ht1._table;
  }

  HashTable& operator=(const HashTable& rhs) {
    if (this != &rhs) {
      _capacity = rhs._capacity;
      _table.clear();
      _table.resize(_capacity);
      _table = rhs._table;
      _hasher = rhs._hasher;
    }
      return *this;
  }

  void insert(const KeyType& key, const ValueType& value) {
    int index = hashFunction(key);
    _table[index].push_front(make_pair(key, value));
  }

  bool remove(const KeyType& key) {
    int index = hashFunction(key);
    for (auto it = _table[index].begin(); it != _table[index].end(); ++it) {
      if (it->first == key) {
        _table[index].erase(it);
        return true;
      }
    }
    return false;
  }

  friend bool operator==(const HashTable& lhs, const HashTable& rhs) {
    if (lhs._capacity != rhs._capacity) {
      return false;
    }
    for (int i = 0; i < lhs._capacity; i++) {
      if (lhs._table != rhs._table) {
        return false;
      }
    }
    return true;
  }

  std::pair<const KeyType, ValueType> operator[](const KeyType& key) const {
    int index = hashFunction(key);
    for (const auto& it : _table[index]) {
      if (it.first == key) {
        return it;
      }
    }
    throw std::out_of_range("Key not found");
  }

  ValueType& search(const KeyType& key) {
    int index = hashFunction(key);
    for (auto& it : _table[index]) {
      if (it.first == key) {
        return it.second;
      }
    }
    throw std::out_of_range("Key not found");
  }

  const ValueType& search(const KeyType& key) const {
    int index = hashFunction(key);
    for (const auto& it : _table[index]) {
      if (it.first == key) {
        return it.second;
      }
    }
    throw std::out_of_range("Key not found");
  }

  void clear() {
    _table.clear();
    _table.resize(_capacity);
  }

private:
  std::vector<std::list<std::pair<KeyType, ValueType>>> _table;
  Hasher<std::string> _hasher;
  int _capacity;

  int hashFunction(const KeyType& key) const {
    return _hasher.get_hash(key);
  }
};

template<typename KeyType, typename ValueType>
class Map {
public:
  class MapIterator;

  Map() : _hash_table(10000), _size(0) {}


  ValueType& operator[](const KeyType& key) {
    try {
      return _hash_table.search(key);
    } catch (const std::out_of_range&) {
      ValueType v{};
      _hash_table.insert(key, v);
      _keys.push_back(key);
      return _hash_table.search(key);
    }
  }

  void clear() {
    _size = 0;
    _hash_table.clear();
    _keys.clear();
  }

  Map& operator=(const Map& rhs) {
    if (this != &rhs) {
        _size = rhs._size;
        _keys = rhs._keys;
        _hash_table = rhs._hash_table;
    }
    return *this;
  }

  void insert(const KeyType& key, const ValueType& value) {
    _hash_table.insert(key, value);
    _keys.push_back(key);
    _size++;
  }

  void erase(const KeyType& key) {
    if (_hash_table.remove(key)) {
      auto it = std::find(_keys.begin(), _keys.end(), key);
      if (it != _keys.end()) {
        _keys.erase(it);
        _size--;
      }
    }
  }

  int size() const {
    return _size;
  }

  bool is_empty() const {
    return _size == 0;
  }

  MapIterator begin() {
    return MapIterator(_hash_table, _keys, 0);
  }

  MapIterator end() {
    return MapIterator(_hash_table, _keys, _keys.size());
  }

  bool contains(const KeyType& key) const {
    try {
      _hash_table.search(key);
      return true;
    } catch (const std::out_of_range&) {
      return false;
    }
  }

private:
  HashTable<KeyType, ValueType> _hash_table;
  std::vector<KeyType> _keys;
  int _size;
};

template<typename KeyType, typename ValueType>
class Map<KeyType, ValueType>::MapIterator {
public:
  MapIterator(HashTable<KeyType, ValueType>& hash_table, const std::vector<KeyType>& keys, int pos)
    : _hash_table(hash_table), _keys(keys), _pos(pos) {}

  std::pair<const KeyType, ValueType> operator*() const {
    KeyType key = _keys[_pos];
    return _hash_table[key];
  }

  MapIterator& operator++() {
    _pos++;
    return *this;
  }

  MapIterator operator++(int) {
    MapIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  bool operator==(const MapIterator& mp) const {
    return _pos == mp._pos && _hash_table == mp._hash_table;
  }

  bool operator!=(const MapIterator& mp) const {
    return !(*this == mp);
  }

private:
  HashTable<KeyType, ValueType>& _hash_table;
  std::vector<KeyType> _keys;
  int _pos;
};

int main() {
  Map<std::string, int> mp;
  mp.insert("a", 1);
  mp.insert("b", 2);
  mp.clear();
  mp.insert("c", 2);

  std::cout << mp["c"] << std::endl;

  for (auto it : mp) {
    std::cout << it.first << " " << it.second << "\n";
  }
}
