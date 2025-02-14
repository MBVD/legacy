#include<bits/stdc++.h>
#include <cstdlib>
template<typename T>
class Matrix{
  public:
    class MatrixIterator; 
    Matrix<T>(int n, int m, T** array) : _n(n), _m(m), _array(array) {}
    Matrix<T>(Matrix<T>& m1) : _n(m1._n), _m(m1._m) {
      _array = (T**)malloc(m1._n * sizeof(T*));
      for (int i = 0; i < m1._n; i++){
        _array[i] = (T*)malloc(sizeof(T) * m1._m);
        for (int j = 0; j<m1._m; j++){
          _array[i][j] = m1._array[i][j];
        }
      }
    }

    Matrix<T>(int n, int m) : _n(n), _m(m){
      _array = (T**)malloc(_n * sizeof(T*));
      for (int i = 0; i<_n; i++){
        _array[i] = (T*)malloc(_m * sizeof(T));
      }
    }

    Matrix<T>() : _n(0), _m(0), _array(nullptr){}

    ~Matrix(){
      for (int i = 0; i < _n; i++) {
        free(_array[i]);
      }
      free(_array);
    }

    MatrixIterator begin() {
      return MatrixIterator(_array, _n, _m, 0, 0);
    }
    MatrixIterator end() {
      return MatrixIterator(_array, _n, _m, _n, _m);
    }

    Matrix& operator+= (const Matrix& rhs){
      if (_n != rhs._n || _m != rhs._m) {
        std::cout<<"different sizes";
      }
      for (int i = 0; i < _n; i++) {
        for (int j = 0; j < _m; j++) {
          _array[i][j] += rhs._array[i][j];
        }
      }
      return *this;
    }

    Matrix& operator-= (const Matrix& rhs){
      if (_n != rhs._n || _m != rhs._m) {
        std::cout << "different sizes";
      }
      for (int i = 0; i < _n; i++) {
        for (int j = 0; j < _m; j++) {
          _array[i][j] += rhs._array[i][j];
        }
      }
      return *this;
    }

    Matrix& operator*= (const Matrix& rhs){
      if (_m != rhs._n){
        std::cout << "different sizes";
        return *this;
      }
      Matrix result(_n, rhs._m);
      for (int i = 0; i < _n; i++){
        for (int j = 0; j < rhs._m; j++){
          T sm = 0;
          for (int k = 0; k < _m; k++){
            sm += _array[i][k] * rhs._array[k][j];
          }
          result._array[i][j] = sm;
        }
      }
      for (int i = 0; i < _n; i++){
        free(_array[i]);
      }
      free(_array);
      _array = (T**)malloc(_n * sizeof(T*));
      for (int i = 0; i < _n; i++){
        _array[i] = (T*)malloc(rhs._m * sizeof(T));
        for (int j = 0; j < rhs._m; j++){
          _array[i][j] = result._array[i][j];
        }
      }
      _m = rhs._m;
      return *this;
  }

    Matrix& operator*= (const T& rhs){
      for (int i = 0; i<_n; i++){
        for (int j = 0; j<_m; j++){
          _array[i][j] *= rhs;
        }
      }
      return *this;
    }

    Matrix& operator= (const Matrix& rhs){
     _n = rhs._n;
     _m = rhs._m;
     _array = (T**)realloc(_array, _n * sizeof(T*));
     for (int i = 0; i<_n; i++){
      _array[i] = (T*)realloc(_array[i], _m * sizeof(T));
      for (int j = 0; j<_m; j++){
        _array[i][j] = rhs._array[i][j];
      }
     }
     return *this; 
    }

    void gauss(){
      if (_n != _m){
        std::cout<<"not correct sizes \n";
        return;
      }
      for (int i = 0; i<_n; i++){
        T del = _array[i][i];
        for (int j = 0; j<_m; j++){
          _array[i][j] /= del;
        }
        for (int j = i+1; j<_n; j++){
          T coefficient = _array[j][i];
          for (int k = 0; k<_m; k++){
            _array[j][k] -= _array[i][k] * coefficient;
          }
        }
      }
    }

    T det(){
      gauss();
      T ans = 1;
      for(int i = 0; i<_n; i++){
        ans*= _array[i][i];
      }
      return ans;
    }

    friend void transposition (Matrix& m){
      Matrix tmp(m._m, m._n);
      for (int i = 0; i<m._m; i++){
        for (int j = 0; j<m._n; j++){
          tmp._array[i][j] = m._array[j][i];
        }
      }
      m = tmp;
    }

    friend void inverse(Matrix& m) {
      if (m._n != m._m) {
        std::cout<<"sizes icorrect \n";
        return;
      }
      Matrix inverse(m._n, m._n);
      for (int i = 0; i < m._n; i++) {
        inverse._array[i][i] = 1;
      }

      for (int i = 0; i < m._n; i++) {
        T del = m._array[i][i];
        for (int j = 0; j < m._n; j++) {
          m._array[i][j] /= del;
          inverse._array[i][j] /= del;
        }
        for (int j = 0; j < m._n; j++) {
          if (i != j) {
            T coefficient = m._array[j][i];
            for (int k = 0; k < m._n; k++) {
              m._array[j][k] -= m._array[i][k] * coefficient;
              inverse._array[j][k] -= inverse._array[i][k] * coefficient;
            }
          }
        }
      }   
      m = inverse;
    }

    friend Matrix operator+ (const Matrix lhs, const Matrix rhs){
      Matrix tmp = lhs;
      tmp += rhs;
      return tmp;
    }

    friend Matrix operator- (const Matrix lhs, const Matrix rhs){
      Matrix tmp = lhs;
      tmp -= rhs;
      return tmp;
    }

    friend Matrix operator* (const Matrix lhs, const Matrix rhs){
      Matrix tmp = lhs;
      tmp *= rhs;
      return tmp;
    }

    friend Matrix operator* (const Matrix lhs, const T rhs){
      Matrix tmp = lhs;
      tmp *= rhs;
      return tmp;
    }

    friend Matrix operator* (const T lhs, Matrix rhs){
      Matrix tmp = rhs;
      tmp *= lhs;
      return tmp;
    }

    friend Matrix operator/ (const Matrix lhs, const T rhs){
      Matrix tmp(lhs._n, lhs._m);
      for (int i = 0; i<lhs._n; i++){
        for (int j =0; j<lhs._m; j++){
          tmp._array[i][j] /= rhs;
        }
      }
      return tmp;
    }

    friend std::ostream& operator<<(std::ostream& out, const Matrix& m){
      for (int i = 0; i<m._n; i++){
        for (int j = 0; j<m._m; j++){
          out<<m._array[i][j]<<" ";
        }
        out<<"\n";
      }
      return out;
    }

    // friend std::istream& operator>>(std::istream& in, Matrix& m){
    //   in >> m._n >> m._m;
    //   m._array = (T**)malloc(sizeof(T*)  * m._n);
    //   for (int i = 0; i<m._n; i++){
    //     m._array[i] = (T*)malloc(sizeof(T) * m._m);
    //     for (int j = 0; j<m._m; j++){
    //       in>>m._array[i][j];
    //     }
    //   }
    //   return in;
    // }
    

  private:
    int _n, _m;
    T** _array;
};

template<typename T>
class Matrix<T>::MatrixIterator {
private:
    T** _array;
    int _n, _m;
    int _row, _col;

public:
    MatrixIterator(T** array, int n, int m, int row = 0, int col = 0)
        : _array(array), _n(n), _m(m), _row(row), _col(col) {}

    T& operator*() const {
        return _array[_row][_col];
    }

    MatrixIterator& operator++() {
        _col++;
        if (_col == _m) {
            _col = 0;
            _row++;
        }
        return *this;
    }

    MatrixIterator operator++(int) {
        MatrixIterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const MatrixIterator& other) const {
        return _row == other._row && _col == other._col;
    }
    bool operator!=(const MatrixIterator& other) const {
        return !(*this == other);
    }
};

template<typename T>
T** cin_array(int n, int m){
  T** tmp = (T**)malloc(n * sizeof(T*));
  for (int i = 0; i<n; i++){
    tmp[i] = (T*)malloc(m * sizeof(T));
    for (int j = 0; j<m; j++){
      std::cin>>tmp[i][j];
    }
  }
  return tmp;
}
class Tester{
  public:
    static void test_proizv(){
      int n, m;
      std::cin>>n>>m;
      double** arr = cin_array<double>(n, m);
      std::cout<<std::endl;
      Matrix m1(n, m, arr);
      std::cout<<m1<<std::endl;
      std::cin>>n>>m;
      double **arr1 = cin_array<double>(n, m);
      Matrix m2(n, m, arr1);
      std::cout<<m2<<std::endl;
      m1 *= m2;
      std::cout<<"proizvedenye \n"<<m1;
    } 
    static void test_gauss(){
      int n, m;
      std::cin>>n>>m;
      double** arr = cin_array<double>(n, m);
      std::cout<<std::endl;
      Matrix m1(n, m, arr);
      m1.gauss();
      std::cout<<m1;
    }
    static void test_det(){
      int n, m;
      std::cin>>n>>m;
      double** arr = cin_array<double>(n, m);
      std::cout<<std::endl;
      Matrix m1(n, m, arr);
      std::cout<<m1.det();
    }

    static void test_iterator() {
      int n, m;
      std::cin >> n >> m;
      double** arr = cin_array<double>(n, m);
      std::cout << std::endl;
      Matrix m1(n, m, arr);
      for (auto it = m1.begin(); it != m1.end(); ++it) {
        std::cout << *it << " ";
      }
      std::cout << std::endl;
    }


};

int main(){
  Tester::test_iterator();
}