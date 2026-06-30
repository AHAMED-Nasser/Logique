#include "Formule.h"
#include "Pile.h"
#include "logique.h"
#include <string>

using namespace std;

string Formule::getFormule() {
    return this->formule;
}

void Formule::setFormule(const string& formule) {
    this->formule = formule;
}

string Formule::toPostfixe() {
    string output;
    Pile pile;

    for (char c : this->formule) {
        if (LOGIQUE_H::is_proposition(c)) {
            output += c;
        } else if (LOGIQUE_H::is_proposition(c)) {
            while (!pile.isEmpty() && LOGIQUE_H::precedence(pile.top()[0]) >= LOGIQUE_H::precedence(c)) {
                output += pile.unstack();
            }
            pile.stack(string(1, c));
        } else if (c == '(') {
            pile.stack(string(1, c));
        } else if (c == ')') {
            while (!pile.isEmpty() && pile.top() != "(") {
                output += pile.unstack();
            }
            pile.unstack();
        }
    }

    while (!pile.isEmpty()) {
        output += pile.unstack();
    }

    return output;
}

string Formule::toPrefixe() {
    string output;
    Pile pile;

    for (char c : reversed(this->formule)) {
        if (LOGIQUE_H::is_proposition(c)) {
            output = c + output;
        } else if (LOGIQUE_H::is_operator(c)) {
            while (!pile.isEmpty() && LOGIQUE_H::precedence(pile.top()[0]) >= LOGIQUE_H::precedence(c)) {
                output = pile.unstack() + output;
            }
            pile.stack(string(1, c));
        }
        else if (c == ')') {
            pile.stack(string(1, c));
        } else if (c == '(') {
            while (!pile.isEmpty() && pile.top() != ")") {
                output = pile.unstack() + output;
            }
            pile.unstack();
        }
    }

    while (!pile.isEmpty()) {
        output = pile.unstack() + output;
    }

    return output;
}


string Formule::negation_formula() {
    return LOGIQUE_H::negation_formula(this->formule);
}

vector<vector<string>> Formule::cnfListNegation() {
    return LOGIQUE_H::cnfListNegation(this->formule);
}

string Formule::nnf() {
    return LOGIQUE_H::nnf(this->formule);
}

string Formule::implFree() {
    return LOGIQUE_H::implFree(this->formule);
}

string Formule::morgan() {
    return LOGIQUE_H::morgan(this->implFree());
}

string Formule::implFreeNnf() {
    return LOGIQUE_H::nnf(this->implFree());
}

string Formule::morganNnf() {
    return LOGIQUE_H::nnf(this->morgan());
}

string Formule::cnf() {
    return LOGIQUE_H::cnf(this->formule);
}

vector<vector<string>> Formule::cnfList() {
    return LOGIQUE_H::cnfList(this->cnf());
}

string Formule::toString() {
    return "Formule{formule = " + this->formule + "}";
}

