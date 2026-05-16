#ifndef PILE_H
#define PILE_H

#include <vector>
#include <string>

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
