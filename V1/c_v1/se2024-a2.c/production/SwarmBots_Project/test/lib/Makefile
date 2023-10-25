#
# Monix C - Makefile du package lib des tests.
#
# @author Matthias Brun
# @author Joshua MONTREUIL : adaptation du Makefile aux besoins du projet de l'équipe ProSE A2.
#

#
# Organisation des sources.
#

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

# Gcov informations
GCDA = $(SRC:.c=.gcda)
GCNO = $(SRC:.c=.gcno)


# Inclusion depuis le niveau du package.
CCFLAGS += -I.. -I../../$(SRCDIR)

#
# Règles du Makefile.
#

.PHONY: all clean

# Compilation.
all: $(OBJ)

.c.o:
	$(CC) -c $(CCFLAGS) $< -o $@
	
# Nettoyage.
clean: clean_coverage
	@rm -f $(OBJ) $(DEP) $(GCNO)

clean_coverage:
	@rm -f $(GCDA)


-include $(DEP)

