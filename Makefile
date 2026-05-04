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
OBJS = $(patsubst $(SRCDIR)/%.cxx, $(OBJDIR)/%.o, $(SRCS))

# Règle par défaut
all: $(TARGET)

# Liaison de l'exécutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilation des fichiers objets
$(OBJDIR)/%.o: $(SRCDIR)/%.cxx | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Création du dossier objet s'il n'existe pas
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Nettoyage des fichiers générés
clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean
