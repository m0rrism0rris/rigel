#ifndef RG_DEFS
#define RG_DEFS

#define TAG_ERR -2
#define TAG_EOF -1
#define TAG_NAME 0
#define TAG_LN 1
#define TAG_AT 2

#define TAG_INT 4
#define TAG_FLOAT 5

#define TAG_COMMA 16
#define TAG_LPAREN 17
#define TAG_RPAREN 18
#define TAG_ASSIGN 19

#define TAG_IF 32
#define TAG_THEN 33
#define TAG_ELSE 34
#define TAG_WHILE 35
#define TAG_DO 36
#define TAG_END 37

#define TAG_LOGOP 128
#define TAG_EQOP 129
#define TAG_ADDOP 130
#define TAG_MULOP 131

typedef struct token {
	int tag;
	union {
		int t_int;
		double t_float;
		char t_op;
		char* t_name;
		char* t_err;
	} data;
} token;

#endif