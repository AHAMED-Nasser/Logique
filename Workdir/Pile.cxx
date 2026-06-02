#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include "Pile.h"

using namespace std;

bool Pile::isEmpty() const {
    return pile.empty();
}

void Pile::stack(string c) {
    pile.push_back(c);
}

string Pile::unstack() {
    if (pile.empty()) throw std::out_of_range("Pile vide"); // Si la pile est vide -> erreur
    string c = pile.back();
    pile.pop_back();
    return c;
}

string Pile::top() const {
    if (pile.empty()) throw std::out_of_range("Pile vide");
    return pile.back();
}

string Pile::toString() const {
    // Construction basique d'une chaine de caractère
    std::string s = "[";
    for (unsigned int i = 0; i < pile.size(); ++i) {
        s += pile[i];
        if (i < pile.size() - 1) { // Évite une virgule à la fin de la liste
            s += ",";
        }
    }
    s += "]";
    return s;
}
