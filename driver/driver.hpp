#ifndef DRIVER_HH
# define DRIVER_HH
# include <string>
# include <map>
# include <variant>
# include "parser.hpp"

// Dichiarazione del prototipo yylex per Flex
// Flex va proprio a cercare YY_DECL perché
// deve espanderla (usando M4) nel punto appropriato
# define YY_DECL \
  yy::parser::symbol_type yylex (driver& drv)
// Per il parser è sufficiente una forward declaration
YY_DECL;

typedef std::variant<std::string,double> lexval;
const lexval NONE = 0.0;

// Classe base dell'intera gerarchia di classi che rappresentano
// gli elementi del programma
class RootAST {
public:
  virtual ~RootAST() {};
  virtual RootAST *left() {return nullptr;};
  virtual RootAST *right() {return nullptr;};
  virtual lexval getLexVal() {return NONE;};
  virtual void visit() {};
};

// Classe che rappresenta la sequenza di statement
class SeqAST : public RootAST {
private:
  RootAST* first;
  RootAST* continuation;

public:
  SeqAST(RootAST* first, RootAST* continuation);
  RootAST *left();
  RootAST *right();
  void visit();
};

/// ExprAST - Classe base per tutti i nodi espressione
class ExprAST : public RootAST {
public:
  virtual ~ExprAST() {};
  //virtual void	 visit() {};
  //virtual ExprAST *left() {return nullptr;};
  //virtual ExprAST *right() {return nullptr;};
};

/// NumberExprAST - Classe per la rappresentazione di costanti numeriche
class NumberExprAST : public ExprAST {
private:
  double Val;

public:
  NumberExprAST(double Val);
  void visit();
  lexval getLexVal() const;
};

/// VariableExprAST - Classe per la rappresentazione di riferimenti a variabili
class VariableExprAST : public ExprAST {
private:
  std::string Name;
  
public:
  VariableExprAST(std::string &Name);
  lexval getLexVal() const;
  void visit();
};

/// BinaryExprAST - Classe per la rappresentazione di operatori binary
class BinaryExprAST : public ExprAST {
private:
  char Op;
  ExprAST* LHS;
  ExprAST* RHS;

public:
  BinaryExprAST(char Op, ExprAST* LHS, ExprAST* RHS);
  ExprAST* left();
  ExprAST* right();
  void visit();
};

/// CallExprAST - Classe per la rappresentazione di chiamate di funzione
class CallExprAST : public ExprAST {
private:
  std::string Callee;
  std::vector<ExprAST*> Args;  // ASTs per la valutazione degli argomenti

public:
  CallExprAST(std::string Callee, std::vector<ExprAST*> Args);
  lexval getLexVal() const;
  void visit();
};

/// PrototypeAST - Classe per la rappresentazione dei prototipi di funzione
/// (nome, numero e nome dei parametri; in questo caso il tipi è implicito
/// perché unico)
class PrototypeAST : public RootAST {
private:
  std::string Name;
  std::vector<std::string> Args;

public:
  PrototypeAST(std::string Name, std::vector<std::string> Args);
  lexval getLexVal() const;
  const std::vector<std::string> &getArgs() const; 
  void visit();
  int argsize();
};

/// FunctionAST - Classe che rappresenta la definizione di una funzione
class FunctionAST : public RootAST {
private:
  PrototypeAST* Proto;
  ExprAST* Body;
  bool external;
  
public:
  FunctionAST(PrototypeAST* Proto, ExprAST* Body);
  void visit();
  int nparams();
};

// Classe che organizza e gestisce il processo di compilazione
class driver
{
public:
  driver();
  void scan_begin();     // Implementata nello scanner
  void scan_end();       // Implementata nello scanner
  int parse (const std::string& f);

  RootAST* root;         // A fine parsing "punta" alla radice dell'AST
  yy::location location; // Utilizzata dallo scanner per localizzare i token
  std::string file;      // File sorgente
  
  bool trace_parsing;    // Abilita le tracce di debug nel parser
  bool trace_scanning;   // Abilita le tracce di debug nello scanner
};

#endif // ! DRIVER_HH
