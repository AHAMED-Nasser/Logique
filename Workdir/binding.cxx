#include <pybind11/pybind11.h> // Permet de créer des bindings (des fonctions personnalisées) entre C++ et Python
#include <pybind11/stl.h> // Permet de convertir les std::vector en listes Python automatiquement

// Inclusion des fichiers d'en-tête contenant la définition de la classe Formule
#include "Formule.h"
#include "logique.h"

namespace py = pybind11;

// Création d'un module Python nommé "logicEngine"

PYBIND11_MODULE(logicEngine, m) { // Le nom du module qui sera utilisé pour importer dans Python sera "logicEngine"
    m.doc() = "Module contenant une classe Formule C++ pour gérer la transformation logique d'une expression boolléenne pour SAT-Solver"; // Documentation rapide du module

    m.def("cnfListNegation", &LOGIQUE_H::cnfListNegation);
    
    py::class_<Formule>(m, "Formule") // génération de la classe Formule dans le module Python, avec comme nom "Formule"
        .def(py::init<>()) // génération du constructeur par défaut de la classe Formule
        .def("setFormule", &Formule::setFormule) // bindings de la méthode setFormule pour Python
        .def("getFormule", &Formule::getFormule) // bindings de la méthode getFormule pour Python
        .def("cnf", &Formule::cnf) // bindings de la méthode cnf pour Python
        .def("cnfList", &Formule::cnfList) // bindings de la méthode cnfList pour Python
        .def("negationFormule", &Formule::negation_formula) // bindings de la méthode negationFormule pour Python
        .def("cnfListNegation", &Formule::cnfListNegation); // bindings de la méthode cnfListNegation pour Python
}
