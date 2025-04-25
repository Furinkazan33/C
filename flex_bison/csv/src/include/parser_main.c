#define LOG(stream, mess) fprintf(stream, "%s:%s:%d:%s\n", __FILE__, __func__, __LINE__, mess);
#define LOGE(mess) fprintf(stderr, "[ERROR] %s:%s:%d:%s\n", __FILE__, __func__, __LINE__, mess);
#define CHECK(var, mess) if(!var) { LOGE(mess); return 1; }

void yyerror(const char *s) {
	fflush(stdout);
	fprintf(stderr, "[ERROR] %s\n", s);
}
int yyparse();


int main(int argc, char **argv) {
   if (argc > 1) {
      yyin = fopen(argv[1], "r");
      if (yyin == NULL){
	 printf("syntax: %s filename\n", argv[0]);
      }
   }
   return yyparse(); // Calls yylex() for tokens.
}

