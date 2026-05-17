#ifndef LOGIQUE_H
#define LOGIQUE_H

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include "Pile.h"

int precedence(char c);

bool is_operator(char c);

bool is_proposition(char c);

// Méthode python que je refais en c++
// DEBUT
std::string strip(const std::string& s);

std::string reversed(const std::string& str);

// bool starts_with(const std::string& str, const std::string& paterne);

// bool ends_with(const std::string& str, const std::string& paterne);


// std::string string_between(const std::string& str, int start, int end);
// FIN

std::string implFree(std::string formule);

std::string morganStape(std::string formule);

std::string morgan(std::string formule);

std::string nnf(std::string formule);

#endif