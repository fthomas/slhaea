#include <fstream>
#include <iostream>
#include <slhaea.h>

using namespace std;
using namespace SLHAea;

int main(int, char* [])
{
  ifstream ifs("slha.par");
  const Coll in(ifs);

  double alpha_em = to_<double>(in.at("SMINPUTS").at("1").at(1));
  double G_mu     = to_<double>(in.at("SMINPUTS").at("2").at(1));
  double alpha_s  = to_<double>(in.at("SMINPUTS").at("3").at(1));
  double m_Z      = to_<double>(in.at("SMINPUTS").at("4").at(1));
  double m_b      = to_<double>(in.at("SMINPUTS").at("5").at(1));
  double m_t      = to_<double>(in.at("SMINPUTS").at("6").at(1));
  double m_tau    = to_<double>(in.at("SMINPUTS").at("7").at(1));

  cout << "1/alpha_em: " << 1./alpha_em << endl;
  cout << "1/G_mu:     " << 1./G_mu     << endl;
  cout << "1/alpha_s:  " << 1./alpha_s  << endl;
  cout << "1/m_Z:      " << 1./m_Z      << endl;
  cout << "1/m_b:      " << 1./m_b      << endl;
  cout << "1/m_t:      " << 1./m_t      << endl;
  cout << "1/m_tau:    " << 1./m_tau    << endl;

  return 0;
}
