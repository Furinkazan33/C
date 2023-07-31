CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

OBJ=/usr/local/obj
SRC=/usr/local/src
INC=/usr/local/include
LIB=/usr/local/lib
RES=/usr/local/ressources


# $@ Le nom de la cible
# $< Le nom de la première dépendance
# $^ La liste des dépendances
# $? La liste des dépendances plus récentes que la cible
# $* Le nom du fichier sans suffixe

all: test


./bin/main: ../build/config.o
	$(CC) $(CFLAGS) -I$(INC) \
			./src/main.c $(SRC)/config.c \
			-L $(LIB) -lcjson \
			-o $@


./log/main.log: ./bin/main
	@echo "########## Testing config #################"
	./bin/main $(RES)/config.json > ./log/main.log	|| exit 1
	@echo "########## end testing config #############"


test: ./log/main.log
	

clean: 
	rm -f ./bin/*

