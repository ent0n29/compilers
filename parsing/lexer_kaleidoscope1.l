%{
#include <stdexcept>
#include <map>
#include <cstring>
#include <string>
#include "tokens.h"
#include "symbtbl.h"

// Forward declaration
TokenType check_ide(std::string tok);
void illegal(char c);

// Definitions used by the parser
std::map<const int,Token*> symbtbl;
int symbtblptr;

// Local only variables
Token *t;
int count = 0;

%}

ide     [a-zA-Z][a-zA-Z0-9]*
fpnum   [0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?
fixnum  (0|[1-9][0-9]*)\.?[0-9]*
num     {fpnum}|{fixnum}
%option noyywrap

%%
[ \t\n]   { /* Ignoriamo i separatori */ }
"def"     { return tok_def; }
"extern"  { return tok_extern; }
{ide}     { t = new Token;
            t->toktype=tok_id;
            strcpy(t->id, yytext);
            symbtblptr=++count;
            symbtbl[symbtblptr]=t;
	    return tok_id; }
{num}     { t = new Token;
            t->toktype=tok_number;
            t->num=std::stof(yytext);
            symbtblptr=++count;
            symbtbl[symbtblptr]=t;
	    return tok_number; }
"*"       { return int('*'); }
"+"       { return int('+'); }
"("       { return int('('); }
")"       { return int(')'); }
","       { return int(','); }
";"       { return int(';'); }
<<EOF>>   { return tok_eof; }
.         { illegal(yytext[0]); }

%%

void illegal(char c) {
   throw std::invalid_argument( "Trovato un carattere illegale" );
}
