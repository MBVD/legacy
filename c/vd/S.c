#include <stdio.h>
#include <math.h>

#define max_iterations 1000

double f1(double x) {
    return pow(2, x) + 1;
}

double f2(double x) {
    return pow(x, 5);
}

double f3(double x) {
    return (1 - x) / 3;
}

double F12(double x){
    return f1(x)-f2(x);
}

double F13(double x){
    return f1(x)-f3(x);
}

double F23(double x){
    return f2(x)-f3(x);
}

double dF12(double x){
    return log(2)*pow(2, x) - 5*pow(x, 4);
}

double dF13(double x){
    return log(2)*pow(2, x) + 1/3;
}

double dF23(double x){
    return 5*pow(x, 4) + 1/3;
}

void newton_method(double (*f)(double), double (*df)(double), double *x, int* iterations) {
    *x = *x - f(*x) / df(*x);
    *iterations++;
}

void chord_method(double (*f)(double), double *a, double *b, int *iterations) {
    double x = (*a * f(*b) - *b * f(*a)) / (f(*b) - f(*a));

    if (f(x) * f(*a) < 0) {
        *b = x;
    } else {
        *a = x;
    }
    *iterations++;
}



double intersection_points(double (*f)(double), double (*g)(double), double a, double b, double epsilon) {
    int iteration = 0;
    double xN = a, xCH = (a * f(b) - b * f(a)) / (f(b) - f(a));

    while ((fabs(f(xN)) > epsilon || fabs(f(xN)) > epsilon) && iteration < max_iterations)
    {
        newton_method(f, g, &xN, &iteration);
        chord_method(f, &a, &b, &iteration);
    }
    if (fabs(f(xCH)) < epsilon){
        return xCH;
    }
    return xN;
}

double area_under_graph(double (*f)(double), double a, double b, int accuracy) {
    double h = (b - a) / accuracy; 
    double sum = (f(a) + f(b)) / 2; 

    for(int i = 1; i < accuracy; i++){
        sum += f(a + i*h); 
    }

    return h * sum;
}



int main() {
    int accuracy;
    scanf("%d ", &accuracy);
    double a = -3, b = 2, eps = 0.000001, S = 0;

    double x12 = intersection_points(F12, dF12, a, b, eps);
    double x23 = intersection_points(F23, dF23, a, b, eps);
    double x13 = intersection_points(F13, dF13, a, b, eps);

    printf("Ð¢Ð¾Ñ‡ÐºÐ° Ð¿ÐµÑ€ÐµÑÐµÑ‡ÐµÐ½Ð¸Ñ 1: %.3f\n", x12);
    printf("Ð¢Ð¾Ñ‡ÐºÐ° Ð¿ÐµÑ€ÐµÑÐµÑ‡ÐµÐ½Ð¸Ñ 2: %.3f\n", x23);
    printf("Ð¢Ð¾Ñ‡ÐºÐ° Ð¿ÐµÑ€ÐµÑÐµÑ‡ÐµÐ½Ð¸Ñ 3: %.3f\n", x13);

    S += area_under_graph(f1, x13, x23, accuracy);
    //printf("%f \n", area_under_graph(f1, x12, x13));
    S -= area_under_graph(f2, x23, x12, accuracy);
    //printf("%f \n", area_under_graph(f2, x12, x23));
    S -= area_under_graph(f3, x12, x23, accuracy);
    //printf("%f \n", area_under_graph(f3, x12, x23));

    printf("S: %.3f\n", S);

    return 0;
}