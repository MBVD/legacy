#include <iostream>


template <typename T>
class Matrix{ 
private:
    T** _matr;
    int _m, _n;

public:
    Matrix(T** arr, int n, int m) : _n(n), _m(m) { /*[m][n]*/
        _matr = new T*[n];
        for (int i = 0; i < n; ++i) {
            _matr[i] = new T[m];
            for (int j = 0; j < m; ++j) {
                _matr[i][j] = arr[i][j];
            }
        }
    }
    Matrix(const Matrix& other) : _n(other._n), _m(other._m) {  /*copy*/
        _matr = new T*[_n];
        for (int i = 0; i < _n; ++i) {
            _matr[i] = new T[_m];
            for (int j = 0; j < _m; ++j) {
                _matr[i][j] = other._matr[i][j];
            }
        }
    }

    Matrix& operator= (const  Matrix& rhs){
        _n = rhs._n;
        _m = rhs._m;

       _matr = new T*[_n];
            for (int i = 0; i < _n; ++i) {
                _matr[i] = new T[_m];
                for (int j = 0; j < _m; ++j) {
                    _matr[i][j] = rhs._matr[i][j];
                }
            }
        
        return *this;
    }


    void gauss(){
      if (_n != _m){
        std::cout<<"Error.\n";
        return;
      }
      for (int i = 0; i < _n; i++){
        T del = _array[i][i];
        for (int j = 0; j < _m; j++){
          _array[i][j] /= del;
        }
        for (int j = i+1; j <_ n; j++){
          T koef = _array[j][i];
          for (int k = 0; k<_m; k++){
            _array[j][k] -= _array[i][k] * koef;
          }
        }
      }
    }

    T det(){
      gauss();
      T det = 1;
      for(int i = 0; i < _n; i++){
        det *=  _array[i][i];
      }
      return det;
    }
    
    Matrix& input(int n, int m){
        T** arr[n][m];
        for (int i = 0; i < n; i++){
            for (int j = 0; j < m; j++){    
                std::cin>>arr[i][j];
            }
        }
        Matrix tmp(arr, n, m);
        return tmp;
    }

    ~Matrix() {
        for (int i = 0; i < _n; ++i) {
            delete[] _matr[i];
        delete[] _matr;
        }


    }

    Matrix& operator+= (const Matrix& rhs){
        if (_n != rhs._n || _m != rhs._m){
            return NULL;
        }
        for (int i = 0; i < _n; i++){
                for (int j = 0; j < _m; j++){
                    _matr[i][j] += rhs._matr[i][j];
                }
            }
    }


   Matrix& operator*=(const T alpha) {
        for (int i = 0; i < _n; ++i) {
            for (int j = 0; j < _m; ++j) {
                _matr[i][j] *= alpha;
            }
        }
        return *this;
    }

    Matrix& operator-= (const  Matrix& rhs){
        if (_n != rhs._n || _m != rhs._m){
                return NULL;
            }
        for (int i = 0; i < _n; i++){
            for (int j = 0; j < _m; j++){
                _matr[i][j] -= rhs._matr[i][j];
            }
        }
    }

    Matrix& operator*= (const Matrix& rhs) {
        *this = (*this * rhs);
        return *this;
    }


    Matrix& operator+ (const  Matrix& rhs){
        Matrix tmp = rhs;
        tmp += rhs;
        return tmp;
   }
   
    Matrix& operator- (const  Matrix& rhs){
        Matrix tmp = rhs;
        tmp -= rhs;
        return tmp;
    }

    Matrix operator* (const Matrix& rhs) const {
        if (_m != rhs._n) {
            return NULL;
        }
        Matrix result(_n, rhs._m);
        for (int i = 0; i < _n; ++i) {
            for (int j = 0; j < rhs._m; ++j) {
                for (int k = 0; k < _m; ++k) {
                    result._matr[i][j] += _matr[i][k] * rhs._matr[k][j];
                }
            }
        }
        return result;
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


   
    Matrix transpose() const {
        Matrix result(_m, _n);
        for (int i = 0; i < _m; ++i) {
            for (int j = 0; j < _n; ++j) {
                result._matr[i][j] = _matr[j][i];
            }
        }
        return result;
    }


class Iterator {
    private:
        Matrix& _matrix;
        int _row, _col;
    public:
       Iterator(Matrix& matrix, int row, int col) : _matrix(matrix), _row(row), _col(col) {}

       Iterator& operator++() {
            if (++_col >= _matrix._m) {
                _col = 0;
                ++_row;
            }
            return *this;
        }

        Iterator& operator--() {
            if (--_col < 0) {
                _col = _matrix._m - 1;
                --_row;
            }
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return _row != other._row || _col != other._col;
        }

        T& operator*() const { //back ssylka
            return _matrix._matr[_row][_col];
        }
    };

    Iterator begin() {
        return Iterator(*this, 0, 0);
    }

    Iterator end() {
        return Iterator(*this, _n, 0);
    }
};


