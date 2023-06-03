
#define _POSIX_C_SOURCE 200809L

/*
   3D vector or point
*/
typedef struct {
	float x;
	float y;
	float z;
} vector;

/* Calcul du vecteur différence de deux vecteurs */
void vect_diff(vector *res, vector *v1, vector *v2);

/* Calcul du produit vectoriel */
void vect_set_prod(vector *res, vector *u, vector *v);

/* Assignation des valeurs d'un vecteur */
void vect_set(vector *res, float *values);

vector *vect_new(float x, float y, float z);

/*
   3D Triangle with normal
*/
typedef struct {
	vector *p0;
	vector *p1;
	vector *p2;
	vector *n;
} triangle;

void tri_print(triangle *t);
	
/* Assignation du vecteur normal */
void tri_set_normal(triangle *t);
/* Assignation de toutes les valeurs du triangle */
void tri_set(triangle *t, float v0[3], float v1[3], float v2[3]);

/* Allocation d'un nouveau triangle et assignation de toutes ses valeurs */
triangle *tri_new(vector *p0, vector *p1, vector *p2);

/* Free memory */
void tri_delete(triangle *t);

