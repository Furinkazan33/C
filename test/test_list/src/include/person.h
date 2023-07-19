
#define p_cast(p) ((person *)p)
#define P_SF(name) person_sf name = (person_sf) { person_free, person_cmp, person_print };

typedef struct person {
	int id;
	char name[20];
} person;

void person_free(void *p);
int person_cmp(void *p1, void *p2);
void person_print(void *p);

typedef struct person_sf {
	void (*free)(void *);
	int (*cmp)(void *, void *);
	void (*print)(void *);
} person_sf;


