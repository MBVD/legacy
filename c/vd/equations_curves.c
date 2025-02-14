#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define eps 0.001

double func1(double x) {
  return 3*(0.5/(x + 1) + 1);
}

double func2(double x){
  return 2.5*x-9.5;
}

double func3(double x){
  return 5/x;
}

double d(double(*f)(double x), double x){
  const double long h=1e-10;
  return (f(x+h)-f(x-h))/(2.0*h);
}

double comb_method(double(*f)(double), double a, double b){
  if (f(a) * f(b) > 0){
    printf("na intervalah odinakovie znaki!!!!!!!!😋😊 %lf, %lf", f(a), f(b));
    return -1;
  }
  while(1){
    double x1 = (a*f(b) - b*(f(a))) / (f(b) - f(a));
    if ((f(a) < 0 && f((a+b)/2) <= (f(a) + f(b))/2) || (f(a) >= 0 && f((a+b)/2) > (f(a) + f(b))/2)){
      a = x1;
    }else{
      b = x1;
    }
    double d1;
    if ((f(a) < 0 && f((a+b)/2) <= (f(a) + f(b))/2) || (f(a) >= 0 && f((a+b)/2) > (f(a) + f(b))/2)){
      d1 = b;
    }else{
      d1 = a;
    }
    double x2 = d1 - f(d1)/d(f, d1);
    // printf("%lf \n", x1);

    // if (x2 < a || x2 > b){
    //   x2 = x1;
    // }    
    // if (dd (d(f, x2+1e-10), d(f, x2-1e-10)) * d(f, x2) > 0){
    //   a = x2;
    // } else{
    //   b = x2;
    // }
    // if (f(x1) * f(x2) < 0){
    //   a = x1;
    //   b = x2;
    // }
    // else{
    //   a = x0;
    //   b = x2;
    // }
    if (fabs(b-a) < eps){
      return x2;
    }
  }
}

double simpson_integral(double (*func)(double), double a, double b) {
  int n = 2;
  double h = (b - a) / n;
  double sum_odd = func((a+b)/2);
  double sum_even = 0;  
  double integral = h * (func(a) + sum_odd*4 + func(b)) / 3;
  double integral_prev;
  do {
    sum_even += sum_odd;
    // printf("%lf \n", integral);
    integral_prev = integral;
    n *= 2;
    h /= 2;
    sum_odd = 0;
    for (int i = 1; i < n; i += 2) {
      sum_odd += func(a + i * h);
    }
    integral = h * (func(a) + 4 * sum_odd + 2 * sum_even + func(b)) / 3;
  } while (1/15*fabs((integral - integral_prev)) >= eps);
  return integral;
}

int main(){
  // printf("%lf ",  comb_method(func3, -0.1, 100000));
  // printf("%lf %lf", comb_method(func2, 0, 4), comb_method(func1, -1.8, -1.3));
  double root1 = comb_method(func2, 0, 4), root2 = comb_method(func1, -1.8, -1.3);
  // printf("%g \n", root2);
  printf("%lf %lf \n", fabs(simpson_integral(func2, 0, root1)), fabs(simpson_integral(func1, root2, 0)));
}