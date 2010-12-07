#include <algorithm>
#include <fstream>
#include <iostream>
#include <slhaea.h>

using namespace std;
using namespace SLHAea;

int main()
{
  ifstream ifs("slha1.txt");
  Coll input(ifs);

  rotate(input.begin(), input.find("MINPAR"), input.end());
  reverse(input.at("SMINPUTS").begin()+1, input.at("SMINPUTS").end());
  input.at("EXTPAR").comment();

  cout << input;
}
