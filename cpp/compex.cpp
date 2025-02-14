#include<bits/stdc++.h>

class Complex {
public:

    Complex(double re = 0, double im = 0) : _re(re), _im(im) {}

    Complex(const Complex&) = default;

    Complex& operator=(const Complex&) = default;

    ~Complex() = default;

    double& real() {
        return _re;
    }

    double real() const {
        return _re;
    }

    double& imag() {
        return _im;
    }

    double imag() const {
        return _im;
    }

    Complex conj() const {
        return Complex(_re, -_im);
    }

    double abs() const {
        return std::sqrt(_re * _re + _im * _im);
    }

    double arg() const {
        return std::atan2(_im, _re);
    }

    void print(int mode = 0) const {
        switch (mode) {
            case 1:
                std::cout << abs() << " * e ^ (i * " << arg() << ")" << std::endl; 
            case 2:
                std::cout << abs() << "* ( cos(" << arg() <<") + i * sin(" << arg() << "))" << std::endl; 
            default:
                std::cout << _re << " + i * " << _im << std::endl; 
        }
    }

    operator double() const {
        return _re;
    }

    Complex operator+() const {
        return *this;
    }

    Complex operator-() const {
        return Complex(-_re, -_im);
    }  

    Complex& operator++() {
        ++_re;
        return *this;
    }

    Complex operator++(int) {
        Complex tmp = *this;
        ++_re;
        return tmp;
    } 

    Complex& operator--() {
        --_re;
        return *this;
    }

    Complex operator--(int) {
        Complex tmp = *this;
        --_re;
        return tmp;
    }  

    friend Complex operator+(const Complex lhs, const Complex rhs) {
        return Complex(lhs._re + rhs._re, lhs._im + rhs._im);
    }

    friend Complex operator-(const Complex lhs, const Complex rhs) {
        return Complex(lhs._re - rhs._re, lhs._im - rhs._im);
    }

    friend Complex operator*(const Complex lhs, const Complex rhs) {
        return Complex(lhs._re * rhs._re -  lhs._im * rhs._im, lhs._re * rhs._im + lhs._im * rhs._re);
    }

    friend Complex operator/(const Complex lhs, const Complex rhs) {
        Complex tmp = lhs * rhs.conj();
        double rhs_abs = rhs.abs();
        rhs_abs *= rhs_abs;
        return Complex(tmp._re / rhs_abs, tmp._im / rhs_abs);
    }

    friend bool operator==(const Complex& lhs, const Complex& rhs) {
        return lhs._re == rhs._re && lhs._im == rhs._im;
    }

    friend bool operator!=(const Complex& lhs, const Complex& rhs) {
        return !(lhs == rhs);
    } 

    friend std::ostream& operator<<(std::ostream& out, const Complex& z) {
        out << z._re << " + i * " << z._im;
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Complex& z) {
        in >> z._re >> z._im;
        return in;
    }


private:
    double _re, _im;
};

const Complex i = Complex(0, 1);


int main() {   
    Complex z;
    std::cin >> z;
    std::cout << z << std::endl;

}