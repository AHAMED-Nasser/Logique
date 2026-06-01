# Nom de l'exécutable final
TARGET = app.out

# Compilateur et options
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -IInclude

# Répertoires
SRCDIR = Workdir
OBJDIR = obj

# Liste des fichiers sources et des objets
SRCS = $(wildcard $(SRCDIR)/*.cxx)
SRCS_APP = $(filter-out $(SRCDIR)/binding.cxx, $(SRCS))
OBJS = $(patsubst $(SRCDIR)/%.cxx, $(OBJDIR)/%.o, $(SRCS_APP))

# Variable dynamique pour pybind11 et python 3.13
PYBIND_INCLUDES = $(shell python3 -m pybind11 --includes)
PYBIND_SUFFIX = $(shell python3-config --extension-suffix)

SRCS_MODULE = $(filter-out $(SRCDIR)/main.cxx, $(SRCS))

# Règle par défaut
all: $(TARGET)

# Liaison de l'exécutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# --- NOUVELLE RÈGLE : MODULE PYTHON ---
# Compile tous les algos + binding.cxx en un fichier .so partagé
# --- RÈGLE POUR LE MODULE PYTHON SUR MAC ---
logicEngine: $(SRCS_MODULE)
	$(CXX) $(CXXFLAGS) -O3 -shared -fPIC -undefined dynamic_lookup $(PYBIND_INCLUDES) $^ -o $@$(PYBIND_SUFFIX)

# Compilation des fichiers objets
$(OBJDIR)/%.o: $(SRCDIR)/%.cxx | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Création du dossier objet s'il n'existe pas
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Nettoyage des fichiers générés
clean:
	rm -rf $(OBJDIR) $(TARGET) logicEngine*.so logicEngine*.pyd

.PHONY: all clean logicEngine
