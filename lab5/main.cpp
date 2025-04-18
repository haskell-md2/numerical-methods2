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


bool RungeRule(double y2, double y1, double eps) {
    return fabs(y2 - y1) / (7) < eps;
}

double get_y_by_h(double x, double y, double h, double (*equa) (double, double)) {
    double k1 = equa(x, y);
    double k2 = equa(x + h / 3.0, y + h * k1 * (1.0 / 3.0));
    double k3 = equa(x + (2 / 3.0) * h, y + (2.0 / 3.0) * h * k2);

    return y + (h / 4.0) * (k1 + 3 * k3);
}

double next_y(double x, double y, double* h, double (*equa) (double, double), double eps) {
    double y1 = get_y_by_h(x, y, *h, equa);

    double y2 = get_y_by_h(x, y, (*h) / 2.0, equa);
    y2 = get_y_by_h(x, y2, (*h) / 2.0, equa);

    while (!RungeRule(y2, y1, eps))
    {
        *h = (*h) / 2.0;
        y1 = y2;
        y2 = get_y_by_h(x, y, (*h) / 2.0, equa);
    }

    return y2;
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


vector<Point> solve_ode_adaptive(
    double (*f)(double, double),
    Point start,
    double a,
    double b,
    double eps,
    double n_init
) {
    vector<Point> solution = { start };
    double x = start.x;
    double y = start.y;
    double n = n_init;

    double h = (b - a) / n;

    for (int i = 0; i < n; i++) {
        

        int temp_n = 1;
        while (true)
        {
            double temp_h = h / temp_n;


            double y1 = y;
            double y2 = y;

            for (int j = 0; j < temp_n; j++) {
                y1 = get_y_by_h((a + i *h )+ j* temp_h, y1, temp_h, f);
            }
            for (int j = 0; j < 2*temp_n; j++) {
                y2 = get_y_by_h((a + i * h) + j * temp_h/2.0 , y2, temp_h /2.0, f);
            }

            double error = fabs(y1 - y2) / 7.0;

            if (error < eps) {
                        
                //for test3
                if (temp_n * n > t3_nmax) t3_nmax = temp_n * n;
                /////

                y = y2;
                break;
            }
            else
            {
                temp_n *= 2;
            }
        }



        x = a + h*(i+1);
        solution.push_back({ x ,y });


    }


    return solution;
}


void save_vector(vector<Point>* res, ofstream* f) {
    for (auto p : *res) {
        *f << p.x << " " << p.y << endl;
    }
}

void show_err(vector<Point>* res) {
    for (auto p : *res) {
        cout << p.x << " " << fabs(p.y - goal_fun(p.x)) << endl;
    }
}

double get_accurancy(vector<Point> v, double (*goal_fun) (double)) {
    double max_err = 0;
    for (auto p : v) {
        double err = fabs(p.y - goal_fun(p.x));
        if (err > max_err) max_err = err;
    }

    return max_err;
}


void TEST3() {
    ofstream out1;
    out1.open(PATH"3q_1.txt");

    ofstream out2;
    out2.open(PATH"3q_2.txt");

    for (int k = 1; k <= 12; k++) {
        vector<Point> result = solve_ode_adaptive(equation, Point{ 1,goal_fun(1) }, 1, 5, pow(10, -k), 20);

        double err = get_accurancy(result, goal_fun);

        out1 << pow(10, -k) << " " << err << endl;

        out2 << pow(10, -k) << " " << round(log(t3_nmax)) << endl;

        t3_nmax = 0;
    }


    out1.close();
    out2.close();
}

void TEST2() {
    ofstream out;
    out.open(PATH"2q.txt");

    for (int k = 1; k <= 12; k++) {
        vector<Point> result = solove_difur(equation, Point{ 1,goal_fun(1) }, 1, 5, pow(2, k));

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

    vector<Point> result2h = solove_difur(equation, Point{ 1,goal_fun(1) }, 1, 5, 5);
    vector<Point> resulth = solove_difur(equation, Point{ 1,goal_fun(1) }, 1, 5, 10);

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

void TEST4(){
    ofstream out;
    out.open(PATH"4q.txt");

    for (int k = 1; k <= 12; k++) {
        // vector<Point> result = solve_ode_adaptive(equation, Point{ 1,goal_fun(1) }, 1, 5, pow(10, -k), 20);
        vector<Point> result = solove_difur(equation, Point{ 1,goal_fun(1) }, 1, 5, pow(2, k));

        Point max_err = {0,0};
        
        for(auto point : result){
            double err = fabs(point.y - goal_fun(point.x));
            
            cout << err << endl;

            if(err > max_err.y){
                cout << err << " " << point.x <<  endl;
                max_err.x = point.x;
                max_err.y = err;
            }
        }

        out << k << " " << max_err.x << endl;
    }

    out.close();
}

int main() {

    // TEST1();
    // TEST2();
    // TEST3();
    TEST4();

    return 0;
}