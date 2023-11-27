#include "driver.hpp"
#include "parser.hpp"

LLVMContext *context = new LLVMContext;
Module *module = new Module("Kaleidoscope", *context);
IRBuilder<> *builder = new IRBuilder(*context);

Value *LogErrorV(const std::string Str) {
  std::cerr << Str << std::endl;
  return nullptr;
}

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
static AllocaInst *CreateEntryBlockAlloca(driver& drv, Function *TheFunction,
                                          StringRef VarName) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                   TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(Type::getDoubleTy(*context), nullptr, VarName);
}

driver::driver(): trace_parsing (false), trace_scanning (false), ast_print (false) {
};

int driver::parse (const std::string &f) {
  file = f;
  location.initialize(&file);
  scan_begin();
  yy::parser parser(*this);
  parser.set_debug_level(trace_parsing);
  int res = parser.parse();
  scan_end();
  return res;
}

void driver::codegen() {
  root->codegen(*this);
};

Value* TopExpression(ExprAST* E, driver& drv) {
  /* Crea una funzione anonima in cui viene "racchiusa" una (eventuale) espressione
     che, con riferimento alla grammatica, corrisponda ad un non-terminale "top"
     Per semplicità etichiettiamo come top-level queste espressioni */
  E->toggle();    // Ogni espressione di regola non è top, tranne che in questo caso
  PrototypeAST *Proto = new PrototypeAST("__espr_anonima"+std::to_string(++drv.Cnt),
					 std::vector<std::string>());
  Proto->noemit();   // Il codice del prototipo verrà emesso 
  //FunctionAST *F = new FunctionAST(std::move(Proto),E);
  FunctionAST *F = new FunctionAST(Proto,E);
  auto *FnIR = F->codegen(drv);
  /* Cancelliamo la funzione dal modulo (il "parent") perché la funzione
     è anonima e non può essere invocata. Il codice è stato comunque generato */
  FnIR->eraseFromParent();  
  return nullptr;
};

/************************* Sequence tree **************************/
SeqAST::SeqAST(RootAST* first, RootAST* continuation):
  first(first), continuation(continuation) {};

Value *SeqAST::codegen(driver& drv) {
  if (first != nullptr) {
    Value *f = first->codegen(drv);
  } else {
    if (continuation == nullptr) return nullptr;
  }
  Value *c = continuation->codegen(drv);
  return nullptr;
};

/************************ Expression tree *************************/
void ExprAST::toggle() {
  top = top ? false : true;
};
bool ExprAST::gettop() {
  return top;
};

/********************* Number Expression Tree *********************/
NumberExprAST::NumberExprAST(double Val): Val(Val) { top=false; };

lexval NumberExprAST::getLexVal() const {
  // Non utilizzata, Inserita per continuità con versione precedente
  lexval lval = Val;
  return lval;
};

Value *NumberExprAST::codegen(driver& drv) {  
  if (gettop()) 
     return TopExpression(this, drv);
  else
     return ConstantFP::get(*context, APFloat(Val));
};

/******************** Variable Expression Tree ********************/
VariableExprAST::VariableExprAST(const std::string &Name): Name(Name) { top=false; };

lexval VariableExprAST::getLexVal() const {
  lexval lval = Name;
  return lval;
};

//const std::string& VariableExprAST::getName() const {
//  return Name;
//};

Value *VariableExprAST::codegen(driver& drv) {
  if (gettop()) 
     return TopExpression(this, drv);
  AllocaInst *A = drv.NamedValues[Name];
  if (!A)
     return LogErrorV("Variabile non definita");
  return builder->CreateLoad(A->getAllocatedType(), A, Name.c_str());
}

/******************** Binary Expression Tree **********************/
BinaryExprAST::BinaryExprAST(char Op, ExprAST* LHS, ExprAST* RHS):
  Op(Op), LHS(LHS), RHS(RHS) { top=false; };

Value *BinaryExprAST::codegen(driver& drv) {
  if (gettop()) 
     return TopExpression(this, drv);
  Value *L = LHS->codegen(drv);
  Value *R = RHS->codegen(drv);
  if (!L || !R) 
     return nullptr;
  switch (Op) {
  case '+':
    return builder->CreateFAdd(L,R,"addres");
  case '-':
    return builder->CreateFSub(L,R,"subres");
  case '*':
    return builder->CreateFMul(L,R,"mulres");
  case '/':
    return builder->CreateFDiv(L,R,"addres");
  default:  
    return LogErrorV("Operatore binario non supportato");
  }
};

/********************* Call Expression Tree ***********************/
/* Call Expression Tree */
CallExprAST::CallExprAST(std::string Callee, std::vector<ExprAST*> Args):
  Callee(Callee),  Args(std::move(Args)) { top=false; };

lexval CallExprAST::getLexVal() const {
  lexval lval = Callee;
  return lval;
};

Value* CallExprAST::codegen(driver& drv) {
  if (gettop()) 
     return TopExpression(this, drv);
  // Cerchiamo la funzione nell'ambiente globale
  Function *CalleeF = module->getFunction(Callee);
  if (!CalleeF)
     return LogErrorV("Funzione non definita");
  // Controlliamo che gli argomenti coincidano in numero coi parametri
  if (CalleeF->arg_size() != Args.size())
     return LogErrorV("Numero di argomenti non corretto");
  std::vector<Value *> ArgsV;
  for (auto arg : Args) {
     ArgsV.push_back(arg->codegen(drv));
     if (!ArgsV.back())
        return nullptr;
  }
  return builder->CreateCall(CalleeF, ArgsV, "calltmp");
}

/************************* Prototype Tree *************************/
PrototypeAST::PrototypeAST(std::string Name, std::vector<std::string> Args):
  Name(Name), Args(std::move(Args)), emitcode(true) {};  //Di regola il codice viene emesso

//const std::string& PrototypeAST::getName() const { return Name; };
lexval PrototypeAST::getLexVal() const {
   lexval lval = Name;
   return lval;	
};

const std::vector<std::string>& PrototypeAST::getArgs() const { 
   return Args;
};

void PrototypeAST::noemit() { 
   emitcode = false; 
};

Function *PrototypeAST::codegen(driver& drv) {
  // Costruisce una struttura double(double,...,double) che descrive 
  // tipo di ritorno e tipo dei parametri (in Kaleidoscope solo double)
  std::vector<Type*> Doubles(Args.size(), Type::getDoubleTy(*context));
  FunctionType *FT =
      FunctionType::get(Type::getDoubleTy(*context), Doubles, false);
  Function *F =
      Function::Create(FT, Function::ExternalLinkage, Name, *module);

  // Attribuiamo agli argomenti il nome dei parametri formali specificati dal programmatore
  unsigned Idx = 0;
  for (auto &Arg : F->args())
    Arg.setName(Args[Idx++]);

  /* Abbiamo completato la creazione del codice del prototipo
     e dunque possiamo emetterlo (emissione su stderr).
     Tuttavia, se il prototipo è stato "aggiunto" ad una funzione anonima
     (ovvero una funzione creata in corrispondenza di un'espressione top-level),
     esso non deve essere emesso, proprio perché verrà emesso una volta
     completata la generazione della funzione anonima (prototipo + body).
     Il flag emitcode viene inizializzato a true ma per il prototipo di una
     funzione anonima viene "resettato" al valore false (si veda la funzione
     TopExpression
  */
  if (emitcode) {
    F->print(errs());
    fprintf(stderr, "\n");
  };
  
  return F;
}

/************************* Function Tree **************************/
FunctionAST::FunctionAST(PrototypeAST* Proto, ExprAST* Body): Proto(Proto), Body(Body) {
  if (Body == nullptr) 
     external=true;
  else 
     external=false;
};

Function *FunctionAST::codegen(driver& drv) {
  // Verifica che non esiste già, nel contesto, una funzione con lo stesso nome
  Function *function = 
      module->getFunction(std::get<std::string>(Proto->getLexVal()));
  // Se non esiste prova a definirla
  if (!function)
    function = Proto->codegen(drv);
  if (!function)
    return nullptr;  // Se la definizione "fallisce" restituisce nullptr

  // Crea un blocco di base in cui iniziare a inserire il codice
  BasicBlock *BB = BasicBlock::Create(*context, "entry", function);
  builder->SetInsertPoint(BB);

  // Registra gli argomenti nella symbol table
  drv.NamedValues.clear();
  for (auto &Arg : function->args()) {
    // Genera un'istruzione Alloca per la variabile corrente
    AllocaInst *Alloca = CreateEntryBlockAlloca(drv, function, Arg.getName());
    // Genera un'istruzione per la memorizzazione del valore iniziale (cioè
    // dell'argomento passato alla funzione
    builder->CreateStore(&Arg, Alloca);
    // Registra gli argomenti nella symbol table
    drv.NamedValues[std::string(Arg.getName())] = Alloca;
  }
  if (Value *RetVal = Body->codegen(drv)) {
    // Termina la creazione del codice corrispondente alla funzione
    builder->CreateRet(RetVal);

    // Effettua la validazione del codice e un controllo di consistenza
    verifyFunction(*function);
 
    /* Abbiamo completato la creazione del codice della funzione
       e dunque possiamo emetterlo (emissione su stderr) */
    function->print(errs());
    fprintf(stderr, "\n");
    return function;
  }

  // Errore nella definizione. La funzione viene rimossa
  function->eraseFromParent();
  return nullptr;
};

