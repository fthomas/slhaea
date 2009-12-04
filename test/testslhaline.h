// SLHAme - SUSY Les Houches Accord made easy
// Copyright © 2009 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SLHAME_TESTSLHALINE_H
#define SLHAME_TESTSLHALINE_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <slhame.h>

namespace SLHAme {

class TestSlhaLine : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestSlhaLine);
  CPPUNIT_TEST(testConstructors);
  CPPUNIT_TEST(testAccessors);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}

  void tearDown() {}

  void testConstructors()
  {
    SlhaLine l1;
    CPPUNIT_ASSERT(l1.empty() == true);
    CPPUNIT_ASSERT(l1.size() == 1);

    SlhaLine l2(" 1 2 3 4 ");
    CPPUNIT_ASSERT(l2.empty() == false);
    CPPUNIT_ASSERT(l2.size() == 4);

    SlhaLine l3(" 1 2 3 4 \n 5 6 ");
    CPPUNIT_ASSERT(l3.empty() == false);
    CPPUNIT_ASSERT(l3.size() == 4);

    const SlhaLine l4(" 1 2 3 4 # 5 6 ");
    CPPUNIT_ASSERT(l4.empty() == false);
    CPPUNIT_ASSERT(l4.size() == 5);

    const SlhaLine l5(" \n 1 2 3 4 # 5 6 ");
    CPPUNIT_ASSERT(l5.empty() == true);
    CPPUNIT_ASSERT(l5.size() == 1);
  }

  void testAccessors()
  {
    SlhaLine l1;
    const SlhaLine cl1;
    CPPUNIT_ASSERT(l1[0] == "");
    CPPUNIT_ASSERT(cl1[0] == "");
    CPPUNIT_ASSERT(l1[0] == l1.at(0));
    CPPUNIT_ASSERT(cl1[0] == cl1.at(0));

    l1[0] = " 1 2 ";
    CPPUNIT_ASSERT(l1[0] == " 1 2 ");
    CPPUNIT_ASSERT(l1.size() == 1);
    l1.at(0) = "";
    CPPUNIT_ASSERT(l1.empty() == true);

    try
    {
      l1[1] = "3";
      l1.at(2) = "4";
    }
    catch (std::out_of_range ex)
    {
      CPPUNIT_ASSERT(l1.empty() == true);
    }
  }

/*
  void testSlhaLine()
  {
    SlhaLine l1;
    CPPUNIT_ASSERT(l1.str() == "");
    
    l1.str(" 1 2 3 4 ");
    CPPUNIT_ASSERT(l1.str() == " 1 2 3 4");
    CPPUNIT_ASSERT(l1.size() == 4);
    
    l1 = "BLOCK TEST # comment";
    CPPUNIT_ASSERT(l1.str() == "BLOCK TEST # comment");
    CPPUNIT_ASSERT(l1[0] == "BLOCK");
    CPPUNIT_ASSERT(l1[1] == "TEST");
    CPPUNIT_ASSERT(l1[2] == "# comment");

    l1 = "BLOCK  TEST  # comment";
    CPPUNIT_ASSERT(l1.str() == "BLOCK  TEST  # comment");
    CPPUNIT_ASSERT(l1[0] == "BLOCK");
    CPPUNIT_ASSERT(l1[1] == "TEST");
    CPPUNIT_ASSERT(l1[2] == "# comment");

    l1 = "# one long comment with trailing spaces    ";
    CPPUNIT_ASSERT(l1[0] == "# one long comment with trailing spaces");
    CPPUNIT_ASSERT(l1.size() == 1);

    l1 = "one long data line with trailing spaces    ";
    CPPUNIT_ASSERT(l1.str() == "one long data line with trailing spaces");
    CPPUNIT_ASSERT(l1[6] == "spaces");
    CPPUNIT_ASSERT(l1.size() == 7);

    l1 = "Hello brave new \n world!";
    CPPUNIT_ASSERT(l1.str() == "Hello brave new");
    CPPUNIT_ASSERT(l1.size() == 3);

    l1 = "Hello stupid brave new world!";
    CPPUNIT_ASSERT(l1.str() == "Hello stupid brave new world!");
    l1[1] = "";
    CPPUNIT_ASSERT(l1.str() == "Hello        brave new world!");
    l1[1] = "dumb";
    CPPUNIT_ASSERT(l1.str() == "Hello dumb   brave new world!");
    l1[3] = "brand-new";
    CPPUNIT_ASSERT(l1.str() == "Hello dumb   brave brand-new world!");
    l1[0] = "Goodbye";
    CPPUNIT_ASSERT(l1.str() == "Goodbye dumb brave brand-new world!");

    l1 = " 23  1.123456789E+999  # some arcane value";
    CPPUNIT_ASSERT(l1[0] == "23");
    CPPUNIT_ASSERT(l1[1] == "1.123456789E+999");
    CPPUNIT_ASSERT(l1[2] == "# some arcane value");

    l1 = "\n 1 2 3 4 5";
    CPPUNIT_ASSERT(l1.empty() == true);
    CPPUNIT_ASSERT(l1.size() == 1);
    l1 = "  \n 1 2 3 4 5  ";
    CPPUNIT_ASSERT(l1.empty() == true);
    CPPUNIT_ASSERT(l1.size() == 1);
    l1 = "";
    CPPUNIT_ASSERT(l1.empty() == true);
    CPPUNIT_ASSERT(l1.size() == 1);
    l1 = "   ";
    CPPUNIT_ASSERT(l1.empty() == true);
    CPPUNIT_ASSERT(l1.size() == 1);
    l1 = " . ";
    CPPUNIT_ASSERT(l1.empty() == false);
    CPPUNIT_ASSERT(l1.size() == 1);

    l1 = " 1 22 333 4444 ";
    l1.append(" 55555 # a comment ");
    CPPUNIT_ASSERT(l1.size() == 6);
    CPPUNIT_ASSERT(l1[3] == "4444");
    CPPUNIT_ASSERT(l1[4] == "55555");
    CPPUNIT_ASSERT(l1[5] == "# a comment");
    l1 += " 7 ";
    CPPUNIT_ASSERT(l1[5] == "# a comment 7");

    l1 = " test  str() and   strPlain() ";
    CPPUNIT_ASSERT(l1.str() == " test  str() and   strPlain()");
    CPPUNIT_ASSERT(l1.strPlain() == "test str() and strPlain()");

    l1 = " 1 2 3 4 5 ";
    CPPUNIT_ASSERT(l1.back() == "5");
    l1.back() = "6";
    CPPUNIT_ASSERT(l1.back() == "6");
    CPPUNIT_ASSERT(l1.front() == "1");
    l1.front() = "-1";
    CPPUNIT_ASSERT(l1.front() == "-1");

    CPPUNIT_ASSERT(l1.front() == *l1.begin());
    CPPUNIT_ASSERT(l1.back() == *(l1.end()-1));
    CPPUNIT_ASSERT(*(l1.end()-2) == "4");
    *l1.begin() = "0";
    CPPUNIT_ASSERT(l1.front() == "0");
    *(l1.end()-1) = "7";
    CPPUNIT_ASSERT(l1.back() == "7");

    CPPUNIT_ASSERT(*l1.rbegin() == l1.back());
    CPPUNIT_ASSERT(*(l1.rend()-1) == l1.front());
    *l1.rbegin() = "8";
    CPPUNIT_ASSERT(l1.back() == "8");
    *(l1.rend()-1) = "-2";
    CPPUNIT_ASSERT(l1.front() == "-2");
  }*/
};

} // namespace SLHAme

#endif // SLHAME_TESTSLHALINE_H

// vim: sw=2 tw=78
