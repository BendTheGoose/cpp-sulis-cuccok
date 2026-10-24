#include <iostream>
#include <map>
#include <list>
#include <string>
#include <fstream>

using namespace std;

class eredmenyek {
private:
    map<int, list<list<int> > > adatok;
// az emailem-ben lévő azonosítót nagybetüvel adtam meg ( -----) , kisbetükkel más adatszerkezetet kaptam
public:
    void uj_jegy(int kod, int vizsgaszam, int jegy) {
        if (adatok[kod].size() <= vizsgaszam) {
            while (adatok[kod].size() <= vizsgaszam) {
                list<int> ures_vizsga;
                adatok[kod].push_back(ures_vizsga);
            }
        }
        list<list<int> >::iterator it = adatok[kod].begin();
        for(int i=0; i< vizsgaszam; i++) {
            it++ ;
        }
        it->push_back(jegy);
    }

    double v_atlag(int kod, int vszam) {
        if (adatok.find(kod) == adatok.end()) {
            return 0.0;
        }
        if (adatok[kod].size() <= vszam) {
            return 0.0; }


        list<list<int> >::iterator it = adatok[kod].begin();
        for(int i = 0; i < vszam; i++)
        {   it++;
        }

        int osszeg = 0;
        int db = 0;

        for (list<int>::iterator jegy_it = it->begin(); jegy_it != it->end(); jegy_it++)
        {
            osszeg += *jegy_it;
            db++;
        }

        if (db == 0) return 0.0;

        return (double)osszeg / db;
    }

    void save(string fajlnev) {
        ofstream fajlmentes(fajlnev);

        for(auto const& kurzus : adatok) {
            fajlmentes << kurzus.first << " " << kurzus.second.size() << endl;

            for(auto const& vizsga : kurzus.second) {
                fajlmentes << vizsga.size() << " ";
                for(int j : vizsga) {
                    fajlmentes << j << " ";
                }
                fajlmentes << endl;
            }
        }
        fajlmentes.close();
    }

    void befajl(string fajlnev) {
        ifstream f(fajlnev);
        if(!f.is_open()) return;

        adatok.clear();
        int k_kod, v_szam;

        while(f >> k_kod >> v_szam) {
            for(int i = 0; i < v_szam; i++) {
                int jegyekszama;
                f >> jegyekszama;

                list<int> aktualis_vizsga;
                for(int j = 0; j < jegyekszama; j++) {
                    int jegy;
                    f >> jegy;
                    aktualis_vizsga.push_back(jegy);
                }
                adatok[k_kod].push_back(aktualis_vizsga);
            }
        }
        f.close();
    }
};

int main() {
    eredmenyek neptun;

    neptun.uj_jegy(101,0,5);
    neptun.uj_jegy(101, 0,3);
    neptun.uj_jegy(101,0,4);

    neptun.uj_jegy(101,1,2);

    neptun.uj_jegy(202,0,5);
    neptun.uj_jegy(202,0,4);

    cout << "101, 1. vizsga atlag: " << neptun.v_atlag(101, 0) << endl;
    cout << "101, 2. vizsga atlag: "<< neptun.v_atlag(101, 1) << endl;
    cout <<"202, 1. vizsga atlag: " << neptun.v_atlag(202, 0)  << endl;

    neptun.save("adatok.txt");

    eredmenyek ures_neptun;
    ures_neptun.befajl("adatok.txt");

    cout << "(Fajlbol) 101, 1. vizsga atlag): " << ures_neptun.v_atlag(101, 0) << endl;

    return 0;
}
