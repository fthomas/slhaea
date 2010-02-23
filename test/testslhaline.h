// SLHAea - another SUSY Les Houches Accord input/output library
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

#ifndef SLHAEA_TESTSLHALINE_H
#define SLHAEA_TESTSLHALINE_H

#include <stdexcept>
#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <slhaea.h>

using namespace std;

namespace SLHAea {

class TestSLHALine : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestSLHALine);
  CPPUNIT_TEST(testConstructors);
  CPPUNIT_TEST(testAccessors);
  CPPUNIT_TEST(testOperators);
  CPPUNIT_TEST(testModifiers);
  CPPUNIT_TEST(testIterators);
  CPPUNIT_TEST(testMiscellaneous);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}

  void tearDown() {}

  void testConstructors()
  {
    SLHALine l1;
    CPPUNIT_ASSERT(l1.empty() == true);
    CPPUNIT_ASSERT(l1.size() == 0);

    SLHALine l2(" 1 2 3 4 ");
    CPPUNIT_ASSERT(l2.empty() == false);
    CPPUNIT_ASSERT(l2.size() == 4);

    SLHALine l3(" 1 2 3 4 \n 5 6 ");
    CPPUNIT_ASSERT(l3.empty() == false);
    CPPUNIT_ASSERT(l3.size() == 4);

    const SLHALine l4(" 1 2 3 4 # 5 6 ");
    CPPUNIT_ASSERT(l4.empty() == false);
    CPPUNIT_ASSERT(l4.size() == 5);

    const SLHALine l5(" \n 1 2 3 4 # 5 6 ");
    CPPUNIT_ASSERT(l5.empty() == true);
    CPPUNIT_ASSERT(l5.size() == 0);
  }

  void testAccessors()
  {
    SLHALine l1("1");
    const SLHALine cl1 = l1;
    CPPUNIT_ASSERT(l1[0] == "1");
    CPPUNIT_ASSERT(cl1[0] == "1");
    CPPUNIT_ASSERT(l1[0] == l1.at(0));
    CPPUNIT_ASSERT(cl1[0] == cl1.at(0));
    CPPUNIT_ASSERT(l1.front() == l1.at(0));
    CPPUNIT_ASSERT(l1.back() == l1.at(0));
    CPPUNIT_ASSERT(cl1.front() == l1.at(0));
    CPPUNIT_ASSERT(cl1.back() == l1.at(0));

    l1[0] = " 1 2 ";
    CPPUNIT_ASSERT(l1[0] == " 1 2 ");
    CPPUNIT_ASSERT(l1.size() == 1);
    l1.at(0) = "";
    CPPUNIT_ASSERT(l1.empty() == false);

    try
    {
      l1.at(1) = "3";
    }
    catch (out_of_range ex)
    {
      CPPUNIT_ASSERT(l1.empty() == false);
    }

    l1 = "  1  2  3  ";
    const SLHALine cl2 = l1;
    CPPUNIT_ASSERT(l1.front() == "1");
    CPPUNIT_ASSERT(cl2.front() == "1");
    CPPUNIT_ASSERT(l1.back() == "3");
    CPPUNIT_ASSERT(cl2.back() == "3");
    CPPUNIT_ASSERT(l1.size() == 3);
    CPPUNIT_ASSERT(cl2.size() == 3);

    CPPUNIT_ASSERT(cl2.str() == "  1  2  3");
    CPPUNIT_ASSERT(cl2.str_plain() == "1 2 3");
  }

  void testOperators()
  {
    SLHALine l1(" 1 2 three # four");
    const SLHALine cl1 = l1;

    l1 = " 1 2 three #";
    CPPUNIT_ASSERT(l1.size() == 4);
    CPPUNIT_ASSERT(l1.data_size() == 3);
    CPPUNIT_ASSERT(l1[3] == "#");

    l1 += " four";
    CPPUNIT_ASSERT(l1.str() == cl1.str());
    CPPUNIT_ASSERT(l1.size() == 4);
    CPPUNIT_ASSERT(l1.data_size() == 3);
    CPPUNIT_ASSERT(cl1.size() == 4);

    l1 = "";
    CPPUNIT_ASSERT(l1.size() == 0);
    CPPUNIT_ASSERT(l1.data_size() == 0);
    CPPUNIT_ASSERT(l1.empty() == true);

    l1 = "1";
    CPPUNIT_ASSERT(l1.size() == 1);
    CPPUNIT_ASSERT(l1.data_size() == 1);
    CPPUNIT_ASSERT(l1.empty() == false);

    l1 += " 2";
    CPPUNIT_ASSERT(l1.size() == 2);
    CPPUNIT_ASSERT(l1.data_size() == 2);
    CPPUNIT_ASSERT(l1.empty() == false);

    l1 += "\n 3";
    CPPUNIT_ASSERT(l1.size() == 2);
    CPPUNIT_ASSERT(l1.data_size() == 2);
    CPPUNIT_ASSERT(l1.empty() == false);

    SLHALine l2[8];
    l2[0] << 1        << 2        << 3        << 4;
    l2[1] << 11       << 22       << 33       << 44;
    l2[2] << 111      << 222      << 333      << 444;
    l2[3] << 1111     << 2222     << 3333     << 4444;
    l2[4] << 11111    << 22222    << 33333    << 44444;
    l2[5] << 111111   << 222222   << 333333   << 444444;
    l2[6] << 1111111  << 2222222  << 3333333  << 4444444;
    l2[7] << 11111111 << 22222222 << 33333333 << 44444444;

                                // 1   2   3   4   5   6   7   8   9   0
    CPPUNIT_ASSERT(l2[0].str() == " 1  2   3   4");
    CPPUNIT_ASSERT(l2[1].str() == " 11     22  33  44");
    CPPUNIT_ASSERT(l2[2].str() == " 111    222     333     444");
    CPPUNIT_ASSERT(l2[3].str() == " 1111   2222    3333    4444");
    CPPUNIT_ASSERT(l2[4].str() == " 11111  22222   33333   44444");
    CPPUNIT_ASSERT(l2[5].str() == " 111111     222222  333333  444444");
    CPPUNIT_ASSERT(l2[6].str() == " 1111111    2222222     3333333     4444444");
    CPPUNIT_ASSERT(l2[7].str() == " 11111111   22222222    33333333    44444444");

    l1 = "";
    l1 << "BLOCK" << "MODSEL" << "# model selection";
    CPPUNIT_ASSERT(l1.str() == "BLOCK MODSEL    # model selection");

    l1 = "";
    l1 << 1 << 1 << "# mSUGRA";
    CPPUNIT_ASSERT(l1.str() == " 1  1   # mSUGRA");

    l1 = "";
    l1 << 2 << "-1.2345678E+01" << "# some double";
    CPPUNIT_ASSERT(l1.str() == " 2  -1.2345678E+01  # some double");

    l1 = "";
    l1 << 2 << "1.234567E+01" << "# some double" << " here";
    CPPUNIT_ASSERT(l1.str() == " 2  1.234567E+01    # some double here");
  }

  void testModifiers()
  {
    SLHALine l1(" 1 2 three # four");
    const SLHALine cl1(" 1 2 three # four");

    l1.clear();
    CPPUNIT_ASSERT(l1.empty() == true);

    l1.append(" 1 2");
    CPPUNIT_ASSERT(l1.size() == 2);
    CPPUNIT_ASSERT(l1.data_size() == 2);

    l1 += " three # four";
    CPPUNIT_ASSERT(l1.size() == 4);
    CPPUNIT_ASSERT(l1.data_size() == 3);
    CPPUNIT_ASSERT(l1.str() == cl1.str());

    l1.clear();
    CPPUNIT_ASSERT(l1.empty() == true);

    l1.str(" 1 2 three # four");
    CPPUNIT_ASSERT(l1.str() == cl1.str());

    l1.clear();
    l1 = " 1 2 three # four";
    CPPUNIT_ASSERT(l1.str() == cl1.str());
  }

  void testIterators()
  {
    SLHALine l1 = string(" one two three four # five ");
    const SLHALine cl1 = l1;

    CPPUNIT_ASSERT(*l1.begin() == *cl1.begin());
    CPPUNIT_ASSERT(*(l1.end()-1) == *(cl1.end()-1));
    CPPUNIT_ASSERT(*l1.rbegin() == *cl1.rbegin());
    CPPUNIT_ASSERT(*(l1.rend()-1) == *(cl1.rend()-1));

    CPPUNIT_ASSERT(l1.begin()+0 == l1.end()-5);
    CPPUNIT_ASSERT(l1.begin()+1 == l1.end()-4);
    CPPUNIT_ASSERT(l1.begin()+2 == l1.end()-3);
    CPPUNIT_ASSERT(l1.begin()+3 == l1.end()-2);
    CPPUNIT_ASSERT(l1.begin()+4 == l1.end()-1);

    CPPUNIT_ASSERT(l1.rbegin()+0 == l1.rend()-5);
    CPPUNIT_ASSERT(l1.rbegin()+1 == l1.rend()-4);
    CPPUNIT_ASSERT(l1.rbegin()+2 == l1.rend()-3);
    CPPUNIT_ASSERT(l1.rbegin()+3 == l1.rend()-2);
    CPPUNIT_ASSERT(l1.rbegin()+4 == l1.rend()-1);
  }

  void testMiscellaneous()
  {
    SLHALine l1;
    CPPUNIT_ASSERT(l1.str() == "");

    l1.str(" 1 2 3 4 ");
    CPPUNIT_ASSERT(l1.str() == " 1 2 3 4");
    CPPUNIT_ASSERT(l1.size() == 4);
    CPPUNIT_ASSERT(l1.data_size() == 4);
    CPPUNIT_ASSERT(l1.is_block_def()    == false);
    CPPUNIT_ASSERT(l1.is_comment_line() == false);
    CPPUNIT_ASSERT(l1.is_data_line()    == true);

    l1 = "BLOCK TEST # comment";
    CPPUNIT_ASSERT(l1.str() == "BLOCK TEST # comment");
    CPPUNIT_ASSERT(l1.data_size() == 2);
    CPPUNIT_ASSERT(l1[0] == "BLOCK");
    CPPUNIT_ASSERT(l1[1] == "TEST");
    CPPUNIT_ASSERT(l1[2] == "# comment");
    CPPUNIT_ASSERT(l1.is_block_def()    == true);
    CPPUNIT_ASSERT(l1.is_comment_line() == false);
    CPPUNIT_ASSERT(l1.is_data_line()    == false);

    l1 = "BLOCK  TEST  # comment";
    CPPUNIT_ASSERT(l1.str() == "BLOCK  TEST  # comment");
    CPPUNIT_ASSERT(l1.data_size() == 2);
    CPPUNIT_ASSERT(l1[0] == "BLOCK");
    CPPUNIT_ASSERT(l1[1] == "TEST");
    CPPUNIT_ASSERT(l1[2] == "# comment");
    CPPUNIT_ASSERT(l1.is_block_def()    == true);
    CPPUNIT_ASSERT(l1.is_comment_line() == false);
    CPPUNIT_ASSERT(l1.is_data_line()    == false);

    l1 = "# one long comment with trailing spaces    ";
    CPPUNIT_ASSERT(l1[0] == "# one long comment with trailing spaces");
    CPPUNIT_ASSERT(l1.size() == 1);
    CPPUNIT_ASSERT(l1.data_size() == 0);
    CPPUNIT_ASSERT(l1.is_block_def()    == false);
    CPPUNIT_ASSERT(l1.is_comment_line() == true);
    CPPUNIT_ASSERT(l1.is_data_line()    == false);

    l1 = "one long data line with trailing spaces    ";
    CPPUNIT_ASSERT(l1.str() == "one long data line with trailing spaces");
    CPPUNIT_ASSERT(l1[6] == "spaces");
    CPPUNIT_ASSERT(l1.size() == 7);
    CPPUNIT_ASSERT(l1.data_size() == 7);
    CPPUNIT_ASSERT(l1.is_block_def()    == false);
    CPPUNIT_ASSERT(l1.is_comment_line() == false);
    CPPUNIT_ASSERT(l1.is_data_line()    == true);

    l1 = "Hello brave new \n world!";
    CPPUNIT_ASSERT(l1.str() == "Hello brave new");
    CPPUNIT_ASSERT(l1.size() == 3);
    CPPUNIT_ASSERT(l1.data_size() == 3);

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
    CPPUNIT_ASSERT(l1.size() == 0);
    CPPUNIT_ASSERT(l1.data_size() == 0);
    l1 = "  \n 1 2 3 4 5  ";
    CPPUNIT_ASSERT(l1.empty() == true);
    CPPUNIT_ASSERT(l1.size() == 0);
    CPPUNIT_ASSERT(l1.data_size() == 0);
    l1 = "";
    CPPUNIT_ASSERT(l1.empty() == true);
    CPPUNIT_ASSERT(l1.size() == 0);
    CPPUNIT_ASSERT(l1.data_size() == 0);
    l1 = "   ";
    CPPUNIT_ASSERT(l1.empty() == true);
    CPPUNIT_ASSERT(l1.size() == 0);
    CPPUNIT_ASSERT(l1.data_size() == 0);
    l1 = " . ";
    CPPUNIT_ASSERT(l1.empty() == false);
    CPPUNIT_ASSERT(l1.size() == 1);
    CPPUNIT_ASSERT(l1.data_size() == 1);

    l1 = " 1 22 333 4444 ";
    l1.append(" 55555 # a comment ");
    CPPUNIT_ASSERT(l1.size() == 6);
    CPPUNIT_ASSERT(l1.data_size() == 5);
    CPPUNIT_ASSERT(l1[3] == "4444");
    CPPUNIT_ASSERT(l1[4] == "55555");
    CPPUNIT_ASSERT(l1[5] == "# a comment");
    l1 += " 7 ";
    CPPUNIT_ASSERT(l1[5] == "# a comment 7");

    l1 = " test  str() and   str_plain() ";
    CPPUNIT_ASSERT(l1.str() == " test  str() and   str_plain()");
    CPPUNIT_ASSERT(l1.str_plain() == "test str() and str_plain()");

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
  }
};

} // namespace SLHAea

#endif // SLHAEA_TESTSLHALINE_H

// vim: sw=2 tw=78
