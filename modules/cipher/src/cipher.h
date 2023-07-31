
int base_to_decimal(char *word, int base);
char *decimal_to_base(int decimal, int base);
char *base_to_base(char *word, int from, int to);

void encode(char *dest, char *text, int base, int *n);
void decode(char *dest, char *text, int base, int *n);


