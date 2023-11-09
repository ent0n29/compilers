#include <iostream>
#include <FlexLexer.h>
#include <string>
#include <vector>
#include <cstring>
#include <exception>
#include <stack>
#include <map>
#include "tokens.h"
#include "symbtbl.h"
#include "pparser.hpp"
using namespace std;

extern std::map<int,Token*> symbtbl;
extern int symbtblptr;

bool isterm(int v) {
    return v<BASE_VARIABLES;
}

bool iseps(int v) {
    return v==EPS;
}

FlexLexer* lexer;
int currenttok;

bool parser() {
  std::map<int,int> production;
  std::stack<int> matching;
  int prodnum;
  matching.push(tok_eof);
  matching.push(AXIOM);
  int X = AXIOM;
  currenttok = lexer->yylex();
  while (X != tok_eof) {
    matching.pop();
    if (isterm(X)) {
      if (X==currenttok) {
	currenttok = lexer->yylex();
      } else {
	return false;
      }
    } else { // X è un non terminale
      production = parsing_table[X];
      prodnum = production[currenttok];
      if (prodnum==0) {
	std::cerr << "ERRORE: nessuna produzione specificata per la coppia (" << X << "," << currenttok << ")\n";
	return false;
      }
      std::cout << display[prodnum] << endl;
      std::vector<int> prod = prods[prodnum];
      if (not iseps(prod.at(0))) {
	for(std::vector<int>::reverse_iterator Y=prod.rbegin(); Y!=prod.rend(); Y++) {
	  matching.push(*Y);
	}
      }
    }
    X = matching.top();
  }
  if (X == tok_eof and currenttok == tok_eof) return true;
  return false;
}

int main(int argc, char *argv[])
{
  // Start parsing
  lexer = new yyFlexLexer;
  std::cout << "Starting...\n";;
  set_parsing_table();
  if (parser()) std::cout << "Accept\n";
  else  std::cout << "Reject\n";
  return 0;
}


