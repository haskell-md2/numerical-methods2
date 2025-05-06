#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>

#define PATH "data/"

using namespace std;

//global vars
double t2_h = 0;

////

typedef struct {
    double y;
    double dy;
} State;

typedef struct{
    double x;
    double y;
} Point;

double goal_fun(double x){
    return exp(x);
}

void f(double x, State y, State *dydx) {
    dydx->y = y.dy;
    dydx->dy = exp(x)*(cos(x) + exp(x) + 1) - y.dy*cos(x) - y.y*exp(x);
}

void f_0(double x, State y, State *dydx) {
    dydx->y = y.dy;
    dydx->dy = - y.dy*cos(x) - y.y*exp(x);
}

void rk4_step(double x, double h, State *y, void (*f)(double, State, State*)) {

    State k1, k2, k3, k4, temp;
    
    f(x, *y, &k1);

    temp.y =  y->y + h * k1.y * (1.0 / 3.0);
    temp.dy =  y->dy + h * k1.dy * (1.0 / 3.0);
    f(x + h / 3.0, *y, &k2);


    temp.y =  y->y + (2.0 / 3.0) * h * k2.y;
    temp.dy =  y->dy + (2.0 / 3.0) * h * k2.dy;
    f(x + (2 / 3.0) * h, *y, &k3);

    y->y += (h / 4.0) * (k1.y + 3 * k3.y);
    y->dy += (h / 4.0) * (k1.dy + 3 * k3.dy);

}

void superposition_method(vector <Point> * solution, int N, double A, double B){

    cout << B << endl;

    const double a = 0.0, b = 1.0;
    const double h = (b - a)/N;
    
    const double alpha0 = 1.0, alpha1 = 1.0;
    const double beta0 = 1.0, beta1 = 0.0; 
    
    // 1. Находим два базисных решения однородного уравнения
    vector<State> v(N+1), w(N+1);
    v[0] = {1.0, 0.0};  // v(0)=1, v'(0)=0
    w[0] = {0.0, 1.0};  // w(0)=0, w'(0)=1
    
    for (int i = 0; i < N; ++i) {
        double x = a + i*h;
        v[i+1] = v[i];
        w[i+1] = w[i];
        rk4_step(x, h, &v[i+1], f_0);
        rk4_step(x, h, &w[i+1], f_0);
    }
    
    // 2. Частное решение (можно взять u(x) = e^x, так как оно удовлетворяет неоднородному уравнению)
    vector<State> u(N+1);
    u[0] = {0.0, 0.0};  // u(0)=0, u'(0)=0
    
    for (int i = 0; i < N; ++i) {
        double x = a + i*h;
        u[i+1] = u[i];
        rk4_step(x, h, &u[i+1], f);
    }
    
    // 3. Решаем систему для коэффициентов
    double a11 = alpha0;
    double a12 =  alpha1;
    double b1 = A;
    
    double a21 = beta0*v[N].y + beta1*v[N].dy;
    double a22 = beta0*w[N].y + beta1*w[N].dy;
    double b2 = B - (beta0*u[N].y + beta1*u[N].dy);
    
    double det = a11*a22 - a12*a21;
    
    double c1 = (b1*a22 - b2*a12) / det;
    double c2 = (a11*b2 - a21*b1) / det;
    
    // 4. Строим решение
    for (int i = 0; i <= N; ++i) {
        solution->push_back({a + i*h, u[i].y + c1*v[i].y + c2*w[i].y});
    }

}

double get_accurancy(vector<Point> v, double (*goal_fun) (double)) {
    double max_err = 0;
    int i = 0;
    for (auto p : v) {
        if(i == 0){
            i = 1;
            continue;
        }
        double err = fabs(p.y - goal_fun(p.x));
        if (err > max_err) max_err = err;
    }
    cout << max_err << endl;
    return max_err;
}


void TEST1() {
    ofstream out1, out2;
    out1.open(PATH"1q_1.txt");
    out2.open(PATH"1q_2.txt");
    // out3.open(PATH"1q_3.txt");

    vector<Point> result2h;
    vector<Point> resulth;

    superposition_method(&resulth,32,2.0,exp(1));
    superposition_method(&result2h,64,2.0,exp(1));


    for (auto p : result2h) {
        out1 << p.x << " " << p.y << endl;
    }

    int i = 0;
    for (auto p : resulth) {
        out2 << p.x << " "  << p.y << endl;

    }


    out1.close();
    out2.close();

}

void TEST2() {
    ofstream out;
    out.open(PATH"2q.txt");

    for (int k = 1; k <= 12; k++) {
        vector<Point> result;

        superposition_method(&result,pow(2,k),2.0,exp(1));

        double err = get_accurancy(result, goal_fun);

        out << 1/pow(2,k) << " " << err << endl;
    }

    out.close();
}

void TEST3(){
    ofstream out;
    out.open(PATH"3q.txt");

    vector<Point> result = {};
    double err = 0;

    int n = 16;
    double h = 0.015 *  2.0/n;


    for(int i = n; i > 0; i--){
        superposition_method(&result,64,2.0,exp(1) - i *h);
   
        err = get_accurancy(result, goal_fun);
        out <<  - h * i << " " << err << endl;
        result.clear();
    }
    

    // superposition_method(&result,64,2.0,exp(1));
    // err = get_accurancy(result, goal_fun);
    // out << 0 << " " << err << endl;
    // result.clear();

    for(int i = 0; i <= n; i++){
        superposition_method(&result,64,2.0,exp(1) + i *h);
        err = get_accurancy(result, goal_fun);
        out <<  h * i << " " << err << endl;
        result.clear();
    }


    out.close();
}

int main(){

    TEST3();

    // vector <Point> solution;
    
    // superposition_method(&solution);

    // for (int i = 0; i <= N; i += N/10) {  // Выводим каждую 10-ю точку
    //     double exact = exp(solution[i].x);
    //     double error = fabs(solution[i].y- exact);
    //     cout << error << endl;
    // }
    
    return 0;
}