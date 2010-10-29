#include <fstream>
#include <iostream>
#include <slhaea.h>

using namespace std;
using namespace SLHAea;

int main(int, char* [])
{
  ifstream ifs("slha2.txt");
  const Coll input(ifs);
  const Block chi10_decay = input.at("1000022");

  cout << "chi_10 decay width: " << chi10_decay.at("DECAY").at(2) << endl;

  cout << "BR(chi_10 -> mu- W+):  "
       << chi10_decay.at("(any)", "2", "13", "24").at(0) << endl;
  cout << "BR(chi_10 -> tau- W+): "
       << chi10_decay.at("(any)", "2", "15", "24").at(0) << endl;

  cout << "BR(chi_10 -> mu+ W-):  "
       << input.field("1000022;(any),2,-13,-24;0") << endl;
  cout << "BR(chi_10 -> tau+ W-): "
       << input.field("1000022;(any),2,-15,-24;0") << endl;

  double sum = 0.;
  for (Block::const_iterator line = chi10_decay.begin();
       line != chi10_decay.end(); ++line)
  {
    if (!line->is_data_line()) continue;

    sum += to<double>(line->at(0));
  }

  cout << "Sum of all chi_10 BRs: " << sum << endl;

  return 0;
}
