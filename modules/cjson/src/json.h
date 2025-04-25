#include <cjson/cJSON.h>

#define BUF_SIZE 1024

cJSON *json_load(const char *path);
cJSON *json_save(cJSON *conf, char *path);

