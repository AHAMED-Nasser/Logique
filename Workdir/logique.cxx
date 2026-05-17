#include "logique.h"
#include "stdexcept"
#include "Pile.h"
#include <string>
#include <vector>
#include <utility>

using namespace std;

int precedence(char c) {
    if (c == '#' || c == '%' || c == '-') return 1;
    if (c == '&' || c == '|' || c == '>') return 0;
    return -1;
}

bool is_operator(char c) {
    return (c == '#' || c == '%' || c == '-' || c == '&' || c == '|' || c == '>');
}

bool is_proposition(char c) {
    return (c >= 'a' && c <= 'z');
}

/**
 * Fonction équivalente à la fonctoin strip de python,
 * qui permet de retirer tout les espace d'un string
 */
string strip(const string& s) {
    string striped_str;
    for (unsigned int i = 0; i < s.size(); ++i) {
        if (s[i] != ' ') {
            striped_str += s[i];
        }
    }

    return striped_str;
}

bool starts_with(const string& str, const string& paterne) {
    if (paterne.size() > str.size()) {
        return false;
    }

    string sub_str = string_between(str, 0, paterne.size());

    if (sub_str != paterne) return false;

    return true;
}

bool ends_with(const string& str, const string& paterne) {
    if (paterne.size() > str.size()) {
        return false;
    }

    string sub_str = string_between(str, str.size() - paterne.size(), str.size());

    if (sub_str != paterne) return false;

    return true;
}

string reversed(const string& str) {
    string final_str(str.rbegin(), str.rend());
    return final_str;
}

string string_between(const string& str, int start, int end) {
    if (start < 0 || end > static_cast<int>(str.size()) || start > end) {
        throw out_of_range("Invalid start or end indices");
    }

    return str.substr(start, end - start);
}

string implFree(string formule) {
    formule = strip(formule);

    Pile mainStack;

    for (unsigned int i = 0; i < formule.size(); ++i) {
        char c = formule[i];

        if (c != ')') {
            mainStack.stack(string(1, c));
        }
        else {
            Pile subStack;
            
            while (!mainStack.isEmpty() && mainStack.top() != "(") {
                subStack.stack(mainStack.unstack());
            }

            if (!mainStack.isEmpty() && mainStack.top() == "(") {
                mainStack.unstack();
            }

            string phi1 = "";
            string phi2 = "";
            bool operatorFinded = false;

            while (!subStack.isEmpty()) {
                string element = subStack.unstack();
                if (element == ">") {
                    operatorFinded = true;
                    continue;
                }

                if (!operatorFinded) {
                    phi1 += element;
                }
                else {
                    phi2 += element;
                }
            }

            string subFormula;
            if (operatorFinded) {
                subFormula = "(-" + phi1 + "|" + phi2 + ")";
            }
            else {
                subFormula = "(" + phi1 + ")";
            }

            mainStack.stack(subFormula);
        }
    }

    Pile reverseStack;

    while (!mainStack.isEmpty()) {
        reverseStack.stack(mainStack.unstack());
    }

    string finalResult = "";
    while (!reverseStack.isEmpty()) {
        finalResult += reverseStack.unstack();
    }
    return finalResult;
}

string morganStape(string formule) {
    formule = strip(formule);

    Pile mainStack;

    for (unsigned int i = 0; i < formule.size(); ++i) {
        char c = formule[i];

        if (c != ')') {
            mainStack.stack(string(1, c));
        }
        else {
            Pile subStack;

            while (!mainStack.isEmpty() && mainStack.top() != "(") {
                subStack.stack(mainStack.unstack()); 
            }

            if (!mainStack.isEmpty() && mainStack.top() == "(") {
                mainStack.unstack();
            }

            bool hasGlobalNegation = false;
            if (!mainStack.isEmpty() && mainStack.top() == "-") {
                hasGlobalNegation = true;
                mainStack.unstack();
            }

            string phi1 = "";
            string phi2 = "";
            string goodOperator = "";
            bool operatorFinded = false;

            while (!subStack.isEmpty()) {
                string element = subStack.unstack();

                if (element == "&" || element == "|") {
                    goodOperator = element;
                    operatorFinded = true;
                    continue;
                }

                if (!operatorFinded) {
                    phi1 += element;
                }
                else {
                    phi2 += element;
                }
            }

            string subFormula;
            if (operatorFinded && hasGlobalNegation) {
                // On applique la loi de De Morgan ici
                // -(a & b) -> (-a | -b)
                // -(a | b) -> (-a & -b)
                string newOperator = (goodOperator == "&") ? "|" : "&";

                subFormula = "(-" + phi1 + newOperator + "-" + phi2 + ")";
            }
            else if (hasGlobalNegation) {
                // s'il y a une négation "-" mais pas d'opérateur binaire (ex: -(p)), on garde la négation simple
                subFormula = "(-" + phi1 + ")";
            }
            else {
                // Pas de négation global, on conserve la structure intacte
                if (operatorFinded) {
                    subFormula = "(" + phi1 + goodOperator + phi2 + ")";
                }
                else {
                    subFormula = "(" + phi1 + ")";
                }
            }

            mainStack.stack(subFormula);
        }
    }

    Pile reverseStack;
    while (!mainStack.isEmpty()) {
        reverseStack.stack(mainStack.unstack());
    }

    string finalResult = "";
    while (!reverseStack.isEmpty()) {
        finalResult += reverseStack.unstack();
    }

    return finalResult;
}

string morgan(string formule) {
    string previewFormula = "";
    string newFormula = strip(formule);

    while (newFormula != previewFormula) {
        previewFormula = newFormula;
        newFormula = morganStape(previewFormula);
    }

    return newFormula;
}


string nnf(string formule) {
    formule = strip(formule);

    Pile mainStack;

    for (unsigned int i = 0; i < formule.size(); ++i) {

        char c = formule[i];

        if (c == '-' && !mainStack.isEmpty() && mainStack.top() == "-") {
            mainStack.unstack();
        }
        else {
            mainStack.stack(string(1, c));
        }
    }

    Pile reverseStack;

    while (!mainStack.isEmpty()) {
        reverseStack.stack(mainStack.unstack());
    }

    string finalResult = "";
    while (!reverseStack.isEmpty()) {
        finalResult += reverseStack.unstack();
    }

    return finalResult;
}
