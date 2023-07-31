#include <cJSON.h>

#define BUF_SIZE 1024

cJSON *config_new(const char *path);
int config_save(cJSON *conf, char *path);

