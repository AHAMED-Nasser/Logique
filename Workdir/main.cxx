#include <iostream>
#include "Pile.h"
#include "logique.h"
#include "Formule.h"

using namespace std;

int main() {

    Formule f;
    f.setFormule("(-a)>b");

    string apres_impl = f.impl_free();
    cout << "DEBUG impl_free : " << apres_impl << endl;

    string apres_morgan =f.morgan();
    cout << "DEBUG morgan : " << apres_morgan << endl;

    string impl = morgan(impl_free("-(p>q)&(p>q)"));


    cout << "DEBUG impl_free: " << impl_free("-(p>q)&(p>q)") << endl;
    cout << "DEBUG morgan + impl_free : " << impl << '\n';
    return 0;
}
