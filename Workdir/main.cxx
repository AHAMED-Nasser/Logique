#include <iostream>
#include "Pile.h"
#include "logique.h"
#include "Formule.h"

using namespace std;

// Fonction de teste pour afficher un vector<vector<string>> de manière lisible
void displayVector(vector<vector<string>> listCnf) {
    cout << "[";
    for (size_t i = 0; i < listCnf.size(); ++i) {
        cout << "[";
        
        // Boucle à travers les éléments de la clause actuelle
        for (size_t j = 0; j < listCnf[i].size(); ++j) {
            cout << listCnf[i][j];
            if (j < listCnf[i].size() - 1) {
                cout << ","; // Petit séparateur entre les variables
            }
        }
        
        cout << "]";

        if (i < listCnf.size()-1) {
            cout << ",";
        }
    }

    cout << "]";
}


int main() {

    Formule f;

    vector<string> testFormules = {
    //     // 1. Cas limites
    //     "a",
    //     "-a",
    //     "(((a)))",
    //     // 2. Opérateurs multiples de même niveau
        // "a|b|c|c"
    //     "a &|b|c & c",
    //     "((a &|b|c) | c)",
    //     // 3. Cas De Morgan & Négations complexes
    //     "-(p>q)&(p>q)", // VOIR POUR CE CAS LÀ
    //     "-((a &|b|c) | (c & d))", // VOIR CE QUI CLOCHE
    //     // "----a",
    //     // 4. Distributivité intensive
    //     "((a &|b|c) | (c & d))",
    //     "((a & |b|c & c)) | d)", // VOIR CE CAS PRÉCIS
    //     // 5. Implications imbriquées.
    //     "(a > |b|c > (c > d)))", // VOIR CE CAS AUSSI
    //     "((p > q) > (r > s))"
    };
    
    f.setFormule("(a>b)&(c>d)");
    cout << f.cnf() << '\n';
    displayVector(f.cnfList());

    return 0;
}

