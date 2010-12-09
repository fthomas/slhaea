#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <slhaea.h>

using namespace std;
using namespace SLHAea;

int main()
{
  stringstream some_blocks(
    "BLOCK test Q= 1000 \n"
    "    1   3.14       \n"
    "    2   6.28       \n"
    "BLOCK test Q= 2000 \n"
    "    1   2.71       \n"
    "    2   5.42       \n"
    "BLOCK test Q= 2000 \n"
    "    1   1.61       \n"
    "    2   3.22       \n");

  Coll input(some_blocks);
  Block b1, b2, b3;

  // Find the first block:
  vector<string> key;
  key.push_back("Block");
  key.push_back("TEST");
  key.push_back("q=");
  key.push_back("1000");
  b1 = input.at(key);

  cout << *b1.find_block_def() << endl;
  cout <<  b1.at(1).at(1) << " == 3.14" << endl;
  cout <<  b1.at(2).at(1) << " == 6.28" << endl;

  // Find the second block:
  key.at(3) = "2000";
  b2 = input.at(key);

  cout << *b2.find_block_def() << endl;
  cout <<  b2.at(1).at(1) << " == 2.71" << endl;
  cout <<  b2.at(2).at(1) << " == 5.42" << endl;

  // Find the third block:
  b3 = *Coll::find(input.rbegin(), input.rend(), key);

  cout << *b3.find_block_def() << endl;
  cout <<  b3.at(1).at(1) << " == 1.61" << endl;
  cout <<  b3.at(2).at(1) << " == 3.22" << endl;
}
