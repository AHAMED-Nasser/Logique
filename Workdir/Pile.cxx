#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include "Pile.h"


bool Pile::isEmpty() const {
    return pile.empty();
}

void Pile::stack(char c) {
    pile.push_back(c);
}

char Pile::unstack() {
    if (pile.empty()) throw std::out_of_range("Pile vide");
    char c = pile.back();
    pile.pop_back();
    return c;
}

char Pile::top() const {
    if (pile.empty()) throw std::out_of_range("Pile vide");
    return pile.back();
}

std::string Pile::toString() const {
    std::string s = "[";
    for (unsigned int i = 0; i < pile.size(); ++i) {
        s += pile[i];
        if (i < pile.size() - 1) {
            s += ",";
        }
    }
    s += "]";
    return s;
}
