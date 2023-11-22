#include <iostream>
#include "driver.hpp"

int main (int argc, char *argv[])
{
  driver drv;
  for (int i = 1; i < argc; ++i)
    if (argv[i] == std::string ("-p"))
      drv.trace_parsing = true;
    else if (argv[i] == std::string ("-s"))
      drv.trace_scanning = true;
    else if (!drv.parse (argv[i])) {
      drv.root->visit();
      std::cout << std::endl;
    } else return 1;
  return 0;
}
