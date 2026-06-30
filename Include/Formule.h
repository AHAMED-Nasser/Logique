#ifndef FORMULE_H
#define FORMULE_H

#include "Pile.h"
#include "logique.h"
#include <string>
#include <vector>

/** La classe Formule représente une formule logique et fournit des méthodes pour la manipuler,
la convertir et l'analyser.
*/

class Formule {
    
    private:
        std::string formule;

    public:
        std::string toPostfixe(); // Retourne la formule en notation postfixe
        std::string toPrefixe(); // Retourne la formule en notation prefixe
        std::string toString(); // Retourne la formule telle quelle est écrite
        
        std::string negation_formula(); // Retourne la négation de la formule
        std::string nnf(); // Retourne la formule avec la négation simplifiée (NNF - Negation Normal Form)
        std::string implFree(); // Retourne la formule en tranformant l'implication en disjonction sans nnf appliquée
        std::string morgan(); // Retourne la formule appliquée à la loi de De Morgan sans nnf appliquée

        std::string implFreeNnf(); // Retourne la formule en tranformant l'implication en disjonction avec le nnf appliquée
        std::string morganNnf(); // Retourne la formule appliquée à la loi de De Morgan avec le nnf appliquée

        std::string cnf(); // Retourne la formule sous forme CNF
        std::vector<std::vector<std::string>> cnfList(); // Retourne la formule CNF sous forme d'un vecteur de vecteur
        // Exemple: "(a|b) & (a|c) devient {{a,b},{a,c}}"
        std::vector<std::vector<std::string>> cnfListNegation();

        std::string getFormule(); // Retourne la formule initial
        void setFormule(const std::string& formule); // Permet de mettre à jour la formule initial
};

#endif