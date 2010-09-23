#include <fstream>
#include <iostream>
#include <slhaea.h>

int main(int, char* [])
{
  std::ifstream ifs("slha.par");
  SLHAea::Coll input(ifs);

  for (SLHAea::Block::iterator line = input["SMINPUTS"].begin();
       line != input["SMINPUTS"].end(); ++line)
  {
    if (line->is_data_line())
    {
      std::cout
        << "index: "   << (*line)[0] << ",  "
        << "value: "   << (*line)[1] << ",  "
        << "comment: " << (*line)[2] << std::endl;
    }
  }

  return 0;
}
