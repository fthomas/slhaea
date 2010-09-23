#include <algorithm>
#include <fstream>
#include <iostream>
#include <slhaea.h>

int main(int, char* [])
{
  std::ifstream ifs("slha.par");
  SLHAea::Coll input(ifs);

  std::rotate(input.begin(), input.find("MINPAR"), input.end());
  std::reverse(input.at("SMINPUTS").begin()+1, input.at("SMINPUTS").end());
  input.at("EXTPAR").comment();

  std::cout << input;
  return 0;
}
