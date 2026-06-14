#include <iostream>
#include "Pile.h"
#include "logique.h"
#include "Formule.h"

using namespace std;

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

    // vector<string> testFormules = {
    //     // 1. Cas limites
    //     "a",
    //     "-a",
    //     "(((a)))",
    //     // 2. Opérateurs multiples de même niveau
    //     "a|b|c",
    //     "a & b & c",
    //     "((a & b) | c)",
    //     // 3. Cas De Morgan & Négations complexes
    //     "-(p>q)&(p>q)", // VOIR POUR CE CAS LÀ
    //     "-((a & b) | (c & d))", // VOIR CE QUI CLOCHE
    //     // "----a",
    //     // 4. Distributivité intensive
    //     "((a & b) | (c & d))",
    //     "((a & (b & c)) | d)", // VOIR CE CAS PRÉCIS
    //     // 5. Implications imbriquées.
    //     "(a > (b > (c > d)))", // VOIR CE CAS AUSSI
    //     "((p > q) > (r > s))"
    // };

    // cout << "DEBUT DES TESTS" << '\n';
    

    // for (unsigned int i = 0; i < testFormules.size(); ++i) {
    //     f.setFormule(testFormules[i]);
    //     cout << "-----" << '\n';
    //     cout << "CNF : " << f.cnf() << '\n';
    //     cout << "CNF LIST : " << '\n';
    //     displayVector(f.cnfList());
    //     cout << '\n';
    //     cout << "-----" << '\n';
    //     cout << '\n';
    // }
    f.setFormule("a|b");
    displayVector(f.cnfListNegation());

    return 0;
}

