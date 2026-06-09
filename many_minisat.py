"""Le but est de faire tourner plusieurs fois minisat,en lui donnant un fichier rempli d'expressions
logiques différentes et voir si les resultat final sera toujours SAT
ou UNSAT
"""

import minisat_fonctions as mf

# 1 -> Créer un fichier d'expression logique à la main

# 2 -> Faire un algortihme python qui va lire ce fichier et créé une liste contenant tout les expressions
def build_expressions_list(expression_file: str) -> list:
    with open(expression_file, 'r') as file:
        contents = file.read().strip()
        
        expressions = contents.replace('\n', ',').split(',')
                
    return [expr.strip() for expr in expressions if expr.strip()]

     
# 3 -> Renvoyer SAT ou UNSAT du fichier complet
def return_expressions_satisfiability(expressions_files: list) -> str:
    expressions = []

    # on construit une liste d'expression de chaque fichier d'expression, qui seront directement ajouter dans la liste "expressions"
    for expr in expressions_files:
        expressions.append(build_expressions_list(expr))

    # on fait tourner minisat pour chaque expression de chaque fichier d'expression, et si une expression est UNSAT, on retourne directement UNSAT, sinon on retourne SAT à la fin
    for exprs in expressions:
        for expr in exprs:
            result = mf.show_satisfiability(expr)
            if result == "UNSAT":
                print(f"UNSAT pour cette expression {expr} dans cette liste {exprs}")
                return "UNSAT"

    return "SAT"


if __name__ == "__main__":
    expressions = ["expressions1.txt", "expressions3.txt", "expressions2.txt"]
    print(return_expressions_satisfiability(expressions))