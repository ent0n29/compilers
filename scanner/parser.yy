%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.2"
%defines

%define api.namespace {calc}
%define api.location.file none
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  #include <string>
  #include <map>
  #include <cmath>
}

%locations

%define parse.trace
%define parse.error verbose

%code {
  #include "calc++.hpp"
  extern std::map<std::string,int> variables;
  extern calc::location location;
  extern int result;
}

%define api.token.prefix {TOK_}
%token
  END  0  "end of file"
  ASSIGN  "="
  MINUS   "-"
  PLUS    "+"
  STAR    "*"
  SLASH   "/"
  LPAREN  "("
  RPAREN  ")"
  POWER   "^"
  SQRT    "sqrt"
;

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%nterm  <int> exp

%printer { yyoutput << $$; } <*>;

%%
%start unit;
unit: assignments exp  { result = $2; };

assignments:
  %empty                 {}
| assignments assignment {};

assignment:
  "identifier" "=" exp { variables[$1] = $3; };

%left "+" "-";
%left "*" "/";
%right "^";

exp:
  exp "+" exp   { $$ = $1 + $3; }
| exp "-" exp   { $$ = $1 - $3; }
| exp "*" exp   { $$ = $1 * $3; }
| exp "/" exp   { $$ = $1 / $3; }
| exp "^" exp   { $$ = pow($1,$3); }
| "(" exp ")"   { $$ = $2; }
| "identifier"  { $$ = variables[$1]; }
| "number"      { $$ = $1; };
| "sqrt" "(" exp ")" { $$ = sqrt($3); }
%%

void
calc::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
