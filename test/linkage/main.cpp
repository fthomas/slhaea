#include <iostream>
#include "slhaea.h"

SLHAea::Line bar();
SLHAea::Line foo();

int main(int, char* [])
{
  std::cout << foo() << std::endl;
  std::cout << bar() << std::endl;
  return 0;
}
