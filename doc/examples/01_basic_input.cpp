#include <fstream>
#include <iostream>
#include <slhaea.h>

using namespace std;
using namespace SLHAea;

int main(int, char* [])
{
  ifstream ifs("slha1.txt");
  Coll input(ifs);

  cout << "tan(beta) = "        << input["MINPAR"][3][1] << endl << endl;
  cout << "m_top(pole) line:\n" << input["SMINPUTS"][6]  << endl << endl;
  cout << "SMINPUTS block:\n"   << input["SMINPUTS"];
  return 0;
}
