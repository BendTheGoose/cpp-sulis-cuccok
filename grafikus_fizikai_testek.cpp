#include "graphics.hpp"
#include <vector>
#include <cmath>
#include <ctime>
#include <string>
using namespace std;
using namespace genv;


struct testek {
private:
    double x, y;
    double vx, vy;
    double tomeg;

public:
    testek(double kezd_x, double kezd_y, double kezd_xv, double kezd_yv, double m) {
        x = kezd_x;
        y = kezd_y;
        vx = kezd_xv;
        vy = kezd_yv;
        tomeg = m;
    }

    void fizika(double ero_x, double ero_y, double dt) {

        double ax = ero_x / tomeg;
        double ay = ero_y / tomeg;
        vx += ax * dt;
        vy += ay * dt;
        x += vx * dt;
        y += vy * dt;
    }

    double x1() const { return x; }
    double y1() const { return y; }
    double vx1() const { return vx; }
    double vx2() const { return vy; }
    double tomeg1() const { return tomeg; }
};

struct kornyezet {
    double G;
    double fekero;
    double dt;
};

const int XX = 1000;
const int YY = 800;

int main() {
    gout.open(XX, YY);
    gin.timer(1000 / 60);
    srand(time(0));

    kornyezet alap = {1000, 0.02, 0.05};
    vector<testek> test;

    for (int i = 0; i < 200; ++i) {
        double px = rand() % XX;
        double py = rand() % YY;
        double pvx = ((rand() % 100) - 50) / 10;
        double pvy = ((rand() % 100) - 50) / 10;
        double ptomeg = (rand() % 5) + 1;

        test.push_back(testek(px, py, pvx, pvy, ptomeg));
    }
     int bebe =20;
    int kenyeres =950;
    event ev;
    while (gin >> ev) {

        if (ev.type == ev_key && ev.keycode == key_up) {
            alap.G += 100;
        }
        if (ev.type == ev_key && ev.keycode == key_down) {
            alap.G -= 100;
           if (alap.G < 0)
            {
                alap.G = 0;
           }
        }

        if (ev.type == ev_timer) {

            vector<double> erok_x(test.size(), 0.0);
            vector<double> erok_y(test.size(), 0.0);

            for (int i = 0; i < test.size(); ++i) {
                for (int j = 0; j < test.size(); ++j) {
                    if (i != j) {
                        double dx = test[j].x1() - test[i].x1();
                        double dy = test[j].y1() - test[i].y1();
                        double tavolsag = sqrt(dx*dx + dy*dy) + 2;
                        double ero = alap.G * (test[i].tomeg1() * test[j].tomeg1()) / (tavolsag * tavolsag);

                        erok_x[i] += ero * (dx / tavolsag);
                        erok_y[i] += ero * (dy / tavolsag);
                    }
                }
                erok_x[i] -=alap.fekero * test[i].vx1() * test[i].tomeg1();
                erok_y[i] -= alap.fekero * test[i].vx2() * test[i].tomeg1();
            }

            for (int i = 0; i < test.size(); ++i) {
                test[i].fizika(erok_x[i], erok_y[i], alap.dt);
            }
            gout << move_to(0, 0) << color(0, 0, 0) << box(XX, YY);
            gout << move_to(10, 20) << color(255, 255, 255) << text("G: " + to_string((int)alap.G) + " [m^3/(kg*s^2)]");



            for (int i = 0; i < test.size(); ++i) {
                gout <<color(0,255,255)<< move_to((int)test[i].x1(), (int)test[i].y1()) << dot;
            }

             gout << move_to(kenyeres,bebe) << color (5, 5,5) << text("ZJ1RWE");
                if (bebe> YY )
             {bebe=0;
             kenyeres-=475;} else {
                    bebe+=2; }
                if (kenyeres<0)
                    kenyeres=950;
            }
            gout << refresh;
        }


    return 0;
}
