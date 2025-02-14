#include <iostream> 

class Rational{
    private:
    int _numer; // числитель
    int _denom; // знаменатель
    int _whole; // целая

    public:

    Rational(int whole = 0, int numer = 0, int denom = 0) : _whole(whole), _numer(numer), _denom(denom) {}
    
    Rational(const Rational&) = default;
    ~Rational() = default;

    void normalize() {
        if (_denom == 0) {
            _numer = 0;
            _denom = 1;
            _whole = 0;
        } else {
            _whole += _numer / _denom;
            _numer = _numer % _denom;
            if (_numer == 0) {
                _denom = 1;
            }
        }
    }

    void unnormalize(){
        _numer = _whole * _denom;
        _denom = 0;
    }

    void swap(){
        int tmp = _denom;
        _denom = _numer + _whole * _denom;
        _numer = tmp;
        normalize();
    }

    int nod(int a, int b) {
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    void simplify() {
        if (_numer == 0) {
            _denom = 1;
            return;
        }
        int divisor = nod(abs(_numer), abs(_denom));
        _numer /= divisor;
        _denom /= divisor;

        if (_denom < 0) {
            _numer = -_numer;
            _denom = -_denom;
        }
    }

    friend std::ostream& operator<<(std::ostream& out, const Rational& rar) {
        if (rar._whole != 0) {
            out << rar._whole;
            if (rar._numer != 0) {
                out << ' ' << rar._numer << '/' << rar._denom;
            }
        } else {
            if (rar._numer != 0) {
                out << rar._numer << '/' << rar._denom;
            } else {
                out << 0;
            }
        }
        return out;
    }

    int numer() const{
        return this->_numer;
    }

    int denom() const{
        return this->_denom;
    }

    int whole() const{
        return this->_whole;
    }

    void set_whole(int whole){
        _whole = whole;
    }

    void set_num(int numer){
        _numer = numer;
        normalize();
    }

    void set_denom(int denom){
        _denom = denom;
        normalize();
    }

    friend Rational operator*(const Rational& lhs, const Rational& rhs){
        Rational tmpl(0, lhs._whole * lhs._denom + lhs._numer, lhs._denom);
        Rational tmpr(0, rhs._whole * rhs._denom + rhs._numer, rhs._denom);

        Rational result (0, tmpl._numer * tmpr._numer, tmpl._denom * tmpr._denom);

        result.normalize();
        return result;
    }
        /*Rational tmpl;
        Rational tmpr;
        tmpl._numer = (lhs._whole * lhs._denom) + lhs._numer; 
        tmpr._numer = (rhs._whole * rhs._denom) + rhs._numer; 
        tmpl._denom = lhs._denom;
        tmpr._denom = rhs._denom;
        Rational result;
        result._numer = tmpl._numer * tmpr._numer;
        result._denom = tmpl._denom * tmpr._denom;
        result.normalize();
        return result; */
    

    friend Rational operator/(const Rational& lhs, const Rational& rhs){
        Rational tmpl(0, lhs._whole * lhs._denom + lhs._numer, lhs._denom);
        Rational tmpr(0, rhs._whole * rhs._denom + rhs._numer, rhs._denom);

        tmpr.swap();

        return tmpl*tmpr;
    
       /* Rational tmpl;
        Rational tmpr;
        tmpl._numer = (lhs._whole * lhs._denom) + lhs._numer; 
        tmpr._numer = (rhs._whole * rhs._denom) + rhs._numer;
        tmpl._denom = lhs._denom;
        tmpr._denom = rhs._denom; 
        tmpr.swap();
        return tmpl*tmpr; */
    }

    friend Rational operator+(const Rational& lhs, const Rational& rhs){
        Rational tmpl(0, lhs._whole * lhs._denom + lhs._numer, lhs._denom);
        Rational tmpr(0, rhs._whole * rhs._denom + rhs._numer, rhs._denom);

        int common_denom = tmpl._denom * tmpr._denom;

        tmpl._numer = tmpl._numer * tmpr._denom;
        tmpr._numer = tmpr._numer * tmpl._denom;

        Rational result (0, tmpl._numer + tmpr._numer, common_denom);
        return result;
       
       /* Rational tmpl;
        Rational tmpr;
        tmpl._numer = (lhs._whole * lhs._denom) + lhs._numer; 
        tmpr._numer = (rhs._whole * rhs._denom) + rhs._numer;
        tmpl._denom = lhs._denom;
        tmpr._denom = rhs._denom;
        int common_denom = tmpl._denom * tmpr._denom; 
        tmpl._numer *= tmpr._denom;
        tmpr._numer *= tmpl._denom;
        Rational result;
        result._numer = tmpl._numer + tmpr._numer;
        result._denom = common_denom;
        result.normalize();
        return result; */
    }

    friend Rational operator-(const Rational& lhs, const Rational& rhs){
        Rational tmpl(0, lhs._whole * lhs._denom + lhs._numer, lhs._denom);
        Rational tmpr(0, rhs._whole * rhs._denom + rhs._numer, rhs._denom);

        int  common_denom = tmpl._denom * tmpr._denom;

        tmpl._numer = tmpl._numer* tmpr._denom;
        tmpr._numer = tmpr._numer * tmpl._denom;

        Rational result (0, tmpl._numer - tmpr._numer, common_denom);
        return result;
        /* Rational tmpl;
        Rational tmpr;
        tmpl._numer = (lhs._whole * lhs._denom) + lhs._numer; 
        tmpr._numer = (rhs._whole * rhs._denom) + rhs._numer;
        tmpl._denom = lhs._denom;
        tmpr._denom = rhs._denom;
        int common_denom = tmpl._denom * tmpr._denom; 
        tmpl._numer *= tmpr._denom;
        tmpr._numer *= tmpl._denom;
        Rational result;
        result._numer = tmpl._numer - tmpr._numer; 
        result._denom = common_denom;
        result.normalize();
        return result; */
    }
    friend bool operator==(const Rational& lhs, const Rational& rhs){
        if(lhs._whole == rhs._whole && lhs._numer == rhs._numer && lhs._denom == rhs._denom){
            return true;
        }
        return false;
    }


    Rational& operator=(const Rational& rhs){
        _whole = rhs._whole;
        _numer = rhs._numer;
        _denom = rhs._denom;
        
        return *this;
    }
    Rational& operator-(){
        _whole = -_whole;
        if(_whole == 0){
            _numer = - _numer;
        }
    return *this;
    }

    friend bool operator!=(const Rational& lhs, const Rational& rhs){
        return !(lhs == rhs);
    }
    friend bool operator>(const Rational& lhs, const Rational& rhs){
         if (lhs._whole > rhs._whole){
            return true;
        }

        Rational tmpl(0, lhs._whole * lhs._denom + lhs._numer, lhs._denom);
        Rational tmpr(0, rhs._whole * rhs._denom + rhs._numer, rhs._denom);
        if (double(tmpl._numer / tmpl._denom) > double(tmpr._numer / tmpr._denom)){
            return true;
        }

        return false;
    }

    friend bool operator<(const Rational& lhs, const Rational& rhs){
        return rhs > lhs; 
    }
    
    friend bool operator>=(const Rational& lhs, const Rational& rhs){
        return (rhs > lhs) || (lhs == rhs);
    }
     friend bool operator<=(const Rational& lhs, const Rational& rhs){
        return (lhs > rhs) || (lhs == rhs);
    }
};

using namespace std;
int main(){
    Rational a(0, 0, 3), b(4, 5, 6);
    a.normalize();
    b.normalize();
    cout << "Rational a: " << a << endl;
    cout << "Rational b: " << b << endl;

    Rational d = a * b;
    cout << "a * b = " << d << endl;

    return 0;
}