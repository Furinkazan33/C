
#define _POSIX_C_SOURCE 200809L

#define OBJ_MAX_COMMENTS 10
#define OBJ_MAX_COMMENT_LEN 250
#define OBJ_MAX_NAME_LEN 32
#define OBJ_MAX_VERTEX 100
#define OBJ_MAX_TEXTURES 100
#define OBJ_MAX_NORMALS 100
#define OBJ_MAX_FACES 100

/* Defines maximum obj file line length, max tokens per line and max token length */
#define LINE_MAX_LEN 80
#define LINE_MAX_TOK 4
#define TOK_MAX_LEN 20

/*
 * Vectors and triangles definition and functions
 */


/* Vector or vertex */
typedef struct {
	double x;
	double y;
	double z;
} vector;

/* Calcul du vecteur différence de deux vecteurs */
void vect_diff(vector *res, vector v1, vector v2);

/* Calcul du produit vectoriel */
void vect_set_prod(vector *res, vector u, vector v);

/* Normalize components  */
void vect_normalize(vector *v);

/* Assignation des valeurs d'un vecteur */
void vect_set(vector *res, double values[3]);


/* Triangle with normal */
typedef struct {
	vector p[3];
	vector n;
} triangle;

void tri_print(triangle t);

/* Assignation du vecteur normal */
void tri_set_normal(triangle *t);

/* Assignation de toutes les valeurs du triangle */
void tri_set(triangle *t, vector p[3]);




/*
 * Object models definitions and functions
 * */

/* indexes */
typedef struct facette {
	int vertex;
	int texture;
	int normal;
} facette;

typedef struct face {
	facette facettes[3];
} face;

typedef struct object {
	char comment[OBJ_MAX_COMMENTS][OBJ_MAX_COMMENT_LEN];
	char name[OBJ_MAX_NAME_LEN];
	vector vertex[OBJ_MAX_VERTEX];
	vector textures[OBJ_MAX_TEXTURES];
	vector normal[OBJ_MAX_NORMALS];
	face faces[OBJ_MAX_FACES];
	int n_comments;
	int n_vertex;
	int n_textures;
	int n_normal;
	int n_faces;
} object;

/* Inits the 5 counters */
void object_init(object *o);

/* Load object from file  */
object *object_load(object *o, char *path, char *line, char tokens[LINE_MAX_TOK][TOK_MAX_LEN]);

/* Save object to file */
object *object_save(object *o, char *path);


