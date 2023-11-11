#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "defs.h"
#include "lexer.h"

#include "lexer.c"

#define ETAG_BINOP 1
#define ETAG_UNOP  2
#define ETAG_FCALL 3
#define ETAG_INTLIT 4
#define ETAG_NAME 5

//struct expr;

typedef struct s_expr expr;

typedef struct binop {
	struct expr* left;
	struct expr* right;
	char op;
} binop;

typedef struct unop {
	expr* arg;
	char op;
} unop;

typedef struct s_expr {
	int tag;
	int result_type;
	union {
		binop bop;
		unop uop;
		//fcall fc;
		int lit;
		char* name;
	} t;
} expr;

token tk;

expr parse_primary() {
	expr res;
	switch(tk.tag) {
		case TAG_NAME:
			res.tag = ETAG_NAME;
			res.t.name = tk.data.t_name;
			break;
			
	}
	//tk = next_token();
	return res;
}
 
int main(int argc, char *argv[]) {
	if (argc == 1) {
		printf("please provide a file");
		return -1;
	}
	if (argc > 2) {
		printf("please provide exactly one file");
		return -1;
	}
	int status = lex_file(argv[1]);
	if(status != 0) {
		printf ("unable to open file \"%s\"",argv[1]);
		return -1;
	}
	show_rem_tokens();
	return 0;
}
