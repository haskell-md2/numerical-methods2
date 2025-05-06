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


    for(int i = 3; i <= n; i++ ){

        double next_y = res.back().y + h/12.0 * (23 * equa(res.back().x,res.back().y) 
        - 16*equa(res.at(res.size() - 2).x,res.at(res.size() - 2).y)
        +5*equa(res.at(res.size() - 3).x,res.at(res.size() - 3).y));
        
        res.push_back({a + i * h, next_y});
    }

    return res;
}


vector<Point> solve_ode_adaptive(double (*f)(double, double),Point start,double a,double b,double eps,double n_init) {
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


vector<Point> adams_adaptive(double (*equa) (double, double), Point start_cond, double a, double b, double eps,int n_init){

    double h = (b - a) / n_init;

    vector<Point> res = solve_ode_adaptive(equa,start_cond,a,a + h*2,eps,n_init);

    for(int i = 3; i <= n_init; i++ ){

        
        int temp_n = 1;
        while (true)
        {
            double temp_h = h / temp_n;


            double y1 = res.back().y;
            double y2 = res.back().y;

            for (int j = 0; j < temp_n; j++) {
                y1 = res.back().y + temp_h/12.0 * (23 * equa(res.back().x,res.back().y) 
                - 16*equa(res.at(res.size() - 2).x,res.at(res.size() - 2).y)
                +5*equa(res.at(res.size() - 3).x,res.at(res.size() - 3).y));
            }
            for (int j = 0; j < 2*temp_n; j++) {
                y2 = res.back().y + temp_h/24.0 * (23 * equa(res.back().x,res.back().y) 
                - 16*equa(res.at(res.size() - 2).x,res.at(res.size() - 2).y)
                +5*equa(res.at(res.size() - 3).x,res.at(res.size() - 3).y));
            }

            double error = fabs(y1 - y2) / 7.0;

            if (error < eps) {
                        
                res.push_back({ a + h*(i+1) ,y2 });
                break;
            }
            else
            {
                temp_n *= 2;
            }
        }

        // x = a + h*(i+1);
        // solution.push_back({ x ,y });

        // double next_y = res.back().y + h/12.0 * (23 * equa(res.back().x,res.back().y) 
        // - 16*equa(res.at(res.size() - 2).x,res.at(res.size() - 2).y)
        // +5*equa(res.at(res.size() - 3).x,res.at(res.size() - 3).y));
        
        // res.push_back({a + i * h, next_y});
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


    out1.close();
    out2.close();

}


void TEST3(){
    ofstream out;
    out.open(PATH"3q.txt");

    vector<Point> result = {};
    double err = 0;

    int n = 16;
    double h = powf(1.7,-8)/n;

    double x_0 = 1;

    for(int i = n; i > 0; i--){
        result = adams(equation, Point{ x_0,goal_fun(x_0) - h *i }, x_0, 5, 512);
        err = get_accurancy(result, goal_fun);
        out <<  - h * i << " " << err << endl;
    }
    

    result = adams(equation, Point{ x_0,goal_fun(x_0) }, x_0, 5, 512);
    err = get_accurancy(result, goal_fun);
    out << 0 << " " << err << endl;

    for(int i = 0; i <= n; i++){
        result = adams(equation, Point{ x_0,goal_fun(x_0) + h *i }, x_0, 5, 512);
        err = get_accurancy(result, goal_fun);
        out <<  h * i << " " << err << endl;
    }


    out.close();
}

void TEST4(){
    ofstream out;
    out.open(PATH"4q.txt");

    for(int i = 1; i < 5; i++){
        double eps = powf(10,-i);
        vector<Point> result = adams_adaptive(equation, Point{ 1,goal_fun(1) }, 1, 5, eps,8);
        double err = get_accurancy(result, goal_fun);
        out <<  eps << " " << err << endl;
    }

    out.close();
}

int main() {

    TEST3();


    return 0;
}