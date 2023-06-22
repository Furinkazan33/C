#include <string.h>
#include <stdio.h>

char *str_rpl(char *src, char *pattern, char *rpl) {
  char *dest, *origin;
  char *found = strstr(src, pattern);

  if(!found) {
    return NULL;
  }

  /* Shifting right or left to fit new size */
  dest = found;
  origin = found + strlen(pattern);

  if(strlen(rpl) > strlen(pattern)) {
    dest += strlen(rpl);
  }
  else {
    origin -= strlen(rpl);
  }
  memmove(dest, origin, strlen(origin) + 1);

  /* Replacement */
  memcpy(found, rpl, strlen(rpl));

  return found;
}




int main(void) {
	char src[100] = "un test de string";
	char *replace = "remplacement de string";
	str_rpl(src, "string", replace);

	printf("%s\n", src);

	return 0;
}

