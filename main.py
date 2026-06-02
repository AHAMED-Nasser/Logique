import subprocess # permet l'execution de commande dans le terminal depuis python
import logicEngine # notre bibliothèque c++


def generate_dimacs(raw_formule, in_file="input.cnf"):
    """
        Cette fonction prend une formule logique en entrée, la transforme en format DIMACS et l'écrit dans un fichier.

        param
        -----
        raw_formule : str -> la formule logique à transformer, par exemple : "((a & b) | (c & d))"
        in_file : str -> le nom du fichier de sortie pour le format DIMACS, par défaut "input.cnf"

        return
        ------
        dict -> un dictionnaire de correspondance entre les variables de la formule et les identifiants numériques utilisés dans le format DIMACS, par exemple : {'a': 1, 'b': 2, 'c': 3, 'd': 4}

        exemple
        -------
        >>> generate_dimacs("((a & b) | (c & d))", "test.cnf")
        [*] Fichier DIMACS écrit avec succès : test.cnf
        {'a': 1, 'b': 2, 'c': 3, 'd': 4}
    """

    # 1 -> Transformation de notre formule en liste de clauses cnf
    formula = logicEngine.Formule()
    formula.setFormule(raw_formule)
    clauses = formula.cnfList()
    
    # 2 -> Identificateur des variables unique pour le DIMACS
    unique_vars = set() # un set pour éviter les doublons des variables

    for clause in clauses:
        for litteral in clause:
            name_var = litteral.replace("-", "")
            unique_vars.add(name_var)
    
    # On met tous ça dans un dictionnaire
    # ex: ['a', 'b', 'c'] va donner {'a':1, 'b':2, 'c':3}
    dict_vars = {name: i+1 for i, name in enumerate(sorted(unique_vars))}

    # 3 -> écriture du fichier DIMACS
    with open(in_file, "w") as file:
        file.write(f"c Fichier genere automatiquement avec python\n") # Le 'c' au début c'est pour commentaire
        file.write(f"c Formule originale : {raw_formule}\n")
        file.write(f"c Formule CNF : {formula.cnf()}\n")
        file.write(f"c Clauses CNF : {clauses}\n")
        file.write(f"c Correspondance des variables : {dict_vars}\n\n")
        file.write(f"p cnf {len(unique_vars)} {len(clauses)}\n")
        
        for clause in clauses:
            dimacs_row = []
            for litteral in clause:
                if litteral.startswith("-"):
                    raw_name = litteral[1:]
                    dimacs_row.append(f"-{dict_vars[raw_name]}")
                else:
                    dimacs_row.append(f"{dict_vars[litteral]}")
            
            file.write(" ".join(dimacs_row) + " 0\n")
    
    print(f"[+] Fichier DIMACS écrit avec succès : {in_file}")
    return dict_vars # le retour de ce dictionnaire est optionnel


def execute_minisat(input_file="input.cnf", result_file="output.txt"):
    """
        Exécute MiniSAT sur un fichier DIMACS et créé et lit le résultat.

        param
        -----
        input_file : str -> le nom du fichier DIMACS à utiliser comme entrée pour MiniSAT, par défaut "input.cnf"
        result_file : str -> le nom du fichier de sortie pour les résultats de MiniSAT, par défaut "output.txt"

        exemple
        -------
        >>> execute_minisat("test.cnf", "result.txt")
        [*] Lancement de MiniSAT...
        RESULTAT MINISAT : SAT
        ou
        RESULTAT MINISAT : UNSAT
    """

    try:
        print("[*] Lancement de MiniSAT...")
        subprocess.run(["minisat", input_file, result_file], capture_output=True) # capture_output=True pour éviter que les messages de minisat s'affichent dans le terminal
    except FileNotFoundError:
        print("[-] Erreur : MiniSAT n'est pas installé ou absent du PATH.")
        return
    
    try:
        # Lecture et affichage du resultat de MiniSAT (SAT ou UNSAT)
        with open(result_file, "r") as file:
            result = file.readline().strip()
            print(f"RESULTAT MINISAT : {result}")
    except FileNotFoundError:
        print("[-] Erreur : MiniSAT a tourné mais le fichier de résultat n'a pas été généré.")


if __name__ == "__main__":
    # expression = "((a & b) | (c & d))" -> SAT
    # expression = "(a & -a)" -> UNSAT
    expression = "a|b|c" # -> SAT
    
    mapping_vars = generate_dimacs(expression, "dimacs_file.cnf")
    print(f"-> Correspondance des variables : {mapping_vars}")
    execute_minisat("dimacs_file.cnf", "result.txt")
