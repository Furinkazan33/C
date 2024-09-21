#include "db.h"

typedef struct user {
	int id;
	char *email;
	char *username;
} user;

db_base *db_user_new(size_t alloc_cols, size_t alloc_lines, char *name, char *comment) {
	db_base *res = db_new_with_id(alloc_cols, alloc_lines, name, comment);
	if(!res) {
		fprintf(stderr, "db_user_new : call to db_new returned NULL\n");
		return NULL;
	}

	res = db_col_add(res, true, true, STRING, "email", "user email");
	res = db_col_add(res, true, true, STRING, "username", "user name");
	res = db_col_add(res, true, false, STRING, "password", "user password");

	if(!res) {
		fprintf(stderr, "db_user_new : call to db_col_add returned NULL\n");
		return NULL;
	}

	return res;
}

/* increment id */
array *db_user_line_new(db_base *db, char *email, char *username, char *password) {
	db_base *tmp;
	array *line = db_line_new_with_id(db);

	if(!line) {
		fprintf(stderr, "db_user_line_new : call to db_line_new_with_id returned NULL\n");
		return NULL;
	}

	tmp = db_line_set(db, line, 1, STRING, email);
	tmp = db_line_set(tmp, line, 2, STRING, username);
	tmp = db_line_set(tmp, line, 3, STRING, password);

	if(!tmp) {
		fprintf(stderr, "db_user_line_new : call to db_line_set returned NULL\n");
		return NULL;
	}
	db = tmp;

	return line;
}

db_base *db_auth_new() {

}
