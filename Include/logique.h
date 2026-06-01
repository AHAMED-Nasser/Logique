#ifndef LOGIQUE_H
#define LOGIQUE_H

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include "Pile.h"

// Ce fichier contient l'ensemble des fonctions permetant de faire des opérations logique.

int precedence(char c); // Retourne le niveau de priorité d'un opérateur (ex: la négation est prioritère a la conjonction)

bool is_operator(char c); // Retourne vrai si un symbole fait partie des opérateurs autorisés

bool is_proposition(char c); // Retourne vrai si un symbole fait parti de l'alphabet minuscule (de a à z)

// Méthode python refaites en c++
// DEBUT
std::string strip(const std::string& s); // Retourne une string sans espaces

std::string reversed(const std::string& str); // Renverse et renvoie une string

// FIN

std::string implFree(std::string formule); // Retourne la formule en tranformant l'implication en disjonction

std::string morganStape(std::string formule); // Retourne la formule avec UNE SEULE étape de la loi de De Morgan

std::string morgan(std::string formule); // Retourne la formule appliquée à la loi de De Morgan

std::string nnf(std::string formule); // Retourne la formule avec la négation simplifiée (NNF - Negation Normal Form)

std::string blocDistrib(std::string left, std::string right); // Retourne la formule après distribution

std::string cnfStape(std::string formule); // Retourne la formule avec UNE SEULE étape de cnf

std::string cnf(std::string formule); // Retourne la formule sous forme CNF

std::vector<std::vector<std::string>>  cnfList(std::string); // Retourne la formule CNF sous forme d'un vecteur de vecteur
// Exemple: "(a|b) & (a\c) devient {{a,b},{a,c}}"

std::string cleanParenthese(std::string formule); // Retourne la formule sans les parenthèse global au tours.

#endif