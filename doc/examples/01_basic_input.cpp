#include <fstream>
#include <iostream>
#include <slhaea.h>

int main(int, char* [])
{
  std::ifstream ifs("slha.par");
  SLHAea::Coll input(ifs);

  std::cout << "tan(beta) = "        << input["MINPAR"][3][1] << std::endl
    << std::endl;
  std::cout << "m_top(pole) line:\n" << input["SMINPUTS"][6]  << std::endl
    << std::endl;
  std::cout << "SMINPUTS block:\n"   << input["SMINPUTS"]     << std::endl;

  return 0;
}
