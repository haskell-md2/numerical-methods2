#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip> 

using namespace std;

#define PATH "data/"

//global vars
double t2_h = 0;

double t3_nmax = 0;

typedef struct
{
    double x, y;
} Point;


double goal_fun(double x) {
    return (1) / ((x + 1) * log(x + 1));
}

double equation(double x, double y) {
    return -y / (x + 1) - powf(y, 2);
}

void show_vector(vector<Point>* res) {
    for (auto p : *res) {
        cout << p.x << " " << p.y << endl;
    }
}

double get_y_by_h(double x, double y, double h, double (*equa) (double, double)) {
    double k1 = equa(x, y);
    double k2 = equa(x + h / 3.0, y + h * k1 * (1.0 / 3.0));
    double k3 = equa(x + (2 / 3.0) * h, y + (2.0 / 3.0) * h * k2);

    return y + (h / 4.0) * (k1 + 3 * k3);
}


vector<Point> solove_difur(double (*equa) (double, double), Point start_cond, double a, double b, int n) {

    vector<Point> res = vector<Point>();
    res.push_back(start_cond);

    double h = (b - a) / n;

    t2_h = h;

    for (int i = 1; i <= n; i++) {

        double new_y = get_y_by_h(res.back().x, res.back().y, h, equa);
        res.push_back(Point{ a + i * h, new_y });

    }

    return res;
}


vector<Point> adams(double (*equa) (double, double), Point start_cond, double a, double b, int n){

    double h = (b-a)/n;

    vector<Point> res = solove_difur(equa,start_cond,a,a + h*2,2);

    //show_vector(&res);

    for(int i = 3; i <= n; i++ ){

        double next_y = res.back().y + h/12.0 * (23 * equa(res.back().x,res.back().y) 
        - 16*equa(res.at(res.size() - 2).x,res.at(res.size() - 2).y)
        +5*equa(res.at(res.size() - 3).x,res.at(res.size() - 3).y));
        
        res.push_back({a + i * h, next_y});
    }

    return res;
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
        cout << err << endl;
        if (err > max_err) max_err = err;
    }

    return max_err;
}




void TEST2() {
    ofstream out;
    out.open(PATH"2q.txt");

    for (int k = 1; k <= 12; k++) {
        vector<Point> result = adams(equation, Point{ 1,goal_fun(1) }, 1, 5, pow(2, k));

        double err = get_accurancy(result, goal_fun);

        out << t2_h << " " << err << endl;
    }

    out.close();
}

void TEST1() {
    ofstream out1, out2, out3;
    out1.open(PATH"1q_1.txt");
    out2.open(PATH"1q_2.txt");
    // out3.open(PATH"1q_3.txt");

    vector<Point> result2h = adams(equation, Point{ 1,goal_fun(1) }, 1, 5, 4);
    vector<Point> resulth = adams(equation, Point{ 1,goal_fun(1) }, 1, 5, 8);

    for (auto p : result2h) {
        out1 << p.x << " " << setprecision(14) << p.y << endl;
    }

    int i = 0;
    for (auto p : resulth) {
        out2 << p.x << " " << setprecision(14) << p.y << endl;

    }

    // for (auto p : result2h) {
    //     out3 << p.x << " " << setprecision(14) << goal_fun(p.x) << endl;
    // }


    out1.close();
    out2.close();
    // out3.close();
}


void TEST3(){
    ofstream out;
    out.open(PATH"3q.txt");

    vector<Point> result = {};
    double err = 0;

    for(int i = 10; i > 0; i--){
        result = adams(equation, Point{ 1,goal_fun(1) - 0.1 *i }, 1, 5, 40);
        err = get_accurancy(result, goal_fun);
        out <<  - 0.1 * i << " " << err << endl;
    }
    

    result = adams(equation, Point{ 1,goal_fun(1) }, 1, 5, 40);
    err = get_accurancy(result, goal_fun);
    out << 0 << " " << err << endl;

    for(int i = 0; i <= 10; i++){
        result = adams(equation, Point{ 1,goal_fun(1) + 0.1 *i }, 1, 5, 40);
        err = get_accurancy(result, goal_fun);
        out <<  0.1 * i << " " << err << endl;
    }


    out.close();
}

int main() {

    TEST3();

    return 0;
}