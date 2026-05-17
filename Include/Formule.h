#ifndef FORMULE_H
#define FORMULE_H

#include "Pile.h"
#include "logique.h"
#include <string>
#include <vector>

class Formule {
    
    private:
        std::string formule;

    public:
        std::string toPostfixe();
        std::string toPrefixe();
        std::string toString();
        
        std::string nnf();
        std::string implFree();
        std::string morgan();

        std::string implFreeNnf();
        std::string morganNnf();

        std::string cnf();

        std::string getFormule();
        void setFormule(const std::string& formule);
};

#endif