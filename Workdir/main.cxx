#include <iostream>
#include "Pile.h"
#include "logique.h"
#include "Formule.h"

using namespace std;

int main() {

    Formule f;
    f.setFormule("--((p>q)&(r>s))");

    cout << "Formule de base : " << f.toString() << "\n\n";

    cout << "SANS NNF" << "\n\n";

    cout << "Implication : " << f.implFree() << '\n';
    cout << "Morgan : " << f.morgan() << "\n\n";

    cout << "AVEC NNF" << "\n\n";

    cout << "Implication nnf : " << f.implFreeNnf() << '\n';
    cout << "Morgan nnf : " << f.morganNnf() << "\n\n";

    return 0;
}
