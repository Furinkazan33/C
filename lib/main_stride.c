#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct player {
	int id;
	char pseudo[20];
	char name[40];
} player;

void player_init(player *p, int i) {
	p->id = i;
	strcpy(p->pseudo, "Pseudo");
	strcpy(p->name, "My_name");
}

void player_print(player *p, int n) {
	printf("%d-%d-%s-%s\n", n, p->id, p->name, p->pseudo);
}

void player_inc_id(player *p, int __attribute__((unused)) n) {
	p->id++;
}

void players_do(void *p, void (func)(player *, int), int size, int n) {
	char *pc = (char *)p;

	for(int i = 0; i < n; i++) {
		func((player *)pc, i);
		pc += size;
	}
}


int main(void) {
	player *p = (player *)malloc(sizeof(player) * 10);

	players_do(p, player_init, sizeof(player), 10);
	players_do(p, player_print, sizeof(player), 10);

	puts("");

	players_do(p, player_inc_id, sizeof(player), 10);
	players_do(p, player_print, sizeof(player), 10);

	free(p);

	return 0;
}
