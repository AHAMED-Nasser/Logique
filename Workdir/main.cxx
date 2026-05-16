#include <iostream>
#include "Pile.h"
#include "logique.h"
#include "Formule.h"

using namespace std;

int main() {

    Formule f;
    f.setFormule("(a|b)&c");

    string implement_free = f.impl_free();
    cout << "VRAI : " << impl_free("(a>b)") << '\n';
    cout << "VRAI : " << impl_free("(a>(b>c))") << '\n';
    cout << "VRAI : " << impl_free("-(a>b)") << '\n';
    cout << "FAUX : " << impl_free("((p>q)&(r>s))") << '\n';
    cout << "VRAI : " << impl_free("-(p>q)&(r>s)") << '\n';
    cout << "FAUX : " << impl_free("-((p>q)&(r>s))") << '\n';
    cout << "FAUX : " << impl_free("((p>q)>(r>s))") << '\n';
    
    return 0;
}
