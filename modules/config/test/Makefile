CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=


DIR_BIN=./bin
DIR_LOG=./log

# $@ Le nom de la cible
# $< Le nom de la première dépendance
# $^ La liste des dépendances
# $? La liste des dépendances plus récentes que la cible
# $* Le nom du fichier sans suffixe

all: dirs builds tests

dirs: $(DIR_BIN) $(DIR_LOG)
builds: $(DIR_BIN)/main
tests: $(DIR_LOG)/main.log

$(DIR_BIN):
	test -d $(DIR_BIN) || mkdir -p $(DIR_BIN)

$(DIR_LOG):
	test -d $(DIR_LOG) || mkdir -p $(DIR_LOG)


$(DIR_BIN)/main: ../build/libconfig.a ./Makefile
	$(CC) $(CFLAGS) -I ../src -I ../src/include \
			./src/main.c \
			-L ../build -lconfig \
			-L ../src/lib -lcjson \
			-o $@


$(DIR_LOG)/main.log: $(DIR_BIN)/main
	@echo "########## Testing config #################"
	$(DIR_BIN)/main ./ressources/config.json > $(DIR_LOG)/main.log	|| exit 1
	@echo "########## end testing config #############"


clean: 
	rm -f $(DIR_BIN)/*
	rm -f $(DIR_LOG)/*

