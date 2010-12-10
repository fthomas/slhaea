#include <fstream>
#include <iostream>
#include <slhaea.h>

using namespace std;
using namespace SLHAea;

int main()
{
  ifstream ifs("slha2.txt");
  Coll input(ifs);

  cout << "Size before erasing blocks: " << input.size() << endl;

  Coll::key_matches
    is_rvkappa("RVKAPPA"),
    is_rvsnvev("RVSNVEV"),
    is_sphenorp("SPhenoRP");

  for (Coll::iterator block = input.begin(); block != input.end();)
  {
    if (is_rvkappa(*block) || is_rvsnvev(*block) || is_sphenorp(*block))
    { ++block; }
    else block = input.erase(block);
  }

  cout << "Size after  erasing blocks: " << input.size() << endl;
}
