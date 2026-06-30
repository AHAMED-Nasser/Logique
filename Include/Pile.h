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
        std::vector<std::string> pile; // Initialisation d'un vector de string vide
    
    public:
        bool isEmpty() const; // Vérifi si la pile est vide
        void stack(std::string c); // Permet d'empiler
        std::string unstack(); // Permet de dépiler
        std::string top() const; // Renvoie la valeur au-dessu de la pile sans la dépiler
        std::string toString() const; // Renvoie la pile sous forme d'une liste
};

#endif
