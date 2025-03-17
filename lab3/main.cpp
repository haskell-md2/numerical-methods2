#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

#define PATH "data/"

/* global vars. Only for tests */
int N_2q = 0;
double h_3q = 0;
/*understand and forgive*/

double Function(double x){
    return powl(x,6)/6 - powl(x,4) * 21/20 +  powl(x,3) * 7/6 -  powl(x,2) * 7/2 - x * 37/5 ;
}

double function(double x){
    return powl(x,5) - 4.2 * powl(x,3) + 3.5 * powl(x,2) - 7.0 * x - 7.4;
}


double integrall(double a, double b){
    return Function(b) - Function(a);
}

double TrapezoidFormula(double a, double b, double (*fun)(double x), int rang){
    double res = 0;
    double step = (b - a)/(double)rang;
    double current_a = a;
    double current_b = a + step;
    for(int n = 0; n <  rang; n++){

        current_a =  a + step * n;
        current_b =  a + step + n*step;
        res += (current_b-current_a)/2.0 * (fun(current_a) + fun(current_b));

    }

    h_3q = step;

    return res;
}

bool RungeRule(double Sn, double S2n ,double eps){
    return fabs(S2n - Sn) / 3.0 < eps;
}

double Integrate(double a, double b, double (*fun)(double x), double eps){
    int n1 = 1;
    int n2 = 2;

    double result = TrapezoidFormula(a,b,fun,n2);
    double check = TrapezoidFormula(a,b,fun,n1);
    while (!RungeRule(check,result,eps))
    {
        n1 *= 2;
        n2 *= 2;
        check = result;
        result = TrapezoidFormula(a,b,fun,n2);
    }

    N_2q = n2;

    return result;
    
}


void Test1(){

    double a = -0.8;
    double b = 2.2;

    ofstream out;
    out.open(PATH"1q.txt");

    for(int i = 1; i <= 10; i++){
        double e = powl(10, -i);
        double res = Integrate(a,b,function,e); 
        double err = fabs(res - integrall(a,b) );
        out << e << ' ' << err << endl;
    }


    out.close();
}


void Test2(){
    double a = -0.8;
    double b = 2.2;

    ofstream out;
    out.open(PATH"2q.txt");

    for(int i = 1; i <= 10; i++){
        double e = powl(10, -i);
        Integrate(a,b,function,e); 
        out << e << ' ' << N_2q << endl;
    }


    out.close();
}


void Test3(){
    double a = -0.8;
    double b = 2.2;

    ofstream out;
    out.open(PATH"3q.txt");

    for(int i = 1; i <= 100000; i *= 2){

        double res = TrapezoidFormula(a,b,function,i); 
        double err = fabs(res - integrall(a,b) );

        out << h_3q << ' ' << err << endl;
    }


    out.close();
}

int main(){
    

    Test3();

    return 0;
}