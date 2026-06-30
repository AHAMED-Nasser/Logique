#include "logique.h"
#include "stdexcept"
#include "Pile.h"
#include <string>
#include <vector>
#include <utility>

using namespace std;

/**
 * Nous dit si un opérateur est prioriète ou non.
 */
int precedence(char c) {
    if (c == '#' || c == '%' || c == '-') return 1; // opérateur unitaire plus prioritère
    if (c == '&' || c == '|' || c == '>') return 0; // opérateur binaire moins prioritère
    return -1;
}

/**
 * Nous dit si le caractère en cours d'étude est une opérateur
 */
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
// DEBUT
string strip(const string& s) {
    string striped_str;
    for (unsigned int i = 0; i < s.size(); ++i) {
        if (s[i] != ' ') {
            striped_str += s[i];
        }
    }

    return striped_str;
}

/**
 * Retourne une chaîne de caractère
 */
string reversed(const string& str) {
    string final_str(str.rbegin(), str.rend());
    return final_str;
}

// FIN

/**
 * Transforme une implication en disjonction (ex: "a > b" -> "-a | b")
 */
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

/**
 * Renvoie une étape de la formule appliquée à la loi de De Morgan 
 */
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

/**
 * Renvoie l'entièreté de la formule appliquée à la loi de De Morgan
 */
string morgan(string formule) {
    string previewFormula = "";
    string newFormula = strip(formule);

    while (newFormula != previewFormula) {
        previewFormula = newFormula;
        newFormula = morganStape(previewFormula);
    }

    return newFormula;
}

/**
 * Renvoie la formule avec les négation simplifié (ex: "--a" -> "a" ou encore "---a" -> "-a")
 */
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

/**
 * Renvoie la formule après la distributivité
 */
string blocDistrib(string left, string right) {
    left = strip(left);
    right = strip(right);

    if (left.front() == '(' && left.back() == ')') left = left.substr(1, left.size() - 2);
    if (right.front() == '(' && right.back() == ')') right = right.substr(1, right.size() - 2);

    // --- ANALYSE DU BLOC GAUCHE AVEC UNE PILE ---
    Pile leftStack;
    int indexAndLeft = -1;
    for (unsigned int i = 0; i < left.size(); ++i) {
        char c = left[i];
        if (c == '(') {
            leftStack.stack("(");
        } else if (c == ')') {
            if (!leftStack.isEmpty()) leftStack.unstack();
        } else if (c == '&' && leftStack.isEmpty()) { 
            // Si la pile est vide, on est au niveau 0 du bloc gauche !
            indexAndLeft = i;
            break;
        }
    }

    if (indexAndLeft != -1) {
        string A = left.substr(0, indexAndLeft);
        string B = left.substr(indexAndLeft + 1);
        return "(" + blocDistrib(A, right) + "&" + blocDistrib(B, right) + ")";
    }

    // --- ANALYSE DU BLOC DROITE AVEC UNE PILE ---
    Pile rightStack;
    int indexAndRight = -1;
    for (unsigned int i = 0; i < right.size(); ++i) {
        char c = right[i];
        if (c == '(') {
            rightStack.stack("(");
        } else if (c == ')') {
            if (!rightStack.isEmpty()) rightStack.unstack();
        } else if (c == '&' && rightStack.isEmpty()) {
            // Si la pile est vide, on est au niveau 0 du bloc droite !
            indexAndRight = i;
            break;
        }
    }

    if (indexAndRight != -1) {
        string C = right.substr(0, indexAndRight);
        string D = right.substr(indexAndRight + 1);
        return "(" + blocDistrib(left, C) + "&" + blocDistrib(left, D) + ")";
    }

    // Cas de base
    return "(" + left + "|" + right + ")";
}


string cnfStape(string formule) {
    formule = strip(formule);
    // formule = nnf(morgan(implFree(formule)));
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

            // Extraction des composants internes des parenthèses courants
            string phi1 = "";
            string phi2 = "";
            string op = "";
            bool trouve_op = false;

            while (!subStack.isEmpty()) {
                string element = subStack.unstack();
                if ((element == "&" || element == "|") && !trouve_op) {
                    op = element;
                    trouve_op = true;
                    continue;
                }

                if (!trouve_op) phi1 += element;
                else phi2 += element;
            }

            string subFormula;
            // Si l'opérateur principal extrait est un OU '|', on tente de distribuer
            if (trouve_op && op == "|") {
                subFormula = blocDistrib(phi1, phi2);
            } else if (trouve_op) {
                subFormula = "(" + phi1 + op + phi2 + ")";
            } else {
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

    return cleanParenthese(finalResult);
}


string cnf(string formule) {
    string ancienne = "";
    string nouvelle = strip(formule);
    nouvelle = nnf(morgan(implFree(nouvelle)));

    // Boucle de point fixe : tant que la chaîne est modifiée, on redistribue
    while (nouvelle != ancienne) {
        ancienne = nouvelle;
        nouvelle = cnfStape(ancienne);
    }

    return cleanParenthese(nouvelle);
}


vector<vector<string>> cnfList(string formule) {
    formule = strip(formule);

    Pile mainStack;
    vector<vector<string>> list;

    for (unsigned int i = 0; i < formule.size(); ++i) {

        char c = formule[i];

        if (c != '&') {
            mainStack.stack(string(1, c));
        }
        else {
            Pile subStack;
            vector<string> subList;
            string literal = "";

            while (!mainStack.isEmpty()) {
                if (mainStack.top() == "(" || mainStack.top() == ")") {
                    mainStack.unstack();
                }
                else {
                    subStack.stack(mainStack.unstack());
                }
            }

            string element;
            while (!subStack.isEmpty()) {
                element = subStack.unstack();

                if (element != "|") {
                    literal += element;
                }
                else {
                    subList.push_back(literal);
                    literal = "";
                }
            }

            subList.push_back(literal);
            list.push_back(subList);
        }
    }

    if (!mainStack.isEmpty()) {
        Pile subStack;
        vector<string> subList;
        string literal = "";

        while (!mainStack.isEmpty()) {
            if (mainStack.top() == "(" || mainStack.top() == ")") {
                mainStack.unstack();
            }
            else {
                subStack.stack(mainStack.unstack());
            }
        }

        while (!subStack.isEmpty()) {
            string element = subStack.unstack();

            if (element != "|") {
                literal += element;
            }
            else {
                subList.push_back(literal);
                literal = "";
            }
        }

        subList.push_back(literal);
        list.push_back(subList);
    }

    return list;
}


string negation_formula(string formule) {
    formule = strip(formule);

    return cnf("-(" + formule + ")");
}


vector<vector<string>> cnfListNegation(string formule) {
    return cnfList(negation_formula(formule));
} 


string cleanParenthese(string formule) {
    formule = strip(formule);

    // Suppression des parenthèses redondantes autour de la formule entière
    if (formule.front() == '(' && formule.back() == ')') {
        int count = 0;
        bool redundant = true;

        for (size_t i = 0; i < formule.size(); ++i) {
            if (formule[i] == '(') count++;
            else if (formule[i] == ')') count--;

            // Si on trouve une parenthèse fermante avant d'avoir fermé la première ouvrante, ce n'est pas redondant
            if (count == 0 && i < formule.size() - 1) {
                redundant = false;
                break;
            }
        }

        if (redundant) {
            return formule.substr(1, formule.size() - 2);
        }
    }

    return formule;
}
