# Documentation Technique - Moteur de Transformation Logique

Ce projet implémente un moteur de traitement et de normalisation d'expressions logiques propositionnelles écrit en C++ et interfacé avec Python. Le système transforme des formules logiques complexes en Forme Normale Conjonctive (CNF) textuelle puis structurelle, permettant un export au format DIMACS pour une résolution par le solveur MiniSAT.

---

## Sommaire

1. [Architecture des Classes (C++)](#1-architecture-des-classes-c)
    * [Classe Pile](#classe-pile)
    * [Classe Formule](#classe-formule)
2. [Fonctions Algorithmiques Globales (C++)](#2-fonctions-algorithmiques-globales-c)
3. [Interface de Liaison (Pybind11)](#3-interface-de-liaison-pybind11)
4. [Pipeline d'Orchestration (Python)](#4-pipeline-dorchestration-python)

---

## 1. Architecture des Classes (C++)

### Classe Pile
Structure de base d'une Pile prenant des chaîne de caratère.

*   `void stack(std::string element)` : Ajoute une chaîne de caractères au sommet de la pile.
*   `std::string unstack()` : Retire et renvoie l'élément situé au sommet de la pile.
*   `std::string top()` : Renvoie l'élément au sommet sans le supprimer de la pile.
*   `bool isEmpty()` : Indique si la pile est vide.

### Classe Formule
Classe principale menant à la convertion en forme normal conjonction sous forme d'une liste de clauses.

*   `void setFormule(std::string f)` : Assigne une nouvelle chaîne de caractères à l'attribut contenant la formule.
*   `std::string cnf()` : Méthode principale exécutant le pipeline complet de normalisation : simplification des négation, élimination des implications, application des lois de De Morgan, puis distribution itérative des opérateurs.
*   `std::vector<std::vector<std::string>> cnfList()` Cette méthode va simplement reprendre l'expression sous forme CNF et va en faire une liste de clauses. <u>C'est cette liste qui sera utilisé pour facilité la génération du fichier que souhaite MiniSAT</u>.

---

## 2. Fonctions Algorithmiques Globales (C++)

Ces fonctions effectuent les transformations mathématiques directement sur les structures de chaînes de caractères.

*   `std::string nnf(std::string formule)` : Parcourt l'expression pour éliminé les doubles négations.
*   `std::string impl_free(std::string formule)` : Supprime les connecteurs d'implication (`>`) en appliquant l'équivalence logique : A implique B devient (non A) ou B.
*   `std::string morgan(std::string formule)` : Applique de manière itérative les lois de De Morgan afin de distribuer et descendre les symboles de négation (`-`) directement devant les variables atomiques.
*   `std::string cnfStape(std::string formule)` : Analyse l'expression à l'aide d'une pile. Dès qu'une sous-formule fermée est détectée, elle en extrait les composants et applique la distribution si l'opérateur principal extrait est une disjonction.
*   `std::string blocDistrib(std::string left, std::string right)` : Gère la distributivité de l'opérateur OU (`|`) sur l'opérateur ET (`&`). Elle fragmente les blocs au niveau d'imbrication 0 pour appliquer l'équivalence : A ou (B et C) devient (A ou B) et (A ou C).
*   `std::vector<std::vector<std::string>> cnfList(std::string formule)` : Analyse la chaîne CNF finale pour en extraire la structure algébrique. Les expressions séparées par des connecteurs ET (`&`) forment des sous-listes (les clauses), qui contiennent les différents littéraux.

---

## 3. Interface de Liaison (Pybind11)

Le module de liaison `binding.cxx` expose les composants C++ à l'interpréteur Python sans surcoût d'exécution.

*   Liaison de la classe `Formule` avec exposition des méthodes `set_formule`, `getFormule`, `cnf` et `cnfList`.

---

## 4. Pipeline d'Orchestration (Python)

Le fichier `minisat_fonctions.py` gère la logistique d'entrée, la conversion numérique des littéraux et l'interfaçage système avec le solveur externe.

*   `generer_dimacs(raw_formula: str, in_file="input.cnf")` : Récupère la structure de clauses générée par le code C++, recense l'ensemble des variables uniques pour leur attribuer un index numérique entier positif (1, 2, 3...) et produit le fichier au format standardisé `.cnf`. Le fichier inclut l'en-tête DIMACS standard (`p cnf [nombre_variables] [nombre_clauses]`) et chaque clause se termine par le marqueur `0`.
*   `executer_minisat(input_file="input.cnf", result_file="output.txt")` : Appelle l'exécutable système `minisat` via un sous-processus, intercepte les codes de retour spécifiques du solveur (10 pour SAT, 20 pour UNSAT) et retourne la satifiabilité de l'expression.
*   `show_satisfiability(expression: str)` :
Exécute les deux fonctions précédente et renvoie le résultat de la satisfiabilité de l'expression (SAT ou UNSAT)

Le fichier `many_minisat.py` fait exactement la même chose que le fichier `minisat_fonctions.py`, mais étendu à l'échelle de plusieurs expressions. Cette fois-ci, on teste la satisfiabilité de plusieurs expressions données dans un ou plusieurs fichiers.

*   `build_expressions_list(expression_file: str)` :
Cette fonction prend en paramètre le chemin d'un fichier texte contenant des expressions logiques, chacune séparée par des virgules. Son but est de retourner une liste contenant toutes les expressions du fichier.
*   `return_expressions_satisfiability(expressions_files: list)` :
Cette fonction prend en paramètre une liste de fichiers texte contenant chacun des expressions logiques. Le but de cette fonction est de renvoyer la satisfiabilité finale de l'ensemble de ces fichiers. Par exemple, si au moins un fichier est insatisfiable, on renvoie `UNSAT`, sinon on renvoie `SAT`.

Voici un exemple d'exécution de la dernière fonction
```python
# fichier expressions1.txt -> SAT
# fichier expressions2.txt -> SAT
# fichier expression3.txt -> UNSAT

expressions = ["expressions1.txt", "expressions2.txt", "expressions3.txt"]
print(return_expressions_satisfiability(expressions))
```
Donc le résultat sera
```bash
UNSAT
```

> Il faut noter que ces algorithmes n'ont pas été pensés pour être optimaux dès le départ. Donc, un grand nombre de fichiers testés peut possiblement augmenter le temps d'exécution du processeur.