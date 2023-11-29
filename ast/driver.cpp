#include "driver.hpp"
#include "parser.hpp"

driver::driver(): trace_parsing (false), trace_scanning (false) {};

int driver::parse (const std::string &f)
{
  file = f;
  location.initialize(&file);
  scan_begin();
  yy::parser parser(*this);
  parser.set_debug_level(trace_parsing);
  int res = parser.parse();
  scan_end();
  return res;
}

SeqAST::SeqAST(RootAST* first, RootAST* continuation): first(first), continuation(continuation) {};
RootAST* SeqAST::left() { return first; };
RootAST* SeqAST::right() { return continuation; };
void SeqAST:: visit() {
  if (first != nullptr) {
    first->visit();
  } else {
    if (continuation == nullptr) {
      return;
    };
  };
  std::cout << ";";
  continuation->visit();
};

NumberExprAST::NumberExprAST(double Val): Val(Val) {};
void NumberExprAST::visit() {
  //std::cout << std::get<double>(getLexVal()) << " ";
  std::cout << Val << " ";
};
lexval NumberExprAST::getLexVal() const {
  lexval lval = Val;
  return lval;
};

VariableExprAST::VariableExprAST(std::string &Name): Name(Name) {};
lexval VariableExprAST::getLexVal() const {
  lexval lval = Name;
  return lval;
};
void VariableExprAST::visit() {
  //std::cout << std::get<std::string>(getLexVal()) << " ";
  std::cout << Name << " ";
};

BinaryExprAST::BinaryExprAST(char Op, ExprAST* LHS, ExprAST* RHS): Op(Op), LHS(LHS), RHS(RHS) {};
ExprAST* BinaryExprAST::left() {
  return LHS;
};
ExprAST* BinaryExprAST::right() {
  return RHS;
};
void BinaryExprAST::visit() {
  std::cout << "(" << Op << " ";
  LHS->visit();
  if (RHS!=nullptr) RHS->visit();
  std::cout << ")";
};

CallExprAST::CallExprAST(std::string Callee, std::vector<ExprAST*> Args): Callee(Callee), Args(std::move(Args)) {};
lexval CallExprAST::getLexVal() const {
  lexval lval = Callee;
  return lval;
};
void CallExprAST::visit() {
  std::cout << std::get<std::string>(getLexVal()) << "( ";
  for (ExprAST* arg : Args) {
    arg->visit();
  };
  std::cout << ')';
};

PrototypeAST::PrototypeAST(std::string Name, std::vector<std::string> Args): Name(Name), Args(std::move(Args)) {};
lexval PrototypeAST::getLexVal() const {
  lexval lval = Name;
  return lval;
};
const std::vector<std::string>& PrototypeAST::getArgs() const {
  return Args;
};
void PrototypeAST::visit() {
  std::cout << "extern " << Name << "( ";
  for (auto it=Args.begin(); it!=Args.end(); ++it) {
    std::cout << *it << ' ';
  };
  std::cout << ')';
};
int PrototypeAST::argsize() {
  return Args.size();
};

FunctionAST::FunctionAST(PrototypeAST* Proto, ExprAST* Body): Proto(Proto), Body(Body) {
  if (Body == nullptr) external=true;
  else external=false;
};
void FunctionAST::visit() {
  std::cout << std::get<std::string>(Proto->getLexVal()) << "( ";
  for (auto it=Proto->getArgs().begin(); it!= Proto->getArgs().end(); ++it) {
    std::cout << *it << ' ';
  };
  std::cout << ')';
  Body->visit();
};
int FunctionAST::nparams() {
  return Proto->argsize();
};
