using namespace std;
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <iomanip>
#include <cstdlib>

#include "resizablearray.h"
#include "hash.h"

#include "Magazin.h"
#include "Read.h"

#define MAXPRODUSE 500

void Task2c(string idBon, Hash < string, int > &H, Produs *produse) {
    ResizableArray < int > continut = H.getValue(idBon);

    if (continut.size() == 0) {
        cout << "Id de bon invalid!\n";
        return;
    }

    cout <<"Bonul cu id-ul " << idBon << " contine urmatoarele produse:\n";

    int sz = continut.size();
    for (int i = 0; i < sz; ++i) {
        cout << "   " << produse[ continut[i] ].getNume()<< '\n';
    }
}

double Task1c(Hash < string, int > &H, Produs *produse) {
    int nrBonuri = 0;
    int costTotal = 0;
    int sz1, sz2;
    for (int i = 0; i < MOD; ++i) {
        nrBonuri += H[i].size();
        sz1 = H[i].size();
        for (int j = 0; j < sz1; ++j) {
            sz2 = H[i][j].second.size();
            for (int k = 0; k < sz2; ++k) {
                costTotal += produse[ H[i][j].second[k] ].getPret();
            }
        }
    }

    return (1.0 * costTotal / nrBonuri);
}

void Task1a(ResizableArray < Magazin > &magazine, Produs *produse, Hash < string, int > &H) {
    int szBon;
    string idBon;
    ofstream g("task1a.txt");
    int magSz = magazine.size();

    for (int i = 0; i < magSz; ++i) {
        int totalVanzari = 0;

        for (int j = 1; j < 367; ++j) {
            int bonuriZi = magazine[i].zi[j].size(); // cate bonuri am vandut in ziua j
            for (int k = 0; k < bonuriZi; ++k) {
                idBon = magazine[i].zi[j][k];
                ResizableArray < int > continutBon = H.getValue(idBon);

                szBon = continutBon.size();
                for (int l = 0; l < szBon; ++l) {
                    totalVanzari += produse[ continutBon[l] ].getPret();
                }
            }
        }
        g << magazine[i].getLocatie() << ',' << totalVanzari << '\n';
    }
    g.close();
}

void Task1b(Hash < string, int > &H, Produs *produse) {
    ofstream g("task1b.txt");

    int fcvProduse[MAXPRODUSE] = {0}; // fcvProduse[i] = de cate ori am vandut produsul cu id-ul i

    int szBucket, szBon;
    for (int i = 0; i < MOD; ++i) {
        szBucket = H[i].size();
        for (int j = 0; j < szBucket; ++j) {
            szBon = H[i][j].second.size();

            for (int k = 0; k < szBon; ++k) {
                ++fcvProduse[ H[i][j].second[k] ];
            }
        }
    }

    for (int i = 0; i < MAXPRODUSE; ++i) {
        if (fcvProduse[i] > 0) {
            g << produse[i].getNume() << ',' << fcvProduse[i] << '\n';
        }
    }
    g.close();
}

void task2a(ResizableArray<Magazin> &magazine, Hash < string, int > &hBonuri){
    int *vanzari = new int[367]();
    int vanzareInZi;
    string idBon;

    for (int idmag = 0; idmag < magazine.size(); ++idmag){
        for (int i = 1; i < 367; ++i){
            vanzareInZi = 0;
            for (int j = 0; j < magazine[idmag].zi[i].size(); ++j){
                idBon = magazine[idmag].zi[i][j];
                ResizableArray < int > continut = hBonuri.getValue(idBon);
                vanzareInZi += continut.size();
            }
            vanzari[i] += vanzareInZi;
        }
    }

    ofstream g("task2a.txt");
    for (int i = 1; i < 367; ++i)
        g << i << ',' << vanzari[i] << '\n';
        // cout << vanzari[i] << '\n';

    int max1 = vanzari[1], max2 = vanzari[2], max3 = vanzari[3];
    for (int i = 4; i < 367; ++i){
        if (vanzari[i] < max1)
        {
            if (vanzari[i] < max2)
            {
                if (vanzari[i] > max3)  max3 = vanzari[i];
            }
            else{
                    max3 = max2;
                    max2 = vanzari[i];
                }
        }
        else{
                max3 = max2;
                max2 = max1;
                max1 = vanzari[i];
            }
    }

    for (int i = 1; i < 367; ++i){
        if (vanzari[i] == max1) cout << i << ' ';
    }
    cout << max1 << '\n';
    for (int i = 1; i < 367; ++i){
        if (vanzari[i] == max2) cout << i << ' ';
    }
    cout << max2 << '\n';
    for (int i = 1; i < 367; ++i){
        if (vanzari[i] == max3) cout << i << ' ';
    }
    cout << max3 << '\n';


    delete[] vanzari;
    g.close();
}

int main()
{
    // Ordonam array-ul asta dupa time_t pentru ultimul task
    ResizableArray < Pair < string, time_t > > bonuri;
    ResizableArray< Magazin > magazine; //magazinele vor fi tinute in RA si indexate dupa id - 1
    Categorii cat;
    Produs produse[MAXPRODUSE];
    Hash < string, int > hBonuri(MOD);

    Read R;
    R.MagazineRead("magazine.csv", magazine);
    R.CategoriiRead("categorii.csv", cat);
    R.ProduseRead("produse.csv", produse, cat);
    R.BonuriRead("bonuri.csv", hBonuri);
    R.TranzactiiRead("tranzactii.csv", magazine, bonuri);

    Task1a(magazine, produse, hBonuri);
    Task1b(hBonuri, produse);
    return 0;
}