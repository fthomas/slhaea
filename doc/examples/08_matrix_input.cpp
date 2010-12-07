#include <fstream>
#include <iostream>
#include <slhaea.h>

using namespace std;
using namespace SLHAea;

int main()
{
  ifstream ifs("slha2.txt");
  Coll input(ifs);
  double rvhmix[5][5];

  for (int r = 0; r < 5; ++r)
  {
    for (int c = 0; c < 5; ++c)
    {
      rvhmix[r][c] = to<double>(input.at("RVHMIX").at({r+1, c+1}).at(2));
      cout << (r+1) << " " << (c+1) << ": " << rvhmix[r][c] << endl;
    }
  }
}
