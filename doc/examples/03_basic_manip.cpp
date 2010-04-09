#include <algorithm>
#include <fstream>
#include <iostream>
#include <slhaea.h>

using namespace std;
using namespace SLHAea;

int main(int, char* [])
{
  ifstream ifs("slha.par");
  SLHA input(ifs);

  rotate(input.begin(), input.find("MINPAR"), input.end());
  reverse(input.at("SMINPUTS").begin()+1, input.at("SMINPUTS").end());

  cout << input;
  return 0;
}
