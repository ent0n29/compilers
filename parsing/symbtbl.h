#define maxidlen 16

struct Token {
  int toktype;
  union {
    float num;
    char id[maxidlen];  // Max length for the identifiers (variables anf function names)
  };
}; 

