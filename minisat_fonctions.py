import subprocess # permet l'execution de commande dans le terminal depuis python
import logicEngine # notre bibliothèque c++


def build_expressions_list(expression_file: str) -> list:
    """
    Renvoi une list contenant les expressions du fichier expressions_file

    param
    -----
    - expressions_file : str -> fichier text contenant tout les expressions

    return
    ------
    - list -> list contenant les expressions

    exemple
    -------
    >>> exemple de fichier (expressions.txt) : ((s&-v)>p),(p>a)
    >>> build_expressions_list("expressions.txt")
    ['((s&-v)>p)', '(p>a)']
    """
    with open(expression_file, 'r') as file:
        contents = file.read().strip()
        
        expressions = contents.replace('\n', ',').split(',')
                
    return [expr.strip() for expr in expressions if expr.strip()]


def build_clauses_list(expression_files: list[str]) -> list:
    """
    Renvoi un liste contenant chaque expression sous la forme cnfList

    param
    -----
    - expression_file : str -> fichier text contenant des expressions logiques

    return
    ------
    - list -> liste contenant les expression sous la forme cnfList

    exempl
    ------
    >>> exemple de fichier (expressions.txt) : ((s&-v)>p),(p>a),(a|(b&c))
    >>> build_clauses_list("expressions.txt")
    [['-s', 'v', 'p'], ['-p', 'a'], ['a', 'b'], ['a', 'c']]
    """
    final_list = []
    formula = logicEngine.Formule()

    all_formula = None

    for file in expression_files:
        all_formula = build_expressions_list(file)

        for expr in all_formula:
            formula.setFormule(expr)
            clause = formula.cnfList()

            for sub_expr in clause:
                final_list.append(sub_expr)

    return final_list


def write_dimac_file_from_clauses(clauses: list, input_file="temp_eval.cnf"):
    unique_vars = set()
    for clause in clauses:
        for litteral in clause:
            unique_vars.add(litteral.replace("-", ""))
    
    dict_vars = {name: i+1 for i, name in enumerate(sorted(unique_vars))}

    with open(input_file, "w") as file:
        file.write(f"c Fichier genere dynamiquement pour le test courant\n")
        file.write(f"p cnf {len(unique_vars)} {len(clauses)}\n")
        
        for clause in clauses:
            dimacs_row = []
            for litteral in clause:
                if litteral.startswith("-"):
                    dimacs_row.append(f"-{dict_vars[litteral[1:]]}")
                else:
                    dimacs_row.append(f"{dict_vars[litteral]}")
            file.write(" ".join(dimacs_row) + " 0\n")


def build_negation_cnfList(expression_file: str) -> list:
    """
        Construit la négation d'une formule logique donnée sous forme de cnfList.

        param
        -----
        expression_file : str -> le nom du fichier contenant la formule logique à négativer

        return
        ------
        list : la négation de la formule logique sous forme de cnfList

        exemple
        -------
        >>> exemple de fichier (expression.txt) : (a|b)
        >>> build_negation_cnfList("expression.txt")
        [['-a'], ['-b']]
    """
    formula = logicEngine.Formule()
    all_formula = build_expressions_list(expression_file)

    negation_cnfList = []

    for expr in all_formula:
        formula.setFormule(expr)
        clause = formula.cnfListNegation()

        for sub_expr in clause:
            negation_cnfList.append(sub_expr)

    return negation_cnfList

def execute_minisat(input_file="input.cnf", result_file="output.txt") -> str:
    """
        Exécute MiniSAT sur un fichier DIMACS et créé et lit le résultat.

        param
        -----
        input_file : str -> le nom du fichier DIMACS à utiliser comme entrée pour MiniSAT, par défaut "input.cnf"
        result_file : str -> le nom du fichier de sortie pour les résultats de MiniSAT, par défaut "output.txt"

        exemple
        -------
        >>> execute_minisat("input.cnf", "output.txt")
        SAT or UNSAT
    """

    try:
        subprocess.run(["minisat", input_file, result_file], capture_output=True) # capture_output=True pour éviter que les messages de minisat s'affichent dans le terminal
    except FileNotFoundError:
        print("[-] Erreur : MiniSAT n'est pas installé ou absent du PATH.")
        return
    
    try:
        # Lecture et affichage du resultat de MiniSAT (SAT ou UNSAT)
        with open(result_file, "r") as file:
            result = file.readline().strip()
    except FileNotFoundError:
        print("[-] Erreur : MiniSAT a tourné mais le fichier de résultat n'a pas été généré.")

    return result


def verify_implication(file_K, file_f, file_X) -> str:
    clauses_K = build_clauses_list([file_K])
    clauses_f = build_clauses_list([file_f])

    statics_clauses = clauses_K + clauses_f
    
    negations_X = build_negation_cnfList(file_X)

    eval_vrai = True

    for x in negations_X:
        if not eval_vrai:
            break

        # construction de la conjonction : K et f et negation(x)
        clauses_combined = statics_clauses + [x]
        # print(clauses_combined)

        # Ecriture du fichier DIMACS pour cette conjonction
        write_dimac_file_from_clauses(clauses_combined, "temp_val.cnf")


        # Execution de MiniSAT pour vérifier la satisfiabilité de la conjonction
        result = execute_minisat("temp_val.cnf", "temp_output.txt")
        print(f"Resultat MiniSAT pour (K et f et not x) : {result}")
        # if result == "SAT":
        #     print(f"Par contradiction on voit que x n'est pas une conséquence logique de K et f, donc l'implication est fausse.")

        if result == "UNSAT":
            
            K_and_X_clauses = clauses_K + [x]
            write_dimac_file_from_clauses(K_and_X_clauses, "temp_K_X.cnf")
            result_K_X = execute_minisat("temp_K_X.cnf", "temp_K_X_output.txt")
            print(f"Resultat MiniSAT pour (K et not x) : {result_K_X}")

            if result_K_X == "UNSAT":
                # print(f"Par contradiction on voit que x est une conséquence logique de K, donc l'implication est vraie.")
                pass
            else:
                eval_vrai = False
        else:
            eval_vrai = False

    return eval_vrai


if __name__ == "__main__":
    print(verify_implication("file_K.txt", "file_f.txt", "file_X.txt"))
