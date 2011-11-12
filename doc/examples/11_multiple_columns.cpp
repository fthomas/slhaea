#include <iostream>
#include <sstream>
#include <slhaea.h>

using namespace std;
using namespace SLHAea;

int main()
{
  stringstream dlha_text(
    "BLOCK COSMOLOGY                                                \n"
    "# identifier(s)  value              comment                    \n"
    "  1              0.72            #  normalized expansion rate h\n"
    "  2              5.72248536E-11  #  entropy density s_0 [GeV^3]\n"
    "  3     0        1               # g_{eff}(T) in tabular format\n"
    "#                T [GeV]         g_{eff}(T)                    \n"
    "  3     1        1.00000000E-05  2.00000000E-00                \n"
    "  3     2        5.00000000E-05  2.00000000E-00                \n"
    "  3     3        1.00000000E-04  4.00000000E-00                \n");

  const Coll dlha(dlha_text);
  const Block cosmology = dlha.at("cosmology");

  cout << cosmology.at("1").at(1) << " == 0.72" << endl;
  cout << cosmology.at("2").at(1) << " == 5.72...E-11" << endl;
  cout << cosmology.at("3", "0").at(2) << " == 1" << endl;

  cout << cosmology.at("3", "1").at(2) << " == 1.0...E-05" << endl;
  cout << cosmology.at("3", "1").at(3) << " == 2.0...E-00" << endl;

  cout << cosmology.at("3", "2").at(2) << " == 5.0...E-05" << endl;
  cout << cosmology.at("3", "2").at(3) << " == 2.0...E-00" << endl;

  cout << cosmology.at("3", "3").at(2) << " == 1.0...E-04" << endl;
  cout << cosmology.at("3", "3").at(3) << " == 4.0...E-00" << endl;
}
