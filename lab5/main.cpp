#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;


typedef struct
{
    double x,y;
} Point;


double goal_fun(double x){
    return (1)/((x+1)* log(x+1));
}

double equation(double x, double y){
    return -y/(x+1) - powf(y,2);
}


bool RungeRule(double y2, double y1, double eps){
    return fabs(y2 - y1)/ (7) < eps;
}

double get_y_by_h(double x, double y, double h, double (*equa) (double, double)){
    double k1 = equa(x,y);
    double k2 = equa(x + h/3.0, y + h*k1 * (1.0/3.0));
    double k3 = equa(x + 2*h/3.0, y+ (2.0/3.0) *h * k2);

    return y + h/4.0 * (k1 + 3*k3);
}

double next_y(double x, double y, double *h, double (*equa) (double, double), double eps){
    double y1 = get_y_by_h(x,y,*h,equa);

    double y2 = get_y_by_h(x,y,(*h)/2.0,equa);
    y2 = get_y_by_h(x,y2,(*h)/2.0,equa);

    while (!RungeRule(y2,y1,eps))
    {
        *h = (*h) / 2.0;
        y1 = y2;
        y2 = get_y_by_h(x,y,(*h)/2.0,equa);
    }
    
    return y2;
}

vector<Point> solove_difur(double (*equa) (double, double), Point start_cond, double a, double b, double eps){
    
    vector<Point> res = vector<Point>();
    res.push_back(start_cond);

    int n = 2;
    double h = (b-a)/n;
    double current_x = a+h;
    while(current_x <= b){
        
        double new_y = next_y(res.back().x, res.back().y,&h,equa,eps);
        current_x += h;
        res.push_back(Point{current_x, new_y});

     
    }

    return res;
}

vector<Point> solve_ode_adaptive(
    double (*f)(double, double),
    Point start,
    double a,
    double b,
    double eps,
    double h_init = 0.1,
    double h_min = 1e-6
) {
    vector<Point> solution = {start};
    double x = start.x;
    double y = start.y;
    double h = h_init;

    while (x < b) {
        if (x + h > b) {
            h = b - x;
        }

        double y1 = get_y_by_h(x, y, h, f);
        
        double y2 = get_y_by_h(x, y, h/2, f);
        y2 = get_y_by_h(x + h/2, y2, h/2, f);

        double error = fabs(y1 - y2) / 7.0;

        if (error < eps) {
            solution.push_back({x + h, y1});
            x += h;
            y = y1;
            //h = h_init;
            
        } else {
            h *= 0.5;
        }
    }

    return solution;
}


void save_vector(vector<Point> * res, ofstream * f){
    for(auto p : *res){
        *f << p.x << " " << p.y << endl;
    }
}

void show_err(vector<Point> * res){
    for(auto p : *res){
        cout << p.x << " " << fabs(p.y - goal_fun(p.x)) << endl;
    }
}

int main(){


    ofstream file("res.txt");

    vector<Point> result = solve_ode_adaptive(equation,Point{1,goal_fun(1)},1,5,0.0001,1);

    save_vector(&result,&file);

    show_err(&result);
    // cout << equation(1,0.15342640972);
    return 0;
}