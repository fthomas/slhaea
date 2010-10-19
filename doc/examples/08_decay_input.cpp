#include <fstream>
#include <slhaea.h>

using namespace std;
using namespace SLHAea;

int main(int, char* [])
{
  ifstream ifs("slha2.txt");
  Coll input(ifs);

  return 0;
}
