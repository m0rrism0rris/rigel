#ifndef LEXER_DEFS
#define LEXER_DEFS
//#include "defs.h"

int lex_file(const char* filename);
token next_token();
void delete(token t);
void tput(token t);
void show_rem_tokens(void);

#endif