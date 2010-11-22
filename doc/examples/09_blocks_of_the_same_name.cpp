#include <iostream>
#include <sstream>
#include <slhaea.h>

using namespace std;
using namespace SLHAea;

int main(int, char* [])
{
  stringstream some_blocks(
    "BLOCK foo  Q= 2000 \n"
    "    1   1          \n"
    "BLOCK test Q= 1000 \n"
    "    1   3.14       \n"
    "    2   6.28       \n"
    "BLOCK bar  Q= 1000 \n"
    "    2   2          \n"
    "BLOCK test Q= 2000 \n"
    "    1   2.71       \n"
    "    2   5.42       \n"
    "BLOCK test         \n"
    "    3   3          \n");

  Block q1k, q2k;
  Coll input(some_blocks);

  Coll::iterator first = input.find("test");
  Coll::iterator last  = input.end();

  for (; first != last; first = Coll::find(++first, last, "test"))
  {
    auto block_def = first->find_block_def();
    if (block_def != first->end() &&
        block_def->size() >= 4 &&
        block_def->at(3) == "2000")
    {
      q2k = *first;
      break;
    }
  }

  cout << *q2k.find_block_def() << endl;
  cout <<  q2k.at(1).at(1) << " == 2.71" << endl;
  cout <<  q2k.at(2).at(1) << " == 5.42" << endl;


  Block::key_type q1k_key = {"BLOCK", "test", "Q=", "1000"};

  first = input.begin();
  last  = input.end();

  for (; first != last; ++first)
  {
    if (first->find(q1k_key) != first->end())
    {
      q1k = *first;
      break;
    }
  }

  cout << *q1k.find_block_def() << endl;
  cout <<  q1k.at(1).at(1) << " == 3.14" << endl;
  cout <<  q1k.at(2).at(1) << " == 6.28" << endl;

  return 0;
}
