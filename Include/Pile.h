#ifndef PILE_H
#define PILE_H

#include <vector>
#include <string>

/**
 * La classe Pile représente une pile de string et fournit des méthodes pour empiler, dépiler,
 * vérifier si la pile est vide, etc.
 * Elle est utilisée dans les différentes fonctions de manipulation de formules logiques
 * pour gérer les sous-formules et les opérateurs lors de la conversion, de la simplification, etc.
 */

class Pile {
    private :
        std::vector<std::string> pile;
    
    public:
        bool isEmpty() const;
        void stack(std::string c);
        std::string unstack();
        std::string top() const;
        std::string toString() const;
};

#endif
