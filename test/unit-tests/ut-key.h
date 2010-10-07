// SLHAea - another SUSY Les Houches Accord input/output library
// Copyright © 2010 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file ../../LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <sstream>
#include <string>
#include <vector>
#include "slhaea.h"

using namespace std;
using namespace SLHAea;

BOOST_AUTO_TEST_SUITE(TestKey)

BOOST_AUTO_TEST_CASE(testConstructors)
{
  string s1 = "A;B;1";
  const char* c1 = "A;B;1";

  Key k1(s1);
  Key k2(c1);
  Key k3("A", vector<string>(1, "B"), 1);

  BOOST_CHECK(k1.str() == k2.str());
  BOOST_CHECK(k1.str() == k3.str());
  BOOST_CHECK(k2.str() == k3.str());

  BOOST_CHECK(k1.block == k2.block);
  BOOST_CHECK(k1.line  == k2.line);
  BOOST_CHECK(k1.field == k2.field);

  BOOST_CHECK(k1.block == k3.block);
  BOOST_CHECK(k1.line  == k3.line);
  BOOST_CHECK(k1.field == k3.field);
}

BOOST_AUTO_TEST_CASE(testStr)
{
  string s1 = "A;B;1";
  Key k1(s1);

  BOOST_CHECK(k1.str() == s1);
  BOOST_CHECK(k1.block == "A");
  BOOST_CHECK(k1.line  == vector<string>(1, "B"));
  BOOST_CHECK(k1.field == 1);

  k1.str(s1.c_str());

  BOOST_CHECK(k1.str() == s1);
  BOOST_CHECK(k1.block == "A");
  BOOST_CHECK(k1.line  == vector<string>(1, "B"));
  BOOST_CHECK(k1.field == 1);

  s1 = "A B;C,D;5";
  k1.str(s1);

  BOOST_CHECK(k1.str()       == s1);
  BOOST_CHECK(k1.block       == "A B");
  BOOST_CHECK(k1.line.size() == 2);
  BOOST_CHECK(k1.line[0]     == "C");
  BOOST_CHECK(k1.line[1]     == "D");
  BOOST_CHECK(k1.field       == 5);

  k1.str(s1.c_str());

  BOOST_CHECK(k1.str()       == s1);
  BOOST_CHECK(k1.block       == "A B");
  BOOST_CHECK(k1.line.size() == 2);
  BOOST_CHECK(k1.line[0]     == "C");
  BOOST_CHECK(k1.line[1]     == "D");
  BOOST_CHECK(k1.field       == 5);

  s1 = ";C D;0";
  k1.str(s1);

  BOOST_CHECK(k1.str() == s1);
  BOOST_CHECK(k1.block == "");
  BOOST_CHECK(k1.line  == vector<string>(1, "C D"));
  BOOST_CHECK(k1.field == 0);

  s1 = " ; ;0";
  k1.str(s1);

  BOOST_CHECK(k1.str() == s1);
  BOOST_CHECK(k1.block == " ");
  BOOST_CHECK(k1.line  == vector<string>(1, " "));
  BOOST_CHECK(k1.field == 0);

  s1 = "A;1,2,3,4,5,6,7,8,9,0;0";
  k1.str(s1);

  BOOST_CHECK(k1.str()       == s1);
  BOOST_CHECK(k1.block       == "A");
  BOOST_CHECK(k1.line.size() == 10);
  BOOST_CHECK(k1.field       == 0);

  k1.str(s1.c_str());

  BOOST_CHECK(k1.str()       == s1);
  BOOST_CHECK(k1.block       == "A");
  BOOST_CHECK(k1.line.size() == 10);
  BOOST_CHECK(k1.field       == 0);

  BOOST_CHECK_THROW(k1.str("TEST;1;1;"), invalid_argument);
  BOOST_CHECK_THROW(k1.str("TEST;11"),   invalid_argument);

  k1.str("TEST;1;1");
  stringstream ss("");

  ss << k1;
  BOOST_CHECK(ss.str() == k1.str());
}

BOOST_AUTO_TEST_SUITE_END()

// vim: sw=2 tw=78
