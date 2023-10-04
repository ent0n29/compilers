%{
#include <iostream>
using namespace std;
%}

DIGIT   [0-9]
DIGIT1  [1-9]

/* read only one input file */
%option noyywrap

%%
"+"               { cout << "        operatore <" << yytext[0] << ">" << endl; }
"-"               { cout << "        operatore <" << yytext[0] << ">" << endl; }
"="               { cout << "        operatore <" << yytext[0] << ">" << endl; }
{DIGIT1}{DIGIT}*  { cout << "           numero <" << yytext    << ">" << endl; }
.                 { cout << "Token sconosciuto <" << yytext[0] << ">" << endl; }
%%
int main(int argc, char** argv) {
    FlexLexer* lexer = new yyFlexLexer;
    lexer->yylex();
    return 0;
}