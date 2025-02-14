#include <iostream>
#include <cmath>

class Vector {
public:

    Vector() : _size(0), _storage(nullptr) {}
    explicit Vector(std::size_t count, double value = 0.) : _size(count), _storage(new double[count]) {
        for (std::size_t i = 0; i < count; ++i) {
            _storage[i] = value;
        }
    }

    Vector(const Vector& other)  : _size(other._size), _storage(new double[other._size]) {
        for (std::size_t i = 0; i < _size; ++i) {
            _storage[i] = other._storage[i];
        }       
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete [] _storage;

            _size = other._size;
            _storage = new double[_size]; 

            for (std::size_t i = 0; i < _size; ++i) {
                _storage[i] = other._storage[i];
            }
        }

        return *this; 
    }

    ~Vector() {
        delete [] _storage;
    }

    std::size_t size() const {
        return _size;
    }

    double length() const {
        return std::sqrt(*this * *this);
    }

    friend double angle(const Vector& lhs, const Vector& rhs) {
        return std::acos((lhs * rhs) / (lhs.length() * rhs.length()));
    }

    friend bool is_orthogonal(const Vector& lhs, const Vector& rhs) {
        return lhs * rhs == 0.;
    }

    friend bool is_collinear(const Vector& lhs, const Vector& rhs) {
        if (lhs._size == rhs._size) {
            double q = lhs._storage[0] / rhs._storage[0];

            for (std::size_t i = 0; i < lhs._size; ++i) {
                if (q != lhs._storage[i] / rhs._storage[i]) return false;
            }

            return true;
        }
        return false;
    }

    double& operator[](std::size_t index) {
        return _storage[index];
    }

    double operator[](std::size_t index) const {
        return _storage[index];
    }

    Vector operator+() const {
        return *this;
    }

    Vector operator-() const {
        Vector tmp = *this;
        for (std::size_t i = 0; i < _size; ++i) {
            tmp._storage[i] *= -1;
        }
        return tmp;
    }


    Vector& operator+=(const Vector& rhs) {
        if (_size == rhs._size) {

            for (std::size_t i = 0; i < _size; ++i) {
                _storage[i] += rhs._storage[i];
            }
        }

        return *this;
    }

    Vector& operator-=(const Vector& rhs) {
        if (_size == rhs._size) {
            for (std::size_t i = 0; i < _size; ++i) {
                _storage[i] -= rhs._storage[i];
            }
        }
        return *this;
    }

    Vector& operator*=(double alpha) {
        for (std::size_t i = 0; i < _size; ++i) {
            _storage[i] *= alpha;
        }

        return *this;
    }

    Vector& operator/=(double alpha) {
        for (std::size_t i = 0; i < _size; ++i) {
            _storage[i] /= alpha;
        }

        return *this;
    }

    friend Vector operator+(const Vector& lhs, const Vector& rhs) {
        Vector tmp = lhs;
        return tmp += rhs;
    }

    friend Vector operator-(const Vector& lhs, const Vector& rhs) {
        Vector tmp = lhs;
        return tmp -= rhs;
    }

    friend Vector operator*(const Vector& lhs, double alpha) {
        Vector tmp = lhs;
        return tmp *= alpha;
    }

    friend Vector operator*(double alpha, const Vector& rhs) {
        Vector tmp = rhs;
        return tmp *= alpha;
    }

    friend Vector operator/(const Vector& lhs, double alpha) {
        Vector tmp = lhs;
        return tmp /= alpha;
    }

    friend double operator*(const Vector& lhs, const Vector& rhs) {
        double sum = 0;
        if (lhs._size == rhs._size) {
            for (std::size_t i = 0; i < lhs._size; ++i) {
                sum += lhs._storage[i] * rhs._storage[i];
            }
        }
        return sum;
    }

    friend bool operator==(const Vector& lhs, const Vector& rhs) {
        if (lhs._size != rhs._size) return false;

        for (std::size_t i = 0; i < lhs._size; ++i) {
            if (lhs._storage[i] != rhs._storage[i]) return false;
        }

        return true;
    }



    friend bool operator!=(const Vector& lhs, const Vector& rhs) {
        return !(lhs == rhs);
    }

    friend std::ostream& operator<<(std::ostream& out, const Vector& v) {
        for (std::size_t i = 0; i < v._size; ++i) {
            out << v._storage[i];
            if (i != v._size - 1) {
                out << " ";
            }
        }
        return out;
    }

private:
    std::size_t _size;
    double *_storage;
};

int main() {
}
