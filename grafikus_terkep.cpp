#include <fstream>
#include <cmath>
#include <vector>
#include <queue>
#include <string>
#include <iostream>
#include "graphics.hpp"

using namespace genv;
using namespace std;


// az első feladatot választottam
/* A legrövidebb út nem volt feltétel, de sokszor lefagyott, vagy beragadt valami sarokban, amikor random utvonal keresest csinált,  így a bfs  volt a legbiztosabb,
 így legalább fix talál utat */
// és a térképgenerálás link csak visszadobott a moodle-re, szval az egyik órai feladatos térképgeneráló kódot használtam, remélem ezzel is jó.



class Utkereso {
    int X, Y;
    vector<vector<int>> t;

    int kx, ky;
    int vx, vy;

    int katt;
    int szint;

    vector<pair<int, int>> ut;

    void rajzol() {
        for(int i=0; i<X; i++){
            for(int j=0; j<Y; j++){

                if (t[i][j] >= 0) {
                    int zold = 255 - (t[i][j]*2);
                    if(zold<0) zold=0;
                    if(zold>255) zold=255;
                    gout << color(0,zold,0);
                }
                else {
                    int kek = 255 + (t[i][j]*2);
                    if(kek<0) kek=0;
                    if(kek>255) kek=255;
                    gout << color(0,0,kek);
                }
                gout << move_to(i,j) << dot;
            }
        }
    }

    void ut_keres() {
        ut.clear();


        if(t[kx][ky] < 0 || t[vx][vy] < 0) return;


        vector<vector<pair<int, int>>> honnan(X, vector<pair<int, int>>(Y, {-1, -1}));
        vector<vector<bool>> lattam(X, vector<bool>(Y, false));
        queue<pair<int, int>> sor;

        sor.push({kx, ky});
        lattam[kx][ky] = true;

        int dx[] = {0, 1, 0, -1};
        int dy[] = {-1, 0, 1, 0};

        bool megvan = false;

        while(!sor.empty()) {
            auto akt = sor.front();
            sor.pop();

            if (akt.first == vx && akt.second == vy) {
                megvan = true;
                break;
            }

            for (int i=0; i<4; i++) {
                int ujx = akt.first + dx[i];
                int ujy = akt.second + dy[i];

                if(ujx>=0 && ujx<X && ujy>=0 && ujy<Y) {
                    if(!lattam[ujx][ujy] && t[ujx][ujy] >= 0) {
                        lattam[ujx][ujy] = true;
                        honnan[ujx][ujy] = akt;
                        sor.push({ujx, ujy});
                    }
                }
            }
        }


        if(megvan) {
            pair<int, int> akt = {vx, vy};
            while(akt.first != kx || akt.second != ky) {
                ut.push_back(akt);
                akt = honnan[akt.first][akt.second];
            }
            ut.push_back({kx, ky});

            szint = 0;
            if (ut.size() > 1) {
                for(size_t i=0; i < ut.size() - 1; i++) {
                    int m1 = t[ut[i].first][ut[i].second];
                    int m2 = t[ut[i+1].first][ut[i+1].second];
                    szint += abs(m1 - m2);
                }
            }

        }
    }

    void frissit() {
        gout << move_to(0,0) << color(0,0,0) << box(X, Y+50);
        rajzol();

        if (ut.size() > 0) {
            gout << color(255,0,0);
            for (auto p : ut) {
                gout << move_to(p.first, p.second) << box(2,2);
            }
        }


        if(katt >= 1) gout << color(255,255,0) << move_to(kx-2, ky-2) << box(5,5);
        if(katt == 2) gout << color(255,100,0) << move_to(vx-2, vy-2) << box(5,5);


        gout << move_to(10, Y+15) << color(255,255,255);
        if(katt == 0) {
            gout << text("Jelold ki az elso pontot!");
        }
        else if(katt == 1) {
            gout << text("Jelold ki a masodikat is!");
        }
        else if(katt == 2) {
            if(t[kx][ky] < 0 || t[vx][vy] < 0) {
                gout << color(255,0,0) << text("Vizben nem ervenyes :((");
            }
            else if(ut.size() > 0) {

                gout << color(0,255,0) << text("Sikeres ut! Hossz: " + to_string(ut.size()));
            }
            else {
                gout << color(255,0,0) << text("Nincs ilyen ut szarazfoldon!");
            }
        }

        gout << refresh;
    }

public:


    Utkereso(string fajlnev) {
        ifstream f(fajlnev);

        if(!f.is_open()) {
            cout << "nofileeee "  << endl;
            exit(1);
        }

        f >> X >> Y;
        t.resize(X, vector<int>(Y));

        for(int j=0; j<Y; j++) {
            for(int i=0; i<X; i++) {
                f >> t[i][j];
            }
        }
        f.close();

        katt = 0;
        kx = ky = vx = vy = -1;
    }

    void indit() {
        gout.open(X, Y+50);
        event ev;

        frissit();

        while(gin >> ev && ev.keycode != key_escape) {
            if(ev.type == ev_mouse && ev.button == btn_left) {
                int egerx = ev.pos_x;
                int egery = ev.pos_y;

                if (egery < Y) {
                    if(katt == 0 || katt == 2) {
                        kx = egerx;
                        ky = egery;
                        katt = 1;
                        ut.clear();
                    }
                    else if(katt == 1) {
                        vx = egerx;
                        vy = egery;
                        katt = 2;
                        ut_keres();
                    }
                    frissit();
                }
            }
        }
    }
};

int main() {
    Utkereso alma("terkep.txt");
    alma.indit();

    return 0;
}
