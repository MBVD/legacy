#include <stdio.h>
#include <stdlib.h>
#define E 0.001
double Myf(double x) {
  return x*x*x; // 3x^2
}
double d(double x, double(*f)(double)){
  const double long h=1e-1;
  return (f(x+h)-f(x-h))/(2.0*h);
}
 
int main() {
  double dy = d(2, Myf);
  printf("f(x) = %lf, f'(x) = %lf", Myf(2), dy);
}