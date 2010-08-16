// SLHAea - another SUSY Les Houches Accord input/output library
// Copyright © 2010 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file ../LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <fstream>
#include <sstream>
#include "slhaea.h"

using namespace std;
using namespace SLHAea;

int main(int argc, char* argv[])
{
  int iters = 1;
  if (argc > 1)
  {
    istringstream iss(argv[1]);
    iss >> iters;
  }

  for (int i = 0; i < iters; ++i)
  {
    ifstream ifs("input.txt");
    SLHA input(ifs);
  }

  return 0;
}

// vim: sw=2 tw=78
