#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <cstring>
using namespace std;

set<char> operators = {'*','|','.'}; // Il punto indica la concatenazione

class AST {
private:
  char symb;
  AST* left;
  AST* right;
public:
  AST(char symb, AST* left, AST* right): symb(symb), left(left), right(right) {};
  AST(char symb): symb(symb), left(nullptr), right(nullptr) {};
  string visit() {
    /*Algoritmo di visita dell'albero in post-ordine.
      Usando l'informazione ai nodi (un operatore o un carattere dell'alfabeto)
      la visita semplicemente risostruisce la rappresentazione esterna (come stringa)
      dell'albero che a sua volta rappresenta un'espressione regolare.
      Il metodo volutamente non è generico per evitare complicazioni con il C++.
      I tre casi (foglia, un solo figlio, due figli) vengono qui tenuti
      distinti "a scopo didattico" perché (in generale) le
      operazioni da fare possono essere completamente distinte.
     */
    string treerep = "";               // treerep conterrà la rappresentazione del
				       // sottoalbero radicato nel nodo corrente
    if (left != nullptr) {
      string ltree = left->visit();    // Visitiamo il sottoalbero di sx
      if (right != nullptr) {
	// Caso di nodo con due figli
	string rtree = right->visit(); // Visitiamo il sottoalbero di dx
	treerep.push_back(symb);       // 
	treerep.push_back('(');        // 
	treerep.append(ltree);         //
	treerep.push_back(')');        // Calcoli al nodo dopo le due visite 
	treerep.push_back('(');        //  
	treerep.append(rtree);         //
	treerep.push_back(')');        //
      } else {
	// Caso di nodo con un solo figlio
	treerep.push_back(symb);       //
	treerep.push_back('(');        // Calcoli al nodo dopo la visita a sx
	treerep.append(ltree);         //
	treerep.push_back(')');        //
      }
    } else {
      treerep.push_back(symb);         // Calcolo nel caso di foglia
    }
    return treerep;
  }
};

int skipblank(string S, int j) {
  /* Restituisce il primo non blank a partire dalla
     posizione j (inclusa)
  */
  while (j<S.length() and S[j] == ' ') j++;
  return j;
}

string removeblank(string S) {
  /* Restituisce la stringa S "compattata", in cui cioè
     sono rimossi tutti i caratteri "blank"
  */
  int n = S.length();
  int j = skipblank(S,0);
  string compact = "";
  while (j<n) {
    compact.push_back(S.at(j));
    j = skipblank(S,++j);
  }
  return compact;
}

int getsubtree(string S, int j) {
  /* Restituisce la lunghezza della stringa che, a partire dalla
     posizione j, bilancia correttamente le parentesi
     Si aspetta S.at(j) == '('
  */
  int s = j;  // Ricorda la starting position
  j++;
  int numpar = 1;
  while (numpar>0) {
    if (S.at(j)=='(') numpar++;
    if (S.at(j)==')') numpar--;
    j++;
  }
  return j-s;
}

AST* create(string linearrep, set<char> alphabet, bool verbose) {
  /* Costruisce la rappresentazione interna dell'AST
     corrispondente alla rappresentazione lineare linearrep.
     Posto n=lunghezza di linearrep, i casi n=1 e n=3
     indicano che linearrep rappresenta un nodo foglia.
     La differenza è che, se n=1, l'intero albero di cui
     dobbiamo costruire la rappresentazione è formato da un
     solo nodo (radice e foglia simultaneamente).
     Il caso n=3 implica invece che siamo in un passaggio ricorsivo.
     Se n>3 allora linerrep descrive un (sotto) albero composto da
     una radice, con l'operatore, e da almeno un figlio. L'algoritmo
     ricerca nella stringa la rappresentazione del figlio/dei figli,
     ricorsivamente ne costruisce la/le rappresentazione/i, e la/le usa
     per costruire l'albero da restituire al chiamante.
   */
  int n = linearrep.length();
  if (n==1 or n==3) { // linearrep = 'c' oppure "(c)"
    char c = linearrep.at(max(0,n-2));  // Unifica i due casi recuperando la lettera
    if (alphabet.count(c) == 0) throw invalid_argument("Il simbolo "+string(1,c)+" non fa parte dell'alfabeto");
    return new AST(c);
  }
  char op = linearrep.at(1);
  if (operators.count(op) == 0) throw invalid_argument("Il simbolo "+string(1,op)+" non è un operatore legale");
  int start = 2;
  int stop = getsubtree(linearrep, start);
  string left = linearrep.substr(start,stop);
  start += stop;
  if (op=='*') {  // L'albero andrà costruito con un solo figlio
    if (verbose) cout << "op:" << op << "\tleft:" << left << endl;
    AST* lt = create(left,alphabet,verbose);
    return new AST(op,lt,nullptr);
  } else {       // Operatore binario -> due figli
    stop = getsubtree(linearrep, start);
    string right = linearrep.substr(start,stop);
    if (verbose) {
      cout << "op:" << op  << "\tleft:" << left;
      cout << "\tright:" << right << endl;
    }
    AST* lt = create(left,alphabet,verbose);
    AST* rt = create(right,alphabet,verbose);
    return new AST(op,lt,rt);
  }
}

int main(int argc, char** argv) {
  bool verbose = false;
  string fn;
  set<char> symbols;           // Conterrà i simboli dell'alfabeto
  ifstream f(fn);    
  f.open("tree2.txt")          // Apriamo il file con alfabeto e con la rapp. esterna dell'albero
  string line;                 // La prima riga deve contenere l'alfabeto
  getline(f, line);
  
  stringstream alphabet(line); // Ci prepariamo per scan e tokenizzazionea
  char symb;
  while(alphabet>>symb) {      // Se il primo parametro è un oggetto di tipo
                               // stringstream, getline è di fatto uno scanner
        symbols.insert(symb);
  }

  getline(f, line);           // La seconda riga contiene la rappresentazione
                              // esterna dell'AST che, a sua volta, rappresenta
                              // l'espressione regolare.
                              // Per semplicità, non contempliamo l'uso di
                              // espressioni regolari che includono epsilom

  AST *root = create(removeblank(line), symbols, verbose);
  cout << root->visit() << endl; // Verifichiamo di aver internamente ricostruito l'AST
                                 // in modo corretto
}
