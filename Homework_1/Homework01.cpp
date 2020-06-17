#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

double function(double n) {
    return sqrt(n);     //[1,20]
    //return -n * log(n); //[0.001, 2]
    //return -n * n - 0.5; //[-1,1]
}

int main() {
    freopen("graph1.txt", "w", stdout);

    int width = 100;
    int height = 40;

    double x0, x1;
    cin >> x0 >> x1;

    double min_f = function(x0);
    double max_f = function(x1);

    int start_pos = 0;
    double step_height;
    double step_width = (x1 - x0) / width;

    for (int i = 0; i < width; i++) {
        double curr = function(x0 + step_width * i);

        min_f = min(min_f, curr);
        max_f = max(max_f, curr);
    }


    if (max_f == 0) {
        start_pos = 0;
        step_height = (max_f - min_f) / height;
    }

    else if (min_f == 0) {
        start_pos = height;
        step_height = (max_f - min_f) / height;
    }

    else if (min_f > 0 && max_f > 0) {
        start_pos = height;
        step_height = max_f / height;
    }
    else if (min_f < 0 && max_f < 0) {
        start_pos = 0;
        step_height = abs(min_f / height);
    }
    else if (max_f * min_f < 0) {
        step_height = (max_f - min_f) / height;
        start_pos = round(max_f / step_height);
    }

    char massive[height + 1][width];

    for (int i = 0; i <= height; i++) {
        for (int k = 0; k < width; k++) {
            double current = function(x0 + k*step_width);
            if (i == start_pos) {
                massive[i][k] = '-';
            }
            if (i > start_pos) {
                if (((current < max_f - i * step_height) && (max_f >= 0)) || ((current < 0 - i * step_height) && (max_f < 0))) {
                    massive[i][k] = 'X';
                } else {
                    massive[i][k] = ' ';
                }
            }
            if (i < start_pos) {
                if (((current < max_f - i * step_height) && (max_f >= 0)) || ((current < 0 - i * step_height) && (max_f < 0))) {
                    massive[i][k] = ' ';
                } else {
                    massive[i][k] = 'X';
                }
            }
        }
    }

    {
        for (int i = 0; i <= height; i++) {
            for (int j = 0; j < width; j++) {
                cout << massive[i][j];
            }
            cout << std::endl;
        }
    }

    return 0;
}