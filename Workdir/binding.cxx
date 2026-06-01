#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Permet de convertire les std::vector en listes Python automatiquement
#include "Formule.h"
#include "logique.h"

namespace py = pybind11;

// Création d'un module Python nommé "logicEngine"

PYBIND11_MODULE(logicEngine, m) {
    m.doc() = "Module C++ de transformation logique pour SAT-Solver";

    py::class_<Formule>(m, "Formule")
        .def(py::init<>())
        .def("setFormule", &Formule::setFormule)
        .def("getFormule", &Formule::getFormule)
        .def("cnf", &Formule::cnf)
        .def("cnfList", &Formule::cnfList);
}
