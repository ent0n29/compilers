#include <iostream>
#include <map>
#include "parser.hpp"

// Forward declarations (functions defined in scanner.ll)
void scan_begin (const std::string &f);
void scan_end ();

calc::location location;
bool trace_parsing = false;
bool trace_scanning = false;
std::map<std::string,int> variables;
int result;

int parse(const std::string &f) {
  location.initialize (&f);
  scan_begin (f);
  calc::parser parser;
  parser.set_debug_level (trace_parsing);
  int res = parser.parse ();
  scan_end ();
  return res;
}

int main (int argc, char *argv[])
{
  int res = 0;
  for (int i = 1; i < argc; ++i)
    if (argv[i] == std::string ("-p"))
      trace_parsing = true;
    else if (argv[i] == std::string ("-s"))
      trace_scanning = true;
    else if (!parse (argv[i]))
      std::cout << result << '\n';
    else
      res = 1;
  return res;
}
