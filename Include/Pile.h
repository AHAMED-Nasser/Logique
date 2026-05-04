#ifndef PILE_H
#define PILE_H

#include <vector>
#include <string>

class Pile {
    private :
        std::vector<char> pile;
    
    public:
        bool isEmpty() const;
        void stack(char c);
        char unstack();
        char top() const;
        std::string toString() const;
};

#endif
