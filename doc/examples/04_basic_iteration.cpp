#include <fstream>
#include <iostream>
#include <slhaea.h>

using namespace std;
using namespace SLHAea;

int main(int, char* [])
{
  ifstream ifs("slha.par");
  SLHA input(ifs);

  for (SLHABlock::iterator line = input["SMINPUTS"].begin();
       line != input["SMINPUTS"].end(); ++line)
  {
    if (line->is_data_line())
    {
      cout << "index: "   << (*line)[0] << ",  "
           << "value: "   << (*line)[1] << ",  "
           << "comment: " << (*line)[2] << endl;
    }
  }

  return 0;
}