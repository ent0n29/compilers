#ifndef PARSER_HH
# include "parser.hpp"
#endif

// Tell Flex the lexer's prototype ...
# define YY_DECL \
  calc::parser::symbol_type yylex ()
// ... and declare it for the parser's sake.
YY_DECL;
