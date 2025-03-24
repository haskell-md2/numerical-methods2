#include <iostream>
#include <fstream>
#include <cmath>
#include <map>

using namespace std;

#define PATH "data/"

#define GOAL -25.936680479391057

/* global vars. Only for tests */
double h_1q = 0;

int n_3q_1 = 0;
/*understand and forgive*/


using namespace std;

double fun(double x){
    return (powl(x,5) - 4.2 * powl(x,3) + 3.5 * powl(x,2) - 7.0 * x - 7.4) * cos(0.4 * x);
}

double getx(double t, double a, double b){
    return ((a + b)/2.0 + (b-a)/2.0 * t);
}


double Chebyshev(double a, double b){
    double x1 = getx(-1.0/sqrt(2),a,b);
    double x2 = getx(0,a,b);
    double x3 = getx(1.0/sqrt(2),a,b);
    return ((b-a)/2.0)* 2.0/3.0 * (fun(x1) + fun(x2) + fun(x3));
}

bool RungeRule(double Sn, double S2n ,double eps){
    return (fabs(S2n - Sn) / 8) < eps;
}

double Integrate_by_rang(double a, double b, int rang){
    double res = 0;
    double step = (b - a)/(double)rang;
    double current_a = a;
    double current_b = a + step;
    for(int n = 0; n <  rang; n++){
        current_a =  a + step * n;
        current_b =  a + step + n*step;
        res += Chebyshev(current_a,current_b);
    }

    n_3q_1 = log2(rang);
    h_1q = step;
    return res;
}

double Integrate(double a, double b, double eps){
    int n1 = 1;
    int n2 = 2;

    double result = Integrate_by_rang(a,b,n2);
    double check = Integrate_by_rang(a,b,n1);
    while (!RungeRule(check,result,eps))
    {
        n1 *= 2;
        n2 *= 2;
        check = result;
        result = Integrate_by_rang(a,b,n2);
    }


    return result;
    
}

void Test1(){
    double a = -0.8;
    double b = 2.2;

    ofstream out;
    out.open(PATH"1q.txt");

    for(int i = 1; i <= 2048; i *= 2){

        double res = Integrate_by_rang(a,b,i); 
        double err = fabs(res - GOAL);

        out << h_1q << ' ' << err << endl;
    }


    out.close();
}

void Test2(){

    double a = -0.8;
    double b = 2.2;

    ofstream out;
    out.open(PATH"2q.txt");

    for(int i = 1; i <= 13; i++){
        double e = powl(10, -i);
        double res = Integrate(a,b,e); 
        double err = fabs(res - GOAL );
        out << e << ' ' << err << endl;
    }


    out.close();
}


void Test3_1(){

    double a = -0.8;
    double b = 2.2;

    ofstream out;
    out.open(PATH"3_1q.txt");

    for(int i = 1; i <= 13; i++){
        double e = powl(10, -i);
        double res = Integrate(a,b,e); 
        double err = fabs(res - GOAL );
        out << e << ' ' << n_3q_1 << endl;
    }


    out.close();
}

void Test3_2(){

    double a = -0.8;
    double b = 2.2;

    ofstream out;
    out.open(PATH"3_2q.txt");

    double runge = 0;
    double s1 = 0;

    for(int i = 1; i <= 2048; i *= 2){

        double res = Integrate_by_rang(a,b,i); 

        if(s1 != 0){
            runge = fabs(res - s1) / 8.0;
            out << log2(i) << ' ' << runge << endl;
        }

        s1 = res;
        
    }


    out.close();
}


int main(){

    // Test1();
    // Test2();
    Test3_2();
    // double res = Integrate_by_rang(-0.8,2.2,1);
    return 0;
}