#include <stdlib.h>
#include <triangle.h>
#include <stdio.h>

#define _POSIX_C_SOURCE 200809L

/*
	Calcul du vecteur différence de deux vecteurs
*/
void vect_diff(vector *res, vector *v1, vector *v2) {
	res->x = v2->x - v1->x;
	res->y = v2->y - v1->y;
	res->z = v2->z - v1->z;
}

/*
	Calcul du produit vectoriel
*/
void vect_set_prod(vector *res, vector *u, vector *v) {
	res->x = u->y * v->z - u->z * v->y;
	res->y = u->z * v->x - u->x * v->z;
	res->z = u->x * v->y - u->y * v->x;
}

/*
	Assignation des valeurs d'un vecteur
*/
void vect_set(vector *res, float *values) {
	res->x = values[0];
	res->y = values[1];
	res->z = values[2];
}

vector *vect_new(float x, float y, float z) {
	vector *new = malloc(sizeof(vector));
	new->x = x;
	new->y = y;
	new->z = z;
	
	return new;
}

void tri_print(triangle *t) {
	printf("p0 : %f %f %f\n", t->p0->x, t->p0->y, t->p0->z);
	printf("p1 : %f %f %f\n", t->p1->x, t->p1->y, t->p1->z);
	printf("p2 : %f %f %f\n", t->p2->x, t->p2->y, t->p2->z);
	printf("n  : %f %f %f\n", t->n->x, t->n->y, t->n->z);
}

/*
	Assignation du vecteur normal
*/
void tri_set_normal(triangle *t) {
	vector *u = malloc(sizeof(vector));
	vector *v = malloc(sizeof(vector));
	
	/* Calcul des vecteurs u et v à partir des points du triangle */
	vect_diff(u, t->p0, t->p1);
	vect_diff(v, t->p0, t->p2);
	
	/* Calcul du produit vectoriel */
	vect_set_prod(t->n, u, v);
	
	free(u);
	free(v);
}

/*
	Assignation de toutes les valeurs du triangle
*/
void tri_set(triangle *t, float v0[3], float v1[3], float v2[3]) {
	vect_set(t->p0, v0);
	vect_set(t->p1, v1);
	vect_set(t->p2, v2);
	
	/* Calcul du vecteur normal */
	tri_set_normal(t);
}

/*
	Allocation d'un nouveau triangle et assignation de toutes ses valeurs
*/
triangle *tri_new(vector *p0, vector *p1, vector *p2) {
	/* Allocations */
	triangle *new = malloc(sizeof(triangle));
	new->p0 = p0;
	new->p1 = p1;
	new->p2 = p2;
	
	/* Calcul vecteur normal */
	tri_set_normal(new);
	
	return new;
}


void tri_delete(triangle *t) {
	free(t->p0);
	free(t->p1);
	free(t->p2);
	free(t->n);
	free(t);
}
