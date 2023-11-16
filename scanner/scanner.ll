%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <string>
# include "parser.hpp"
# include "calc++.hpp"
  
extern calc::location location;
extern bool trace_scanning;
calc::parser::symbol_type check_res(std::string yytext, calc::location l);


// Work around an incompatibility in flex (at least versions
// 2.5.31 through 2.5.33): it generates code that does
// not conform to C89.  See Debian bug 333231
// <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
# undef yywrap
# define yywrap() 1

// Pacify warnings in yy_init_buffer (observed with Flex 2.6.4)
// and GCC 7.3.0.
#if defined __GNUC__ && 7 <= __GNUC__
# pragma GCC diagnostic ignored "-Wnull-dereference"
#endif
%}

%option noyywrap nounput batch debug noinput

id    [a-zA-Z][a-zA-Z_0-9]*
int   [0-9]+
blank [ \t]

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%%
%{
  // A handy shortcut to the location held by the driver.
  calc::location& loc = location;
  // Code run each time yylex is called.
  loc.step ();
%}
{blank}+   loc.step ();
[\n]+      loc.lines (yyleng); loc.step ();

"-"      return calc::parser::make_MINUS  (loc);
"+"      return calc::parser::make_PLUS   (loc);
"*"      return calc::parser::make_STAR   (loc);
"/"      return calc::parser::make_SLASH  (loc);
"("      return calc::parser::make_LPAREN (loc);
")"      return calc::parser::make_RPAREN (loc);
"="      return calc::parser::make_ASSIGN (loc);
"^"      return calc::parser::make_POWER  (loc);

{int}      {
  errno = 0;
  long n = strtol (yytext, NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw calc::parser::syntax_error (loc, "integer is out of range: "
                                    + std::string(yytext));
  return calc::parser::make_NUMBER (n, loc);
}
{id}       return check_res(yytext,loc);
.          {
             throw calc::parser::syntax_error
               (loc, "invalid character: " + std::string(yytext));
}
<<EOF>>    return calc::parser::make_END (loc);
%%

calc::parser::symbol_type check_res(std::string yytext, calc::location l) {
  if (yytext == "sqrt") return calc::parser::make_SQRT(l);
  else return calc::parser::make_IDENTIFIER (yytext, l);
};

void scan_begin (const std::string &file)
{
  yy_flex_debug = trace_scanning;
  if (file.empty () || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
    {
      std::cerr << "cannot open " << file << ": " << strerror(errno) << '\n';
      exit (EXIT_FAILURE);
    }
}

void scan_end ()
{
  fclose (yyin);
}
