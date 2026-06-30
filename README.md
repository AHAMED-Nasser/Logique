# Documentation Technique - Moteur de Transformation Logique

Ce projet implémente un moteur de traitement et de normalisation d'expressions logiques propositionnelles écrit en C++ et interfacé avec Python. Le système transforme des formules logiques complexes en Forme Normale Conjonctive (CNF) textuelle puis structurelle, permettant un export au format DIMACS pour une résolution par le solveur MiniSAT.

---

## Introduction

Ce projet tel qu'il est actuellement à pour simple but de résoudre l'inférence suivante :

$$ K \vdash_{X} f$$
$$"f \text{ est une déduction logique de } K \text{ sachant les informations exceptionnels } X"$$

## Sommaire
1. [Utilisation et exemple](#1-utiliation)

2. [Architecture des Classes (C++)](#2-architecture-des-classes-c)
    * [Classe Pile](#classe-pile)
    * [Classe Formule](#classe-formule)
3. [Fonctions Algorithmiques Globales (C++)](#3-fonctions-algorithmiques-globales-c)
4. [Interface de Liaison (Pybind11)](#4-interface-de-liaison-pybind11)
5. [Pipeline d'Orchestration (Python)](#5-pipeline-dorchestration-python)

---

## 1. Utilisation

A la fin du fichier ``inference.py``, on va retrouver cette ligne ``print(verify_X_implication("file_K.txt", "file_f.txt", "file_X.txt"))``.

Ce ``print`` va afficher dans la console le résultat de la fonction ``verify_X_implication``, ``True`` ou ``False`` en fonction des informations qu'il recevera en paramètre.

- ``True`` : l'inférence est validé
- ``False`` : l'inférence n'est pas validé

**Prenons l'exemple suivant :**

$K = \{café, (gasoil \rightarrow \lnot miam)\}$
$f = miam$
$X = \{\lnot miam\}$

``file_K.txt``
````
cafe, (gasoil > -miam)
````
``file_f.txt``
````
miam
````
``file_X.txt``
````
-miam
````

$K$ : Base de connaissance initial.
$f$ : "Théorème" à déduire.
$X$ : Tout les informations exceptionnels (c'est les résultat qu'on ne veut pas obtenir).

On inscrit ces trois formules dans trois fichier différent, qui vont être donner à la fonction ``verify_X_implication(file_K, file_f, file_X)`` qui est responsable de la résolution de l'inférence.

Dans notre exemple la fonction va renvoyé ``True`` car il n'y a pas de gasoil et donc on peut déduire que le $café$ sera bon par défaut.

Mais en rajoutant du gasoil ($K \cup \{gasoil\}$) la fonction renvoie ``False``, car il y a du $gasoil$ et donc on ne pas déduire que le $café$ sera bon.

## 2. Architecture des Classes (C++)

### Classe Pile
Structure de base d'une Pile prenant des chaîne de caratère.

*   `void stack(std::string element)` : Ajoute une chaîne de caractères au sommet de la pile.
*   `std::string unstack()` : Retire et renvoie l'élément situé au sommet de la pile.
*   `std::string top()` : Renvoie l'élément au sommet sans le supprimer de la pile.
*   `bool isEmpty()` : Indique si la pile est vide.

### Classe Formule
Classe principale menant à la convertion en forme normal conjonction sous forme d'une liste de clauses.

Voici les méthodes que cette classe contient.

| méthode | description |
|:--------|:-----------:|
|`void setFormule(std::string f)`|Assigne une nouvelle chaîne de caractères à l'attribut contenant la formule.|
|`string getFormule()`| Renvoie la formule brute.|
|`string negation_formula()`|Renvoie la formule sous forme cnf mais négative.|
|`string nnf()` | Renvoie la formule avec les négations simplifié.|
|`string implFree()` |Renvoie la formule après simplification des implications, sans simplication des négations.|
|`string morgan()` | Renvoie la formule après application de la loi de De Morgan, sans simplification des négations.|
|`string implFreeNnf()` |Renvoie le résultat de `implFree`, avec simplification des négations.|
|`string morganNnf()` | Renvoie le résultat de `morgan`, avec simplification des négations.|
|`string cnf()` | Méthode principale exécutant le pipeline complet de normalisation : simplification des négation, élimination des implications, application des lois de De Morgan, puis distribution itérative des opérateurs.|
| `vector<vector<string>> cnfList()`|Cette méthode va simplement reprendre l'expression sous forme CNF et va en faire une liste de clauses. <u>C'est cette liste qui sera utilisé pour facilité la génération du fichier que souhaite MiniSAT</u>.|
|`vector<vector<string>> cnfListNegation()`|Renvoie la négation de la forme `cnfList`.|

---

## 3. Fonctions Algorithmiques Globales (fichier `logique.cxx`)

Ces fonctions effectuent les transformations mathématiques directement sur les structures de chaînes de caractères.

| fonctions | description |
|:--------|:-----------:|
|`string implFree(string formule)`|Supprime les connecteurs d'implication (`>`) en appliquant l'équivalence logique : $A \rightarrow B \Leftrightarrow \lnot A \lor B$|
|`string nnf(string formule)`| Parcourt l'expression pour éliminé les doubles négations : $\lnot\lnot A \Leftrightarrow A$|
|`morganStape(string formule)` | Applique une étape de la loi de De Morgan : $\lnot(A \lor B) \Leftrightarrow (\lnot A \land \lnot B)$|
|`string morgan(string formule)` | Applique de manière itérative la lois de De Morgan afin de distribuer et descendre les symboles de négation (`-`) directement devant les variables atomiques : $\lnot(A \land \lnot(B \lor C)) \Leftrightarrow (\lnot A \lor (\lnot B \land \lnot C)) $|
|`string blocDistrib(string left, string right)` | Gère la distributivité de l'opérateur $\lor$ sur l'opérateur $\land$. Elle fragmente les blocs au niveau d'imbrication 0 pour appliquer l'équivalence : $A \lor (B \land C)$ devient $(A \lor B) \land (A \lor C)$.|
|`string cnfStape(string formule)` | Analyse l'expression à l'aide d'une pile. Dès qu'une sous-formule fermée est détectée, elle en extrait les composants et applique la distribution si l'opérateur principal extrait est une disjonction.|
|`string cnf(string formule)` | Renvoie la formule sous forme CNF.|
|`vector<vector<string>> cnfList(string formule)` | Analyse la chaîne CNF finale pour en extraire la structure algébrique. Les expressions séparées par des connecteurs $\land$ forment des sous-listes (les clauses), qui contiennent les différents littéraux : $(A \lor B) \land (A \lor C)$ devient `[[A, B], [A, C]]`.|
|`string negation_formula` | Renvoie la formule sous forme cnf mais négative : $A \lor (B \land C)$ devient $(\lnot A \land (\lnot B \lor \lnot C))$|
|`vector<vector<string>> cnfListNegation(string formule)` | $A \lor (B \land C)$ devient `[[-A], [-B, -C]]` |
| `string cleanParenthese(string formule)`| Fait un nettoyage des parenthèses parasites|


---

## 4. Interface de Liaison (Pybind11)

Le module de liaison `binding.cxx` permet de lier les méthodes ou classe C++ à Python sans perdre en éfficacité. C'est une sorte de minibibliothèque personnalisé qui s'appelle ici `logicEngine`. Et on a la possibilité de choisir quelles fonctions ou classes pourrons être utilisé dans le code Python. 

*   Liaison de la classe `Formule` avec exposition des méthodes `set_formule`, `getFormule`, `cnf`, `cnfList`, `negation_formula` et `cnfListNegation`.

> Note : la méthode `cnf` et `negation_formula` peuvent techniquement être retirer de la bibliothèque. Mais dans le doûte elles ont était gardés. (On ne sait jamais pour plus tard, même si on peut changer ça très facilement)

---

## 5. Pipeline d'Orchestration (Python)

Le fichier `inference.py` gère la gestion des données (fichiers txt) en entrèes, la création des fichier `DIMAC CNF` pour MiniSat, l'execution de MiniSat et l'algorithme principale permettant d'appliquer l'inférence : $K \vdash_{X} f$.

| fonction | Description |
|:---------|:-----------:|
|`build_expressions_list(expression_file: str) -> list`|Renvoi une liste contenant les expressions du fichier `expressions_file`|
|`build_clauses_list(expression_files: list[str]) -> list`|Renvoi un cnfList liant chaque formule entre eux.|
|`write_dimac_file_from_clauses(clauses: list, input_file="temp_eval.cnf") -> None`| Créer un ficher `DIMAC CNF` d'une fomule sous forme cnfList|
|`build_negation_cnfList(expression_file: str) -> list`|Construit la négation d'une formule logique donnée sous forme de cnfList.|
|`execute_minisat(input_file="input.cnf", result_file="output.txt") -> str`|Exécute MiniSAT sur un fichier DIMACS créé et lit le résultat.|
|`verify_X_implication(file_K, file_f, file_X) -> str`| Renvoie `True` si l'inférence finale est satisfiable, sinon`False` 

Voici un exemple d'exécution de la fonction.

``file_K.txt``
````
cafe, (gasoil > -miam), gasoil
````
``file_f.txt``
````
miam
````
``file_X.txt``
````
-miam
````
```python

print(verify_X_implication("file_K.txt", "file_f.txt", "file_X.txt"))
```
Le résultat sera

```bash
False
```
Car le café contient du `gasoil`, le `gasoil` va activé l'implication `(gasoil > -miam)` et MiniSat va détecter un "bug logique" qui va donc provoqué un résultat insatisfiable(`UNSAT`). Cela veut dire qu'à un certain moment MiniSat n'a pas trouver d'affectation qui pourrait satisfaire la formule. Donc on ne peut pas dire que `miam` est envisageable dans ces conditions.

> Il faut noter que ces algorithmes n'ont pas été pensés pour être optimaux dès le départ. Donc, un très grand nombre de formules testés peut possiblement augmenter le temps d'exécution du processeur. Par exemple la fonction `blocDistrib` n'est pas du tout optimiser. Elle est à revoir.