import subprocess
import logicEngine # notre bibliothèque c++

def generate_dimacs(raw_formule, in_file="input.cnf"):

    # 1 -> Transformation de notre formule en liste de clauses
    formula = logicEngine.Formule()
    formula.setFormule(raw_formule)
    clauses = formula.cnfList()
    
    # 2 -> Identificateur des variables unique pour le DIMACS
    unique_vars = set()

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
    return dict_vars

def execute_minisat(input_file="input.cnf", result_file="output.txt"):
    # On va demander à python de lancer MiniSAT
    
    try:
        print("[*] Lancement de MiniSAT...")
        subprocess.run(["minisat", input_file, result_file], capture_output=True)
    except FileNotFoundError:
        print("[-] Erreur : MiniSAT n'est pas installé ou absent du PATH.")
        return
    
    try:
        # Lecture et affichage
        with open(result_file, "r") as file:
            result = file.readline().strip()
            print(f"RESULTAT MINISAT : {result}")
    except FileNotFoundError:
        print("[-] Erreur : MiniSAT a tourné mais le fichier de résultat n'a pas été généré.")


if __name__ == "__main__":
    expression = "((a & b) | (c & d))"
    # expression = "(a & -a)"
    
    mapping_vars = generate_dimacs(expression, "test_python_dimacs.cnf")
    print(f"-> Correspondance des variables : {mapping_vars}")
    execute_minisat("test_python_dimacs.cnf", "result.txt")
