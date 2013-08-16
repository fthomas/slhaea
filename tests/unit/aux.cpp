// SLHAea - containers for SUSY Les Houches Accord input/output
// Copyright © 2010 Frank S. Thomas <frank@timepit.eu>
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file ../../LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <limits>
#include <string>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>
#include "slhaea.h"

using namespace std;
using namespace SLHAea;
using namespace SLHAea::detail;

BOOST_AUTO_TEST_SUITE(TestAuxiliaries)

BOOST_AUTO_TEST_CASE(testTo)
{
  BOOST_CHECK_EQUAL(to<bool>("0"), false);
  BOOST_CHECK_EQUAL(to<bool>("1"), true);

  BOOST_CHECK_EQUAL(to<int>("0"), 0);
  BOOST_CHECK_EQUAL(to<int>("1"), 1);

  BOOST_CHECK_EQUAL(to<int>("-0"), -0);
  BOOST_CHECK_EQUAL(to<int>("-1"), -1);

  const float float_eps = numeric_limits<float>::epsilon() * 100;

  BOOST_CHECK_CLOSE(to<float>("0"), 0.0, float_eps);
  BOOST_CHECK_CLOSE(to<float>("1"), 1.0, float_eps);

  BOOST_CHECK_CLOSE(to<float>(0), 0.0, float_eps);
  BOOST_CHECK_CLOSE(to<float>(1), 1.0, float_eps);

  BOOST_CHECK_CLOSE(to<float>("10.98765"), 10.98765, float_eps);
  BOOST_CHECK_CLOSE(to<float>("10.51234"), 10.51234, float_eps);
}

BOOST_AUTO_TEST_CASE(testToString)
{
  BOOST_CHECK_EQUAL(to_string("foo"), "foo");
  BOOST_CHECK_EQUAL(to_string("bar"), "bar");

  BOOST_CHECK_EQUAL(to_string(false), "0");
  BOOST_CHECK_EQUAL(to_string(true),  "1");

  BOOST_CHECK_EQUAL(to_string(0), "0");
  BOOST_CHECK_EQUAL(to_string(1), "1");

  BOOST_CHECK_EQUAL(to_string(1.0, 0), "1e+00");
  BOOST_CHECK_EQUAL(to_string(1.0, 1), "1.0e+00");
  BOOST_CHECK_EQUAL(to_string(1.0, 2), "1.00e+00");
  BOOST_CHECK_EQUAL(to_string(1.0, 3), "1.000e+00");
  BOOST_CHECK_EQUAL(to_string(1.0, 4), "1.0000e+00");
}

BOOST_AUTO_TEST_CASE(testTrimLeftRight)
{
  string str = "";
  trim_left(str);
  trim_right(str);
  BOOST_CHECK_EQUAL(str, "");

  str = " \t \r \n \v \f ";
  trim_left(str);
  BOOST_CHECK_EQUAL(str, "");

  str = " \t \r \n \v \f ";
  trim_right(str);
  BOOST_CHECK_EQUAL(str, "");

  str = "\f \n test \n \t";
  trim_left(str);
  BOOST_CHECK_EQUAL(str, "test \n \t");
  trim_right(str);
  BOOST_CHECK_EQUAL(str, "test");

  str = "test";
  trim_left(str);
  BOOST_CHECK_EQUAL(str, "test");
  trim_right(str);
  BOOST_CHECK_EQUAL(str, "test");
}

BOOST_AUTO_TEST_SUITE_END()
