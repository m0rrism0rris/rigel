/*#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"*/

#define ERR_LONGNAME "names may not exceed 256 characters in length";

FILE* in_file;
int c;

//c must be valid char
token lex_name() {
	token res;
	res.tag = TAG_NAME;
	res.data.t_name = malloc(256);
	res.data.t_name[0] = tolower(c);
	int n;
	for (n = 1; n < 255; n++) {
		c = fgetc(in_file);
		if (isalnum(c) || c == '_') {
			res.data.t_name[n] = tolower(c);
		} else break;
	}
	
	if ((n == 255) && (isalnum(c) || c == '_')) {
		free(res.data.t_name);
		res.tag = TAG_ERR;
		res.data.t_err = ERR_LONGNAME;
	} else res.data.t_name[n] = '\0';
	
	if (strcmp(res.data.t_name,"if") == 0) {
		res.tag = TAG_IF;
	} else if (strcmp(res.data.t_name,"then") == 0) {
		res.tag = TAG_THEN;
	} else if (strcmp(res.data.t_name,"else") == 0) {
		res.tag = TAG_ELSE;
	} else if (strcmp(res.data.t_name,"while") == 0) {
		res.tag = TAG_WHILE;
	} else if (strcmp(res.data.t_name,"do") == 0) {
		res.tag = TAG_DO;
	} else if (strcmp(res.data.t_name,"end") == 0) {
		res.tag = TAG_END;
	} else return res;
	
	free(res.data.t_name);
	return res;
}

void delete(token t) {
	switch(t.tag) {
		case TAG_NAME: free(t.data.t_name); break;
	}
}

void tput(token t) {
	token res;
	switch(t.tag) {
		case TAG_NAME:
		  //printf("%s ",t.data.t_name);
		  printf("(%s, if (%i)) ",t.data.t_name,strcmp(t.data.t_name,"if"));
		  break;
		case TAG_LN: printf("\\n "); break;
		case TAG_AT: printf("@ "); break;
		case TAG_INT: printf("%i ",t.data.t_int); break;
		case TAG_LOGOP ... TAG_MULOP: printf("op%c ",t.data.t_op); break;
		case TAG_EOF: printf("EOF "); break;
		case TAG_ERR: printf("syntax error: %s ",t.data.t_err); break;
		case TAG_COMMA: printf(", "); break;
		case TAG_LPAREN: printf("( "); break;
		case TAG_RPAREN: printf(") "); break;
		default: printf("unrecognized tag (%i) ",t.tag);
	}
}

token next_token() {
	token res;
	for (int i = 0; i < 99999; i++) switch(c) {
		case ' ': case '\t': c = fgetc(in_file); continue;
		
		case 'A' ... 'Z': case '_': case 'a'...'z': return lex_name();
			
		case '0' ... '9':
			res.tag = TAG_INT;
			ungetc(c,in_file);
			fscanf_s(in_file,"%i", &res.data.t_int);
			c = fgetc(in_file);
			return res;
			
		case '&': case '|': case '~':
			res.tag = TAG_LOGOP;
			res.data.t_op = c;
			c = fgetc(in_file);
			return res;
			
		case '=':
			res.tag = TAG_EQOP;
			res.data.t_op = '=';
			c = fgetc(in_file);
			return res;
			
		case '#':
			res.tag = TAG_EQOP;
			res.data.t_op = '#';
			c = fgetc(in_file);
			return res;
			
		case '<':
			res.tag = TAG_EQOP;
			c = fgetc(in_file);
			if (c == '=') {
				res.data.t_op = 'l';
				c = fgetc(in_file);
			} else res.data.t_op = '<';
			
		case '>':
			res.tag = TAG_EQOP;
			c = fgetc(in_file);
			if (c == '=') {
				res.data.t_op = 'g';
				c = fgetc(in_file);
			} else res.data.t_op = '>';
			
		case '+': case '-':
			res.tag = TAG_ADDOP;
			res.data.t_op = c;
			c = fgetc(in_file);
			return res;
			
		case '*': case '/': case '%':
			res.tag = TAG_MULOP;
			res.data.t_op = c;
			c = fgetc(in_file);
			return res;
			
		case '@':
			res.tag = TAG_AT;
			c = fgetc(in_file);
			return res;
			
		case ',':
			res.tag = TAG_COMMA;
			c = fgetc(in_file);
			return res;		
			
		case '(':
			res.tag = TAG_LPAREN;
			c = fgetc(in_file);
			return res;		
					
		case ')':
			res.tag = TAG_RPAREN;
			c = fgetc(in_file);
			return res;		
						
		case '\r': case '\n':
			c = fgetc(in_file);
			while ((c == '\r') || (c == '\n') || (c == ' ') || (c == '\t')) c = fgetc(in_file);
			if (c != -1) res.tag = TAG_LN;
			return res;
		
		case EOF:
			res.tag = TAG_EOF;
			return res;
		
		default:
			res.tag = TAG_ERR;
			res.data.t_err = "unrecognized character";
	}
	res.tag = TAG_EOF;
	return res;
}

int lex_file(const char* filename) {
	int status = fopen_s(&in_file,filename,"r");
	if(status != 0) return status;
	c = fgetc(in_file);
	return 0;
}

void show_rem_tokens(void) {
	token tok = next_token();
	tput(tok);
	while (tok.tag >= 0) {
		tok = next_token();
		tput(tok);
		delete(tok);
	}
}
