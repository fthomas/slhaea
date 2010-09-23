#include <fstream>
#include <slhaea.h>

int main(int, char* [])
{
  SLHAea::Coll input;

  std::ifstream ifs("slha.par");
  ifs >> input;
  ifs.close();

  input["MINPAR"][4][1] = "-1.0";
  input["MINPAR"][3][2] = "# tan(beta)";

  std::ofstream ofs("slha.par");
  ofs << input;
  ofs.close();

  return 0;
}
