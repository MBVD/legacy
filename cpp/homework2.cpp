#include<bits/stdc++.h>
using namespace std;

class Matrix {
    double** _buf;
    int _height, _length;
    public:
        Matrix() : _height(0), _length(0), _buf(nullptr) {}
        Matrix(int height, int length) : _height(height), _length(length) {
            _buf = (double**)malloc(height * sizeof(double*));
            for (int i = 0; i < height; i++){
                _buf[i] = (double*)malloc(length * sizeof(double));
            }
        }
        Matrix(const Matrix& other) : _height(other._height), _length(other._length){
            _buf = (double**)malloc(_height * sizeof(double*));
            for (int i = 0; i < _height; i++){
                _buf[i] = (double*)malloc(_length * sizeof(double));
                for (int j = 0; j < _length; j++){
                    _buf[i][j] = other._buf[i][j];
                }
            }
        }
        explicit Matrix(int size) : _height(size), _length(size){
            _buf = (double**)malloc(size * sizeof(double*));
            for (int i = 0; i < size; i++){
                _buf[i] = (double*)malloc(size * sizeof(double));
            }
        }

        double* operator[] (int index) const {
            return _buf[index];
        }

        Matrix& operator() () {
            gauss();
            return *this;
        }

        Matrix& operator= (const Matrix& right){
            free_buf();
            _height = right._height;
            _length = right._length;
            _buf = (double**)malloc(_height * sizeof(double*));
            for (int i = 0; i<_height; i++){
                _buf[i] = (double*)malloc(_length * sizeof(_length));
                for (int j = 0; j<_length; j++){
                    _buf[i][j] = right[i][j];
                }
            }
            return *this;
        }

        Matrix& operator++ () { // сложение единичной матрицы
            if (_length != _height){
                return *this;
            }
            for (int i = 0; i < _height; i++){
                _buf[i][i] += 1;
            }
            return *this;
        }

        Matrix& operator+= (const Matrix& right) {
            *this = *this + right;
            return *this;
        }

        Matrix& operator-= (const Matrix& right) {
            *this = *this - right;
            return *this;
        }

        friend istream& operator>>(istream& input, Matrix& right){
            for (int i = 0; i<right._height; i++){
                for (int j = 0; j<right._length; j++){
                    input >> right[i][j];
                }
            }
            return input;
        }

        friend ostream& operator<<(ostream& out, Matrix& right){
            for (int i = 0; i<right._height; i++){
                for (int j = 0; j<right._length; j++){
                    out << right[i][j] <<" ";
                }
                out<<"\n";
            }
            return out;
        }

        friend Matrix operator+ (const Matrix& left, const Matrix& right){
            if (left._height != right._height || left._length != right._length){
                throw invalid_argument("cannot +");
            }
            Matrix ans = Matrix(left._height, left._length);
            for (int i = 0; i<left._height; i++){
                for (int j = 0; j<left._length; j++){
                    ans[i][j] = left[i][j] + right[i][j];
                }
            }
            return ans;
        }

        friend Matrix operator- (const Matrix& left, const Matrix& right){
            if (left._height != right._height || left._length != right._length){
                throw invalid_argument("cannot -");
            }
            Matrix ans = Matrix(left._height, left._length);
            for (int i = 0; i<left._height; i++){
                for (int j = 0; j<left._length; j++){
                    ans[i][j] = left[i][j] + right[i][j];
                }
            }
            return ans;
        }

        friend Matrix operator* (const Matrix& left, const Matrix& right){
            if (left._length != right._height){
                throw invalid_argument("cannot *");
            }
            Matrix ans = Matrix(left._height, right._length);
            for (int i = 0; i<left._height; i++){
                for (int j = 0; j < right._length; j++){
                    double tmp = 0;
                    for (int k = 0; k < left._length; k++){
                        tmp += left[i][k] * right[k][i];
                    }
                    ans[i][j] = tmp;
                }
            }
            return ans;
        }

        friend bool operator== (const Matrix& left, const Matrix& right){
            if (left._height != right._height || left._length != right._length) return false;
            for (int i = 0; i<left._height; i++){
                for (int j = 0; j<left._length; j++){
                    if (left[i][j] != right[i][j]) return false;
                }
            }
            return true;
        }

        friend bool operator!= (const Matrix& left, const Matrix& right){
            return !(left == right);
        }


    private:
    
        void free_buf(){
            for (int i = 0; i<_height; i++){
                free(_buf[i]);
            }
            free(_buf);
        }

        void gauss() {
            int sz = min(_height, _length);
            for (int i = 0; i<sz; i++){
                for (int j = i + 1; j < _height; j++){
                    double res = _buf[j][i] / _buf[i][i];
                    cout<<"res = "<<res<<"\n";
                    for (int k = i; k < _length; k++){
                        _buf[j][k] -= _buf[i][k] * res;
                    }
                }
            }
        }

};
int main() {
    Matrix matr = Matrix(3, 3);
    cin>>matr;
    matr();
    cout<<matr;
}