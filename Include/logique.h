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

bool starts_with(const std::string& str, const std::string& paterne);

bool ends_with(const std::string& str, const std::string& paterne);

std::string reversed(const std::string& str);

std::string string_between(const std::string& str, int start, int end);
// FIN

std::string impl_free(std::string formule);

std::pair<int, char> get_main_op_morgan(const std::string& formule);

std::string morgan(std::string formule);


#endif