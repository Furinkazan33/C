#include <glib.h>
#include "person.h"


int main(void) {
  person *p0 = person_new(0, 0, "toto", "adress");
  person *p1 = person_new(1, 1, "titi", "");
  person *p2 = person_new(2, 2, "tata", "");

  GArray *a = g_array_new(FALSE, FALSE, sizeof(person *));

  g_array_append_val(a, p0);
  g_array_append_val(a, p1);
  g_array_append_val(a, p2);

  person_print(g_array_index(a, person *, 1));

  g_array_free(a, TRUE); //default free on each person



 return 0;
}
