#include<bits/stdc++.h>

using namespace std;

class Polinom {
    public: 
        Polinom (){
            st = 0;
            coefficients.push_back(0);
        }

        explicit Polinom(int _st){
            st = _st;
            for (int i = 0; i<=st; i++){
                coefficients.push_back(0);
            }
        }

        Polinom(int _st, int num) {
            st = _st;
            for (int i = 0; i<=_st; i++){
                coefficients.push_back(num);
            }
        }

        Polinom(const Polinom& b){
            this->coefficients = b.coefficients;
            this->st = b.st;
        }   

        Polinom(initializer_list <double> init_list) {
            for (auto i : init_list){
                coefficients.push_back(i);
                ++st;
            }
        }


        void read(){
            cout<<"enter st of polinom \n";
            cin>>st;
            cout<<"enter vector \n";
            for (int i = 0; i<=st; i++){
                double x; cin>>x;
                coefficients.push_back(x);
            }
            cout<<"success polinom was modifyed";
        } 

       void print() const {
            int cnt = 0;
            for (auto i : coefficients){
                if (cnt > 0) {cout<<" + ";}
                cout<<i<<"x^"<<cnt;
                cnt++;
            }
            cout<<endl;
        }

    private:
        int st;
        vector <double> coefficients;
};

int main() {
    Polinom p = {1, 2, 3, 4, 5};
    Polinom p1;
    p.print();
    p1.print();
    Polinom p2 = p1;
    p2.print();
}
