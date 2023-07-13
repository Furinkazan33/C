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

void run(void *p, void (func)(void *, int), int size, int n) {
	char *pc = (char *)p;

	for(int i = 0; i < n; i++) {
		func(pc, i);
		pc += size;
	}
}

int is_null(player *p) {
	return p->id == 0 && p->name[0] == '\0' && p->pseudo[0] == '\0';
}

int player_cmp(player *p1, player *p2) {
	return p1->id - p2->id;
}

int main(void) {
	int MAX_PLAYERS = 10;
	player *p = (player *)malloc(sizeof(player) *MAX_PLAYERS);
	memset(p, 0, sizeof(player) * MAX_PLAYERS);

	run(p, (void(*)(void *, int))player_init, sizeof(player), MAX_PLAYERS);
	run(p, (void(*)(void *, int))player_print, sizeof(player), MAX_PLAYERS);

	puts("");

	run(p, (void(*)(void *, int))player_inc_id, sizeof(player), MAX_PLAYERS);
	run(p, (void(*)(void *, int))player_print, sizeof(player), MAX_PLAYERS);

	memset(p+5, 0, sizeof(player));
	player *pp = p;
	while(pp->id != 0) {
		printf("%d\n", pp++->id);
	}

	free(p);

	return 0;
}
