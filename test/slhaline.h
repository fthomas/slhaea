// SLHAea - another SUSY Les Houches Accord input/output library
// Copyright © 2009-2010 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file ../LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <stdexcept>
#include <string>
#include "slhaea.h"

using namespace std;
using namespace SLHAea;

BOOST_AUTO_TEST_SUITE(TestSLHALine)

BOOST_AUTO_TEST_CASE(testConstructors)
{
  SLHALine l1;
  BOOST_CHECK(l1.empty() == true);
  BOOST_CHECK(l1.size() == 0);

  SLHALine l2(" 1 2 3 4 ");
  BOOST_CHECK(l2.empty() == false);
  BOOST_CHECK(l2.size() == 4);

  SLHALine l3(" 1 2 3 4 \n 5 6 ");
  BOOST_CHECK(l3.empty() == false);
  BOOST_CHECK(l3.size() == 4);

  const SLHALine l4(" 1 2 3 4 # 5 6 ");
  BOOST_CHECK(l4.empty() == false);
  BOOST_CHECK(l4.size() == 5);

  const SLHALine l5(" \n 1 2 3 4 # 5 6 ");
  BOOST_CHECK(l5.empty() == true);
  BOOST_CHECK(l5.size() == 0);
}

BOOST_AUTO_TEST_CASE(testAccessors)
{
  SLHALine l1("1");
  const SLHALine cl1 = l1;
  BOOST_CHECK(l1[0] == "1");
  BOOST_CHECK(cl1[0] == "1");
  BOOST_CHECK(l1[0] == l1.at(0));
  BOOST_CHECK(cl1[0] == cl1.at(0));
  BOOST_CHECK(l1.front() == l1.at(0));
  BOOST_CHECK(l1.back() == l1.at(0));
  BOOST_CHECK(cl1.front() == l1.at(0));
  BOOST_CHECK(cl1.back() == l1.at(0));

  l1[0] = " 1 2 ";
  BOOST_CHECK(l1[0] == " 1 2 ");
  BOOST_CHECK(l1.size() == 1);
  l1.at(0) = "";
  BOOST_CHECK(l1.empty() == false);

  try
  {
    l1.at(1) = "3";
  }
  catch (out_of_range)
  {
    BOOST_CHECK(l1.empty() == false);
  }

  l1 = "  1  2  3  ";
  const SLHALine cl2 = l1;
  BOOST_CHECK(l1.front() == "1");
  BOOST_CHECK(cl2.front() == "1");
  BOOST_CHECK(l1.back() == "3");
  BOOST_CHECK(cl2.back() == "3");
  BOOST_CHECK(l1.size() == 3);
  BOOST_CHECK(cl2.size() == 3);

  BOOST_CHECK(cl2.str() == "  1  2  3");
  BOOST_CHECK(cl2.str_plain() == "1 2 3");
}

BOOST_AUTO_TEST_CASE(testOperators)
{
  SLHALine l1(" 1 2 three # four");
  const SLHALine cl1 = l1;

  l1 = " 1 2 three #";
  BOOST_CHECK(l1.size() == 4);
  BOOST_CHECK(l1.data_size() == 3);
  BOOST_CHECK(l1[3] == "#");

  l1 += " four";
  BOOST_CHECK(l1.str() == cl1.str());
  BOOST_CHECK(l1.size() == 4);
  BOOST_CHECK(l1.data_size() == 3);
  BOOST_CHECK(cl1.size() == 4);

  l1 = "";
  BOOST_CHECK(l1.size() == 0);
  BOOST_CHECK(l1.data_size() == 0);
  BOOST_CHECK(l1.empty() == true);

  l1 = "1";
  BOOST_CHECK(l1.size() == 1);
  BOOST_CHECK(l1.data_size() == 1);
  BOOST_CHECK(l1.empty() == false);

  l1 += " 2";
  BOOST_CHECK(l1.size() == 2);
  BOOST_CHECK(l1.data_size() == 2);
  BOOST_CHECK(l1.empty() == false);

  l1 += "\n 3";
  BOOST_CHECK(l1.size() == 2);
  BOOST_CHECK(l1.data_size() == 2);
  BOOST_CHECK(l1.empty() == false);

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
  BOOST_CHECK(l2[0].str() == " 1  2   3   4");
  BOOST_CHECK(l2[1].str() == " 11     22  33  44");
  BOOST_CHECK(l2[2].str() == " 111    222     333     444");
  BOOST_CHECK(l2[3].str() == " 1111   2222    3333    4444");
  BOOST_CHECK(l2[4].str() == " 11111  22222   33333   44444");
  BOOST_CHECK(l2[5].str() == " 111111     222222  333333  444444");
  BOOST_CHECK(l2[6].str() == " 1111111    2222222     3333333     4444444");
  BOOST_CHECK(l2[7].str() == " 11111111   22222222    33333333    44444444");

  l1 = "";
  l1 << "BLOCK" << "MODSEL" << "# model selection";
  BOOST_CHECK(l1.str() == "BLOCK MODSEL    # model selection");

  l1 = "";
  l1 << 1 << 1 << "# mSUGRA";
  BOOST_CHECK(l1.str() == " 1  1   # mSUGRA");

  l1 = "";
  l1 << 2 << "-1.2345678E+01" << "# some double";
  BOOST_CHECK(l1.str() == " 2  -1.2345678E+01  # some double");

  l1 = "";
  l1 << 2 << "1.234567E+01" << "# some double" << " here";
  BOOST_CHECK(l1.str() == " 2  1.234567E+01    # some double here");
}

BOOST_AUTO_TEST_CASE(testModifiers)
{
  SLHALine l1(" 1 2 three # four");
  const SLHALine cl1(" 1 2 three # four");

  l1.clear();
  BOOST_CHECK(l1.empty() == true);

  l1.append(" 1 2");
  BOOST_CHECK(l1.size() == 2);
  BOOST_CHECK(l1.data_size() == 2);

  l1 += " three # four";
  BOOST_CHECK(l1.size() == 4);
  BOOST_CHECK(l1.data_size() == 3);
  BOOST_CHECK(l1.str() == cl1.str());

  l1.clear();
  BOOST_CHECK(l1.empty() == true);

  l1.str(" 1 2 three # four");
  BOOST_CHECK(l1.str() == cl1.str());

  l1.clear();
  l1 = " 1 2 three # four";
  BOOST_CHECK(l1.str() == cl1.str());
}

BOOST_AUTO_TEST_CASE(testIterators)
{
  SLHALine l1 = string(" one two three four # five ");
  const SLHALine cl1 = l1;
  const SLHALine cl2 = l1.str();
  BOOST_CHECK(cl1 == cl2);

  BOOST_CHECK(*l1.begin() == *cl1.begin());
  BOOST_CHECK(*(l1.end()-1) == *(cl1.end()-1));
  BOOST_CHECK(*l1.rbegin() == *cl1.rbegin());
  BOOST_CHECK(*(l1.rend()-1) == *(cl1.rend()-1));

  BOOST_CHECK(l1.begin()+0 == l1.end()-5);
  BOOST_CHECK(l1.begin()+1 == l1.end()-4);
  BOOST_CHECK(l1.begin()+2 == l1.end()-3);
  BOOST_CHECK(l1.begin()+3 == l1.end()-2);
  BOOST_CHECK(l1.begin()+4 == l1.end()-1);

  BOOST_CHECK(l1.rbegin()+0 == l1.rend()-5);
  BOOST_CHECK(l1.rbegin()+1 == l1.rend()-4);
  BOOST_CHECK(l1.rbegin()+2 == l1.rend()-3);
  BOOST_CHECK(l1.rbegin()+3 == l1.rend()-2);
  BOOST_CHECK(l1.rbegin()+4 == l1.rend()-1);
}

BOOST_AUTO_TEST_CASE(testMiscellaneous)
{
  SLHALine l1;
  BOOST_CHECK(l1.str() == "");

  l1.str(" 1 2 3 4 ");
  BOOST_CHECK(l1.str() == " 1 2 3 4");
  BOOST_CHECK(l1.size() == 4);
  BOOST_CHECK(l1.data_size() == 4);
  BOOST_CHECK(l1.is_block_def()    == false);
  BOOST_CHECK(l1.is_comment_line() == false);
  BOOST_CHECK(l1.is_data_line()    == true);

  l1 = "BLOCK TEST # comment";
  BOOST_CHECK(l1.str() == "BLOCK TEST # comment");
  BOOST_CHECK(l1.data_size() == 2);
  BOOST_CHECK(l1[0] == "BLOCK");
  BOOST_CHECK(l1[1] == "TEST");
  BOOST_CHECK(l1[2] == "# comment");
  BOOST_CHECK(l1.is_block_def()    == true);
  BOOST_CHECK(l1.is_comment_line() == false);
  BOOST_CHECK(l1.is_data_line()    == false);

  l1 = "BLOCK  TEST  # comment";
  BOOST_CHECK(l1.str() == "BLOCK  TEST  # comment");
  BOOST_CHECK(l1.data_size() == 2);
  BOOST_CHECK(l1[0] == "BLOCK");
  BOOST_CHECK(l1[1] == "TEST");
  BOOST_CHECK(l1[2] == "# comment");
  BOOST_CHECK(l1.is_block_def()    == true);
  BOOST_CHECK(l1.is_comment_line() == false);
  BOOST_CHECK(l1.is_data_line()    == false);

  l1 = "# one long comment with trailing spaces    ";
  BOOST_CHECK(l1[0] == "# one long comment with trailing spaces");
  BOOST_CHECK(l1.size() == 1);
  BOOST_CHECK(l1.data_size() == 0);
  BOOST_CHECK(l1.is_block_def()    == false);
  BOOST_CHECK(l1.is_comment_line() == true);
  BOOST_CHECK(l1.is_data_line()    == false);

  l1 = "one long data line with trailing spaces    ";
  BOOST_CHECK(l1.str() == "one long data line with trailing spaces");
  BOOST_CHECK(l1[6] == "spaces");
  BOOST_CHECK(l1.size() == 7);
  BOOST_CHECK(l1.data_size() == 7);
  BOOST_CHECK(l1.is_block_def()    == false);
  BOOST_CHECK(l1.is_comment_line() == false);
  BOOST_CHECK(l1.is_data_line()    == true);

  l1 = "Hello brave new \n world!";
  BOOST_CHECK(l1.str() == "Hello brave new");
  BOOST_CHECK(l1.size() == 3);
  BOOST_CHECK(l1.data_size() == 3);

  l1 = "Hello stupid brave new world!";
  BOOST_CHECK(l1.str() == "Hello stupid brave new world!");
  l1[1] = "";
  BOOST_CHECK(l1.str() == "Hello        brave new world!");
  l1[1] = "dumb";
  BOOST_CHECK(l1.str() == "Hello dumb   brave new world!");
  l1[3] = "brand-new";
  BOOST_CHECK(l1.str() == "Hello dumb   brave brand-new world!");
  l1[0] = "Goodbye";
  BOOST_CHECK(l1.str() == "Goodbye dumb brave brand-new world!");

  l1 = " 23  1.123456789E+999  # some arcane value";
  BOOST_CHECK(l1[0] == "23");
  BOOST_CHECK(l1[1] == "1.123456789E+999");
  BOOST_CHECK(l1[2] == "# some arcane value");

  l1 = "\n 1 2 3 4 5";
  BOOST_CHECK(l1.empty() == true);
  BOOST_CHECK(l1.size() == 0);
  BOOST_CHECK(l1.data_size() == 0);
  l1 = "  \n 1 2 3 4 5  ";
  BOOST_CHECK(l1.empty() == true);
  BOOST_CHECK(l1.size() == 0);
  BOOST_CHECK(l1.data_size() == 0);
  l1 = "";
  BOOST_CHECK(l1.empty() == true);
  BOOST_CHECK(l1.size() == 0);
  BOOST_CHECK(l1.data_size() == 0);
  l1 = "   ";
  BOOST_CHECK(l1.empty() == true);
  BOOST_CHECK(l1.size() == 0);
  BOOST_CHECK(l1.data_size() == 0);
  l1 = " . ";
  BOOST_CHECK(l1.empty() == false);
  BOOST_CHECK(l1.size() == 1);
  BOOST_CHECK(l1.data_size() == 1);

  l1 = " 1 22 333 4444 ";
  l1.append(" 55555 # a comment ");
  BOOST_CHECK(l1.size() == 6);
  BOOST_CHECK(l1.data_size() == 5);
  BOOST_CHECK(l1[3] == "4444");
  BOOST_CHECK(l1[4] == "55555");
  BOOST_CHECK(l1[5] == "# a comment");
  l1 += " 7 ";
  BOOST_CHECK(l1[5] == "# a comment 7");

  l1 = " test  str() and   str_plain() ";
  BOOST_CHECK(l1.str() == " test  str() and   str_plain()");
  BOOST_CHECK(l1.str_plain() == "test str() and str_plain()");

  l1 = " 1 2 3 4 5 ";
  BOOST_CHECK(l1.back() == "5");
  l1.back() = "6";
  BOOST_CHECK(l1.back() == "6");
  BOOST_CHECK(l1.front() == "1");
  l1.front() = "-1";
  BOOST_CHECK(l1.front() == "-1");

  BOOST_CHECK(l1.front() == *l1.begin());
  BOOST_CHECK(l1.back() == *(l1.end()-1));
  BOOST_CHECK(*(l1.end()-2) == "4");
  *l1.begin() = "0";
  BOOST_CHECK(l1.front() == "0");
  *(l1.end()-1) = "7";
  BOOST_CHECK(l1.back() == "7");

  BOOST_CHECK(*l1.rbegin() == l1.back());
  BOOST_CHECK(*(l1.rend()-1) == l1.front());
  *l1.rbegin() = "8";
  BOOST_CHECK(l1.back() == "8");
  *(l1.rend()-1) = "-2";
  BOOST_CHECK(l1.front() == "-2");
}

BOOST_AUTO_TEST_SUITE_END()

// vim: sw=2 tw=78
