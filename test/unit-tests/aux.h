// SLHAea - another SUSY Les Houches Accord input/output library
// Copyright © 2010 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file ../../LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "slhaea.h"

using namespace SLHAea;

BOOST_AUTO_TEST_SUITE(TestAuxiliaries)

BOOST_AUTO_TEST_CASE(testTo)
{
  BOOST_CHECK(to<bool>("0") == false);
  BOOST_CHECK(to<bool>("1") == true);

  BOOST_CHECK(to<int>("0") == 0);
  BOOST_CHECK(to<int>("1") == 1);

  BOOST_CHECK(to<int>("-0") == -0);
  BOOST_CHECK(to<int>("-1") == -1);

  BOOST_CHECK(to<float>("0") == 0.0);
  BOOST_CHECK(to<float>("1") == 1.0);

  BOOST_CHECK(to<float>(0) == 0.0);
  BOOST_CHECK(to<float>(1) == 1.0);

  BOOST_CHECK(to<float>("10.51234") > 10.0);
  BOOST_CHECK(to<float>("10.51234") < 11.0);
}

BOOST_AUTO_TEST_CASE(testToString)
{
  BOOST_CHECK(to_string("foo") == "foo");
  BOOST_CHECK(to_string("bar") == "bar");

  BOOST_CHECK(to_string(false) == "0");
  BOOST_CHECK(to_string(true)  == "1");

  BOOST_CHECK(to_string(0) == "0");
  BOOST_CHECK(to_string(1) == "1");

  BOOST_CHECK(to_string(0.0) == "0");
  BOOST_CHECK(to_string(1.0) == "1");

  BOOST_CHECK(to_string(1.0, 0) == "1e+00");
  BOOST_CHECK(to_string(1.0, 1) == "1.0e+00");
  BOOST_CHECK(to_string(1.0, 2) == "1.00e+00");
  BOOST_CHECK(to_string(1.0, 3) == "1.000e+00");
  BOOST_CHECK(to_string(1.0, 4) == "1.0000e+00");
}

BOOST_AUTO_TEST_SUITE_END()

// vim: sw=2 tw=78
