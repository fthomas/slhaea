#include <fstream>
#include <slhaea.h>

using namespace std;
using namespace SLHAea;

int main(int, char* [])
{
  Coll input;

  ifstream ifs("slha1.txt");
  ifs >> input;
  ifs.close();

  input["MINPAR"][4][1] = "-1.0";
  input["MINPAR"][3][2] = "# tan(beta)";

  ofstream ofs("slha1.txt");
  ofs << input;
  ofs.close();
  return 0;
}
