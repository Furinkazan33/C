#include <cJSON.h>

#define BUF_SIZE 1024

cJSON *config_new(const char *path);
cJSON *config_save(cJSON *conf, char *path);

