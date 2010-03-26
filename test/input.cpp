// SLHAea - another SUSY Les Houches Accord input/output library
// Copyright © 2010 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file ../LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <fstream>
#include "slhaea.h"

using namespace std;
using namespace SLHAea;

int main()
{
  ifstream ifs("input.txt");
  SLHA input;
  input.read(ifs);
  ifs.close();

  return 0;
}

// vim: sw=2 tw=78
