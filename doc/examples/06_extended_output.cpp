#include <iostream>
#include <sstream>
#include <string>
#include <slhaea.h>

using namespace std;
using namespace SLHAea;

int main(int, char* [])
{
  double alpha_em = 127.934;
  double G_mu     = 1.16637e-05;
  double alpha_s  = 0.1172;
  double m_Z      = 91.1876;
  double m_b      = 4.25;
  double m_t      = 174.3;
  double m_tau    = 1.777;

  SLHA out;
  string block = "SMINPUTS";
  out[block][""] << "BLOCK" << block;
  out[block][""] << "1"     << alpha_em << "# alpha_em";
  out[block][""] << "2"     << G_mu     << "# G_mu";
  out[block][""] << "3"     << alpha_s  << "# alpha_s";
  out[block][""] << "4"     << m_Z      << "# m_Z";
  out[block][""] << "5"     << m_b      << "# m_b";
  out[block][""] << "6"     << m_t      << "# m_t";
  out[block][""] << "7"     << m_tau    << "# m_tau";

  stringstream ss;
  ss.precision(8);
  ss.setf(ios_base::scientific);
  ss << "BLOCK SMINPUTS\n"
     << " 1  " << alpha_em << "  # alpha_em\n"
     << " 2  " << G_mu     << "  # G_mu\n"
     << " 3  " << alpha_s  << "  # alpha_s\n"
     << " 4  " << m_Z      << "  # m_Z\n"
     << " 5  " << m_b      << "  # m_b\n"
     << " 6  " << m_t      << "  # m_t\n"
     << " 7  " << m_tau    << "  # m_tau\n";

  SLHA out2;
  out2.read(ss);

  cout << out;
  cout << out2;

  return 0;
}
