#include <iostream>
#include "Pile.h"
#include "logique.h"
#include "Formule.h"

using namespace std;

int main() {

    Formule f;
    f.setFormule("-((p>q)&(r>s))");

    cout << "Formule de base : " << f.toString() << '\n';
    cout << "Implication : " << f.impl_free() << '\n';
    cout << "Morgan : " << f.morgan() << '\n';

    // string implement_free = f.impl_free();
    // cout << "VRAI : " << impl_free("(a>b)") << '\n';
    // cout << "VRAI : " << impl_free("(a>(b>c))") << '\n';
    // cout << "VRAI : " << impl_free("-(a>b)") << '\n';
    // cout << "FAUX : " << impl_free("((p>q)&(r>s))") << '\n';
    // cout << "VRAI : " << impl_free("-(p>q)&(r>s)") << '\n';
    // cout << "FAUX : " << impl_free("-((p>q)&(r>s))") << '\n';
    // cout << "FAUX : " << impl_free("((p>q)>(r>s))") << '\n';

    // cout << "VRAI : " << morgan("(-a|b)") << '\n';
    // cout << "VRAI : " << morgan("-(-a|b)") << '\n';
    // cout << "VRAI : " << morgan("-(-p|q)&(-r|s)") << '\n';
    // cout << "VRAI : " << morgan("-((-p|q)&(-r|s))") << '\n';
    // cout << "VRAI : " << morgan("-((-p|q)|(-r|s))") << '\n';

    return 0;
}
