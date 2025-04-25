#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "object.h"

#define _POSIX_C_SOURCE 200809L

#define T_NEWLINE "\n"
#define T_COMMENT '#'
#define T_OBJ_NAME "o"
#define T_VERTEX "v"
#define T_VERTEX_TEXTURE "vt"
#define T_VERTEX_NORMAL "vn"
#define T_FACE "f"


/*
	Calcul du vecteur différence de deux vecteurs
*/
void vect_diff(vector *res, vector v1, vector v2) {
	res->x = v2.x - v1.x;
	res->y = v2.y - v1.y;
	res->z = v2.z - v1.z;
}

/*
	Calcul du produit vectoriel
*/
void vect_set_prod(vector *res, vector u, vector v) {
	res->x = u.y * v.z - u.z * v.x;
	res->y = u.z * v.x - u.x * v.z;
	res->z = u.x * v.y - u.y * v.x;
}

void vect_normalize(vector *v) {
	if(v->x) v->x /= v->x;
	if(v->y) v->y /= v->y;
	if(v->z) v->z /= v->z;
}

/*
	Assignation des valeurs d'un vecteur
*/
void vect_set(vector *res, double values[3]) {
	res->x = values[0];
	res->y = values[1];
	res->z = values[2];
}

void tri_print(triangle t) {
	printf("p0 : %f %f %f\n", t.p[0].x, t.p[0].y, t.p[0].z);
	printf("p1 : %f %f %f\n", t.p[1].x, t.p[1].y, t.p[1].z);
	printf("p2 : %f %f %f\n", t.p[2].x, t.p[2].y, t.p[2].z);
	printf("n  : %f %f %f\n", t.n.x, t.n.y, t.n.z);
}

/*
	Assignation du vecteur normal
*/
void tri_set_normal(triangle *t) {
	vector u, v;

	/* Calcul des vecteurs u et v à partir des points du triangle */
	vect_diff(&u, t->p[0], t->p[1]);
	vect_diff(&v, t->p[0], t->p[2]);

	/* Calcul du produit vectoriel */
	vect_set_prod(&(t->n), u, v);
	vect_normalize(&(t->n));
}

/*
	Allocation d'un nouveau triangle et assignation de toutes ses valeurs
*/
void tri_set(triangle *v, vector p[3]) {
	v->p[0] = p[0];
	v->p[1] = p[1];
	v->p[2] = p[2];

	/* Calcul vecteur normal */
	tri_set_normal(v);
}

int char_inside(char c, char *delims) {
	while(*delims != '\0') {
		if(c == *delims++) {
			return 1;
		}
	}
	return 0;
}

void line_to_tok(char tokens[LINE_MAX_TOK][TOK_MAX_LEN], char *line) {
	int c = 0;
	int i = 0;

	while(*line != '\0') {
		/* Ignoring spaces */
		while(char_inside(*line, " \t")) {
			line++;
		}

		while(!char_inside(*line, " \t") && *line != '\0') {
			if(strcmp(line, T_NEWLINE) != 0) {
				tokens[i][c++] = *line;
			}
			line++;
		}
		tokens[i][c] = '\0';
		i++;
		c=0;
	}
	tokens[i][0] = '\0';
}

void token_to_idx(char *token, int *v, int *t, int *n) {
	char result[3][20];
	int i = 0;
	int j = 0;

	while(*token != '\0') {
		result[i][j++] = *token++;

		if(char_inside(*token, "/")) {
			result[i++][j] = '\0';
			j = 0;
			token++;
		}
	}
	*v = atoi(result[0]);
	*t = atoi(result[1]);
	*n = atoi(result[2]);
}

void vertex_to_string(char *s, vector v) {
	sprintf(s, "%f %f %f", v.x, v.y, v.z);
}

void vertex_print(vector *v, char *type) {
	printf("%s %f %f %f\n", type, v->x, v->y, v->z);
}

void object_init(object *o) {
	o->n_comments = 0;
	o->n_vertex = 0;
	o->n_textures = 0;
	o->n_normal = 0;
	o->n_faces = 0;
}

object *object_load(object *o, char *path, char *line,
						 char tokens[LINE_MAX_TOK][TOK_MAX_LEN]) {
	FILE *file = fopen(path, "r");
	vector *v;
	face *f;

	/* vertex, texture and normal indexes from obj */
	int i, i_v, i_t, i_n;
	int line_n = 0;

	if(file == NULL) {
		perror("fopen");
		return NULL;
	}

	while(fgets(line, LINE_MAX_LEN, file)) {
		line_n++;

		if(line[0] == T_COMMENT) {
			strcpy(o->comment[o->n_comments++], line);
		}
		else {
			line_to_tok(tokens, line);

			if(strcmp(T_OBJ_NAME, tokens[0]) == 0) {
				strcpy(o->name, tokens[1]);
			}
			else if(strcmp(T_VERTEX, tokens[0]) == 0) {
				v = &o->vertex[o->n_vertex++];
				v->x = atof(tokens[1]);
				v->y = atof(tokens[2]);
				v->z = atof(tokens[3]);
			}
			else if(strcmp(T_VERTEX_TEXTURE, tokens[0]) == 0) {
				v = &o->textures[o->n_textures++];
				v->x = atof(tokens[1]);
				v->y = atof(tokens[2]);
				v->z = 0;
			}
			else if(strcmp(T_VERTEX_NORMAL, tokens[0]) == 0) {
				v = &o->normal[o->n_normal++];
				v->x = atof(tokens[1]);
				v->y = atof(tokens[2]);
				v->z = atof(tokens[3]);
			}
			else if(strcmp(T_FACE, tokens[0]) == 0) {
				f = &o->faces[o->n_faces++];

				/* A face has 3 vertex  */
				for(i=0; i<3; i++) {
					/* Numbers to indexes : 1/11/2 => i_v/i_t/i_n = 0/10/1 */
					token_to_idx(tokens[i+1], &i_v, &i_t, &i_n);
					f->facettes[i].vertex = i_v - 1;
					f->facettes[i].texture = i_t - 1;
					f->facettes[i].normal = i_n - 1;
				}
			}
			/* Ignoring empty lines */
			else if(tokens[0][0] == '\0') {
				;
			}
			else {
				printf("Unrecognize symbol line %d :%s:\n", line_n, tokens[0]);
				return NULL;
			}
		}
	}

	fclose(file);

	return o;
}

object *object_save(object *o, char *path) {
	int i, j;
	char line[LINE_MAX_LEN];
	facette *fac;
	printf("Saving to file : %s\n", path);

	FILE *f = fopen(path, "w");

	if(!f) {
		perror("Error opening file in write mode");
		return NULL;
	}

	/* Comments */
	for(i=0; i<o->n_comments; i++) {
		fprintf(f, "%s", o->comment[i]);
	}
	fprintf(f, "\n");

	/* Name  */
	fprintf(f, "o %s\n", o->name);
	fprintf(f, "\n");

	/* Vertex */
	for(i=0; i<o->n_vertex; i++) {
		vertex_to_string(line, o->vertex[i]);
		fprintf(f, "v %s\n", line);
	}
	fprintf(f, "\n");

	/* textures */
	for(i=0; i<o->n_textures; i++) {
		vertex_to_string(line, o->textures[i]);
		fprintf(f, "vt %s\n", line);
	}
	fprintf(f, "\n");


	/* normals */
	for(i=0; i<o->n_normal; i++) {
		vertex_to_string(line, o->normal[i]);
		fprintf(f, "vn %s\n", line);
	}
	fprintf(f, "\n");


	/* Faces */
	for(i=0; i<o->n_faces; i++) {
		fprintf(f, "f ");
		for(j=0; j<3; j++) {
			fac = &o->faces[i].facettes[j];

			/* indexes to numbers */
			fprintf(f, "%d/%d/%d ", fac->vertex + 1, fac->texture + 1, fac->normal + 1);
		}
		fprintf(f, "\n");
	}
	fprintf(f, "\n");


	fclose(f);

	return o;
}


