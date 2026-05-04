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

string reversed(const std::string& str) {
    string final_str(str.rbegin(), str.rend());
    return final_str;
}

string string_between(const std::string& str, int start, int end) {
    if (start < 0 || end > static_cast<int>(str.size()) || start > end) {
        throw std::out_of_range("Invalid start or end indices");
    }

    return str.substr(start, end - start);
}

string impl_free(std::string formule) {
    formule = strip(formule);

    while (starts_with(formule, "(") && ends_with(formule, ")")) {
        int poids = 0;
        bool est_paire = true;

        for (unsigned int i = 0; i < formule.size()-1; ++i) {
            char c = formule[i];
            if (c == '(') poids++;
            else if (c == ')') poids--;

            if (poids == 0) {
                est_paire = false;
                break;
            }
        }

        if (est_paire){
            formule = strip(string_between(formule, 1, formule.size() - 1));
        } else {
            break;
        }
    }
    
    if (starts_with(formule, "-")) {
        string reste = formule.substr(1);
        return "-" + impl_free(reste);
    }

    int index_op = -1;
    char op_trouve = '\0';

    vector<char> operators = {'>', '&', '|'};

    for (char cible : operators) {
        int poids = 0;
        for (unsigned int i = 0; i < formule.size(); ++i) {
            char c = formule[i];
            if (c == '(') poids++;
            else if (c == ')') poids--;
            else if (poids == 0 && c == cible) {
                index_op = i;
                op_trouve = cible;
                break;
            }
        }

        if (index_op != -1) break;
    }

    if (index_op == -1) return formule;
    
    string phi1 = strip(formule.substr(0, index_op));
    string phi2 = strip(formule.substr(index_op+1));

    if (op_trouve == '>') {
        return "(-" + impl_free(phi1) + "|" + impl_free(phi2) + ")";
    } else {
        string (1, op_trouve);
        return "(" + impl_free(phi1) + string(1, op_trouve) + impl_free(phi2) + ")";
    }
}


pair<int, char> get_main_op_morgan(const std::string& formule) {
    vector<char> operators = {'&', '|'};

    for (char cible : operators) {
        int poids = 0;
        for (unsigned int i = 0; i < formule.size(); ++i) {
            char c = formule[i];
            if (c == '(') poids++;
            else if (c == ')') poids--;
            else if (poids == 0 && c == cible) {
                return {i, cible};
            }
        }
    }

    return {-1, '\0'};
}


string morgan(string formule) {
    formule = strip(formule);
    if (formule.size() < 0) return "";

    if (starts_with(formule, "--")) {
        return morgan(formule.substr(2));
    }

    if (starts_with(formule, "(") && ends_with(formule, ")")) {
        int poids = 0;
        bool est_paire = true;

        for (unsigned int i = 0; i < formule.size()-1; ++i) {
            char c = formule[i];
            if (c == '(') poids++;
            else if (c == ')') poids--;

            if (poids == 0) {
                est_paire = false;
                break;
            }
        }

        if (est_paire) {
            return morgan(strip(string_between(formule, 1, formule.size() - 1)));
        }
    }

    pair<int, char> main_op_morgan = get_main_op_morgan(formule);
    int index_op = main_op_morgan.first;
    char op_trouve = main_op_morgan.second;

    if (index_op != -1) {
        string phi1 = strip(string_between(formule, 0, index_op));
        string phi2 = strip(formule.substr(index_op + 1));

        return "(" + morgan(phi1) + op_trouve + morgan(phi2) + ")";
    }

    if (starts_with(formule, "-(") && ends_with(formule, ")")) {
        string contenu = strip(string_between(formule, 2,formule.size() - 1));

        pair<int, char> main_op = get_main_op_morgan(contenu);
        int index_sous_op = main_op.first;
        char sous_op = main_op.second;

        if (index_sous_op != -1) {
            string phi1 = strip(contenu.substr(0, index_sous_op));
            string phi2 = strip(contenu.substr(index_sous_op + 1));

            // Application de la loi : -(A & B) -> -A | -B
            string new_op = (sous_op == '&') ? "|" : "&";

            return "(" + morgan("-" + phi1) + new_op + morgan("-" + phi2) + ")";
        }
    }

    return formule;
}
