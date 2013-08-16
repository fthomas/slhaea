// SLHAea - containers for SUSY Les Houches Accord input/output
// Copyright © 2009-2010 Frank S. Thomas <frank@timepit.eu>
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file ../../LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>
#include <limits>
#include <string>
#include <vector>
#include <boost/concept/assert.hpp>
#include <boost/test/unit_test.hpp>
#include "slhaea.h"

using namespace std;
using namespace boost;
using namespace SLHAea;

BOOST_AUTO_TEST_SUITE(TestLine)

BOOST_AUTO_TEST_CASE(assertConcepts)
{
  BOOST_CONCEPT_ASSERT((Mutable_RandomAccessContainer<Line>));

  BOOST_CONCEPT_ASSERT((Mutable_RandomAccessIterator<Line::iterator>));
  BOOST_CONCEPT_ASSERT((Mutable_RandomAccessIterator<Line::reverse_iterator>));
  BOOST_CONCEPT_ASSERT((RandomAccessIterator<Line::const_iterator>));
  BOOST_CONCEPT_ASSERT((RandomAccessIterator<Line::const_reverse_iterator>));
}

BOOST_AUTO_TEST_CASE(testConstructors)
{
  string init = "";
  Line l1;
  BOOST_CHECK_EQUAL(l1.str(),   init);
  BOOST_CHECK_EQUAL(l1.empty(), true);
  BOOST_CHECK_EQUAL(l1.size(),  0);

  init = " 1 2 3 4 ";
  Line l2(init);
  BOOST_CHECK_EQUAL(l2.str(),   " 1 2 3 4");
  BOOST_CHECK_EQUAL(l2.empty(), false);
  BOOST_CHECK_EQUAL(l2.size(),  4);

  init = " 1 2 3 4 \n 5 6 ";
  Line l3(init);
  BOOST_CHECK_EQUAL(l3.str(),   " 1 2 3 4");
  BOOST_CHECK_EQUAL(l3.empty(), false);
  BOOST_CHECK_EQUAL(l3.size(),  4);

  init = " 1 2 3 4 # 5 6 ";
  const Line l4(init);
  BOOST_CHECK_EQUAL(l4.str(),   " 1 2 3 4 # 5 6");
  BOOST_CHECK_EQUAL(l4.empty(), false);
  BOOST_CHECK_EQUAL(l4.size(),  5);

  init = " \n 1 2 3 4 # 5 6 ";
  const Line l5(init);
  BOOST_CHECK_EQUAL(l5.str(),   "");
  BOOST_CHECK_EQUAL(l5.empty(), true);
  BOOST_CHECK_EQUAL(l5.size(),  0);
}

BOOST_AUTO_TEST_CASE(testAssignmentOperator)
{
  Line l1;

  string init = "1 2 three # four";
  l1 = init;
  BOOST_CHECK_EQUAL(l1.str(),       init);
  BOOST_CHECK_EQUAL(l1.size(),      4);
  BOOST_CHECK_EQUAL(l1.data_size(), 3);

  l1 = "";
  BOOST_CHECK_EQUAL(l1.str(),       "");
  BOOST_CHECK_EQUAL(l1.empty(),     true);
  BOOST_CHECK_EQUAL(l1.size(),      0);
  BOOST_CHECK_EQUAL(l1.data_size(), 0);

  l1 = "\n 1 2 3";
  BOOST_CHECK_EQUAL(l1.str(),       "");
  BOOST_CHECK_EQUAL(l1.empty(),     true);
  BOOST_CHECK_EQUAL(l1.size(),      0);
  BOOST_CHECK_EQUAL(l1.data_size(), 0);

  l1.str("  aa 1 bb 1 cc 1 dd  ");
  BOOST_CHECK_EQUAL(l1.str(),       "  aa 1 bb 1 cc 1 dd");
  BOOST_CHECK_EQUAL(l1.size(),      7);
  BOOST_CHECK_EQUAL(l1.data_size(), 7);

  l1.str("#  aa 1 bb 1 cc 1 dd  \n 11 22 33");
  BOOST_CHECK_EQUAL(l1.str(),       "#  aa 1 bb 1 cc 1 dd");
  BOOST_CHECK_EQUAL(l1.size(),      1);
  BOOST_CHECK_EQUAL(l1.data_size(), 0);

  l1.str("111");
  BOOST_CHECK_EQUAL(l1.str(),       "111");
  BOOST_CHECK_EQUAL(l1.size(),      1);
  BOOST_CHECK_EQUAL(l1.data_size(), 1);

  l1.str("#111");
  BOOST_CHECK_EQUAL(l1.str(),       "#111");
  BOOST_CHECK_EQUAL(l1.size(),      1);
  BOOST_CHECK_EQUAL(l1.data_size(), 0);

  l1.str(" 1\t2\v3\f4\r5\n");
  BOOST_CHECK_EQUAL(l1.str(),       " 1 2 3 4 5");
  BOOST_CHECK_EQUAL(l1.size(),      5);
  BOOST_CHECK_EQUAL(l1.data_size(), 5);

  l1.str("a");
  BOOST_CHECK_EQUAL(l1.str(),       "a");
  BOOST_CHECK_EQUAL(l1.size(),      1);
  BOOST_CHECK_EQUAL(l1.data_size(), 1);

  l1.str("  1  2  3  4");
  l1[0] = "1111";
  l1[1] = "2222";
  l1[2] = "3333";
  l1[3] = "4444";
  BOOST_CHECK_EQUAL(l1.str(),       "  1111 2222 3333 4444");
  BOOST_CHECK_EQUAL(l1.size(),      4);
  BOOST_CHECK_EQUAL(l1.data_size(), 4);
}

BOOST_AUTO_TEST_CASE(testAppending)
{
  Line l1;

  l1 = "1 2 aaa";
  BOOST_CHECK_EQUAL(l1.str(),       "1 2 aaa");
  BOOST_CHECK_EQUAL(l1.size(),      3);
  BOOST_CHECK_EQUAL(l1.data_size(), 3);

  l1 += " bbb";
  BOOST_CHECK_EQUAL(l1.str(),       "1 2 aaa bbb");
  BOOST_CHECK_EQUAL(l1.size(),      4);
  BOOST_CHECK_EQUAL(l1.data_size(), 4);

  l1.append(" # ccc");
  BOOST_CHECK_EQUAL(l1.str(),       "1 2 aaa bbb # ccc");
  BOOST_CHECK_EQUAL(l1.size(),      5);
  BOOST_CHECK_EQUAL(l1.data_size(), 4);

  l1.clear();
  l1 += "1 2";
  BOOST_CHECK_EQUAL(l1.str(),       "1 2");
  BOOST_CHECK_EQUAL(l1.size(),      2);
  BOOST_CHECK_EQUAL(l1.data_size(), 2);

  l1 += "\n 3 4 5";
  BOOST_CHECK_EQUAL(l1.str(),       "1 2");
  BOOST_CHECK_EQUAL(l1.size(),      2);
  BOOST_CHECK_EQUAL(l1.data_size(), 2);
}

BOOST_AUTO_TEST_CASE(testInserting)
{
  Line l1[8];
  l1[0] << 1        << 2        << 3        << 4;
  l1[1] << 11       << 22       << 33       << 44;
  l1[2] << 111      << 222      << 333      << 444;
  l1[3] << 1111     << 2222     << 3333     << 4444;
  l1[4] << 11111    << 22222    << 33333    << 44444;
  l1[5] << 111111   << 222222   << 333333   << 444444;
  l1[6] << 1111111  << 2222222  << 3333333  << 4444444;
  l1[7] << 11111111 << 22222222 << 33333333 << 44444444;

                              // "    1   2   3   4   5   6   7   8   9   0"
  BOOST_CHECK_EQUAL(l1[0].str(), "    1   2   3   4");
  BOOST_CHECK_EQUAL(l1[1].str(), "    11  22  33  44");
  BOOST_CHECK_EQUAL(l1[2].str(), "    111     222     333     444");
  BOOST_CHECK_EQUAL(l1[3].str(), "    1111    2222    3333    4444");
  BOOST_CHECK_EQUAL(l1[4].str(), "    11111   22222   33333   44444");
  BOOST_CHECK_EQUAL(l1[5].str(), "    111111  222222  333333  444444");
  BOOST_CHECK_EQUAL(l1[6].str(), "    1111111     2222222     3333333     4444444");
  BOOST_CHECK_EQUAL(l1[7].str(), "    11111111    22222222    33333333    44444444");

  Line l2;

  l2 << "BLOCK" << "MODSEL" << "# model selection";
  BOOST_CHECK_EQUAL(l2.str(), "BLOCK MODSEL    # model selection");

  l2.clear();
  l2 << 1 << 1 << "# mSUGRA";
  BOOST_CHECK_EQUAL(l2.str(), "    1   1   # mSUGRA");

  l2.clear();
  l2 << 2 << "-1.2345678E+01" << "# some double";
  BOOST_CHECK_EQUAL(l2.str(), "    2  -1.2345678E+01   # some double");

  l2.clear();
  l2 << 2 << "1.234567E+01" << "# some double" << " here";
  BOOST_CHECK_EQUAL(l2.str(), "    2   1.234567E+01    # some double here");

  l2.clear();
  l2 << 1 << 2 << 3 << 4 << 5 << 6;
  BOOST_CHECK_EQUAL(l2.str(), "    1   2   3   4   5   6");

  l2.clear();
  l2 << "" << " " << "\t \n" << "1" << 2 << "\n" << "# test" << "\r";
  BOOST_CHECK_EQUAL(l2.str(), "    1   2   # test");

  l2.clear();
  l2 << "# test" << "\n \t" << "  1" << " 2" << 3 << " # more";
  BOOST_CHECK_EQUAL(l2.str(), "# test  1 23 # more");

  l2.clear();
  l2 << "# Hello " << " world " << "! ";
  BOOST_CHECK_EQUAL(l2.str(), "# Hello world!");

  Line l3;
  l2.clear();
  l3.clear();

  l2 << 1 << -1 << "# test";
  l3 << 2 << "+2" << "# test";
  BOOST_CHECK_EQUAL(l2.str(), "    1  -1   # test");
  BOOST_CHECK_EQUAL(l3.str(), "    2  +2   # test");
}

BOOST_AUTO_TEST_CASE(testFloatInserting)
{
  const int digits_f  = numeric_limits<float>::digits10;
  const int digits_d  = numeric_limits<double>::digits10;
  const int digits_ld = numeric_limits<long double>::digits10;

  const long double ld = 1.234567890123456789012345678901234567890L;
  Line l1;

  l1.clear();
  l1 << 1 << float(ld);
  BOOST_CHECK_EQUAL(l1.str(), "    1   " + to_string(ld, digits_f));

  l1.clear();
  l1 << 2 << double(ld);
  BOOST_CHECK_EQUAL(l1.str(), "    2   " + to_string(ld, digits_d));

  l1.clear();
  l1 << 3 << (long double)(ld);
  BOOST_CHECK_EQUAL(l1.str(), "    3   " + to_string(ld, digits_ld));
}

BOOST_AUTO_TEST_CASE(testSubscriptAccessor)
{
  Line l1("1 2 3 # 4 5");
  const Line cl1 = l1;

  BOOST_CHECK_EQUAL(l1[0], "1");
  BOOST_CHECK_EQUAL(l1[1], "2");
  BOOST_CHECK_EQUAL(l1[2], "3");
  BOOST_CHECK_EQUAL(l1[3], "# 4 5");
  BOOST_CHECK_EQUAL(l1.size(), 4);

  BOOST_CHECK_EQUAL(cl1[0], "1");
  BOOST_CHECK_EQUAL(cl1[1], "2");
  BOOST_CHECK_EQUAL(cl1[2], "3");
  BOOST_CHECK_EQUAL(cl1[3], "# 4 5");
  BOOST_CHECK_EQUAL(cl1.size(), 4);

  l1[0] = "5";
  l1[1] = "4";
  l1[2] = "3";
  l1[3] = "# 2 1";

  BOOST_CHECK_EQUAL(l1[0], "5");
  BOOST_CHECK_EQUAL(l1[1], "4");
  BOOST_CHECK_EQUAL(l1[2], "3");
  BOOST_CHECK_EQUAL(l1[3], "# 2 1");
  BOOST_CHECK_EQUAL(l1.size(), 4);
}

BOOST_AUTO_TEST_CASE(testAtAccessor)
{
  const Line cl1("1 2 3 # 4 5");

  BOOST_CHECK_EQUAL(cl1.at(0), "1");
  BOOST_CHECK_EQUAL(cl1.at(1), "2");
  BOOST_CHECK_EQUAL(cl1.at(2), "3");
  BOOST_CHECK_EQUAL(cl1.at(3), "# 4 5");
  BOOST_CHECK_EQUAL(cl1.size(), 4);

  Line l1 = cl1;
  l1.at(0) = "5";
  l1.at(1) = "4";
  l1.at(2) = "3";
  l1.at(3) = "# 2 1";

  BOOST_CHECK_EQUAL(l1.at(0), "5");
  BOOST_CHECK_EQUAL(l1.at(1), "4");
  BOOST_CHECK_EQUAL(l1.at(2), "3");
  BOOST_CHECK_EQUAL(l1.at(3), "# 2 1");
  BOOST_CHECK_EQUAL(l1.size(), 4);
}

BOOST_AUTO_TEST_CASE(testGeneralAccessors)
{
  Line l1("1");
  const Line cl1 = l1;

  BOOST_CHECK_EQUAL(l1.front(),  l1.at(0));
  BOOST_CHECK_EQUAL(l1.back(),   l1.at(0));
  BOOST_CHECK_EQUAL(l1.front(),  l1.back());
  BOOST_CHECK_EQUAL(cl1.front(), cl1.at(0));
  BOOST_CHECK_EQUAL(cl1.back(),  cl1.at(0));
  BOOST_CHECK_EQUAL(cl1.front(), cl1.back());
  BOOST_CHECK_EQUAL(l1.size(),   1);
  BOOST_CHECK_EQUAL(cl1.size(),  1);

  Line l2(" 11 22 ");
  const Line cl2 = l2;

  BOOST_CHECK_EQUAL(l2.front(),  l2.at(0));
  BOOST_CHECK_EQUAL(l2.back(),   l2.at(1));
  BOOST_CHECK_NE(l2.front(),     l2.back());
  BOOST_CHECK_EQUAL(cl2.front(), cl2.at(0));
  BOOST_CHECK_EQUAL(cl2.back(),  cl2.at(1));
  BOOST_CHECK_NE(cl2.front(),    cl2.back());
  BOOST_CHECK_EQUAL(l2.size(),   2);
  BOOST_CHECK_EQUAL(cl2.size(),  2);

  Line l3("  11  22  33  ");
  const Line cl3 = l3;

  BOOST_CHECK_EQUAL(l3.front(),  "11");
  BOOST_CHECK_EQUAL(l3.back(),   "33");
  BOOST_CHECK_NE(l3.front(),     l3.back());
  BOOST_CHECK_EQUAL(cl3.front(), "11");
  BOOST_CHECK_EQUAL(cl3.back(),  "33");
  BOOST_CHECK_NE(cl3.front(),    cl3.back());
  BOOST_CHECK_EQUAL(l3.size(),   3);
  BOOST_CHECK_EQUAL(cl3.size(),  3);
}

BOOST_AUTO_TEST_CASE(testIterators)
{
  Line l1(" one two three four # five ");
  const Line cl1 = l1;

  BOOST_CHECK_EQUAL(*(l1.begin()),  *(cl1.begin()));
  BOOST_CHECK_EQUAL(*(l1.end()-1),  *(cl1.end()-1));
  BOOST_CHECK_EQUAL(*(l1.rbegin()), *(cl1.rbegin()));
  BOOST_CHECK_EQUAL(*(l1.rend()-1), *(cl1.rend()-1));

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

  BOOST_CHECK(l1.cbegin()+0 == l1.cend()-5);
  BOOST_CHECK(l1.cbegin()+1 == l1.cend()-4);
  BOOST_CHECK(l1.cbegin()+2 == l1.cend()-3);
  BOOST_CHECK(l1.cbegin()+3 == l1.cend()-2);
  BOOST_CHECK(l1.cbegin()+4 == l1.cend()-1);

  BOOST_CHECK(l1.crbegin()+0 == l1.crend()-5);
  BOOST_CHECK(l1.crbegin()+1 == l1.crend()-4);
  BOOST_CHECK(l1.crbegin()+2 == l1.crend()-3);
  BOOST_CHECK(l1.crbegin()+3 == l1.crend()-2);
  BOOST_CHECK(l1.crbegin()+4 == l1.crend()-1);

  BOOST_CHECK(cl1.begin()+0 == cl1.end()-5);
  BOOST_CHECK(cl1.begin()+1 == cl1.end()-4);
  BOOST_CHECK(cl1.begin()+2 == cl1.end()-3);
  BOOST_CHECK(cl1.begin()+3 == cl1.end()-2);
  BOOST_CHECK(cl1.begin()+4 == cl1.end()-1);

  BOOST_CHECK(cl1.rbegin()+0 == cl1.rend()-5);
  BOOST_CHECK(cl1.rbegin()+1 == cl1.rend()-4);
  BOOST_CHECK(cl1.rbegin()+2 == cl1.rend()-3);
  BOOST_CHECK(cl1.rbegin()+3 == cl1.rend()-2);
  BOOST_CHECK(cl1.rbegin()+4 == cl1.rend()-1);

  BOOST_CHECK(cl1.cbegin()+0 == cl1.cend()-5);
  BOOST_CHECK(cl1.cbegin()+1 == cl1.cend()-4);
  BOOST_CHECK(cl1.cbegin()+2 == cl1.cend()-3);
  BOOST_CHECK(cl1.cbegin()+3 == cl1.cend()-2);
  BOOST_CHECK(cl1.cbegin()+4 == cl1.cend()-1);

  BOOST_CHECK(cl1.crbegin()+0 == cl1.crend()-5);
  BOOST_CHECK(cl1.crbegin()+1 == cl1.crend()-4);
  BOOST_CHECK(cl1.crbegin()+2 == cl1.crend()-3);
  BOOST_CHECK(cl1.crbegin()+3 == cl1.crend()-2);
  BOOST_CHECK(cl1.crbegin()+4 == cl1.crend()-1);

  *(l1.begin()+0) = "1";
  *(l1.begin()+1) = "2";
  *(l1.begin()+2) = "3";
  *(l1.begin()+3) = "4";
  *(l1.begin()+4) = "# 5";
                           // " one two three four # five "
  BOOST_CHECK_EQUAL(l1.str(), " 1   2   3     4    # 5");
}

BOOST_AUTO_TEST_CASE(testIntrospection)
{
  Line l1;
  BOOST_CHECK_GT(l1.max_size(), 0);

  l1 = "# 1 2 3";
  BOOST_CHECK_EQUAL(l1.size(),      1);
  BOOST_CHECK_EQUAL(l1.data_size(), 0);
  BOOST_CHECK_EQUAL(l1.is_block_def(),    false);
  BOOST_CHECK_EQUAL(l1.is_comment_line(), true);
  BOOST_CHECK_EQUAL(l1.is_data_line(),    false);

  l1 = " # 1 2 3 ";
  BOOST_CHECK_EQUAL(l1.size(),      1);
  BOOST_CHECK_EQUAL(l1.data_size(), 0);
  BOOST_CHECK_EQUAL(l1.is_block_def(),    false);
  BOOST_CHECK_EQUAL(l1.is_comment_line(), true);
  BOOST_CHECK_EQUAL(l1.is_data_line(),    false);

  l1 = "BlOcK";
  BOOST_CHECK_EQUAL(l1.size(),      1);
  BOOST_CHECK_EQUAL(l1.data_size(), 1);
  BOOST_CHECK_EQUAL(l1.is_block_def(),    false);
  BOOST_CHECK_EQUAL(l1.is_comment_line(), false);
  BOOST_CHECK_EQUAL(l1.is_data_line(),    false);

  l1 = "bLoCk TEST";
  BOOST_CHECK_EQUAL(l1.size(),      2);
  BOOST_CHECK_EQUAL(l1.data_size(), 2);
  BOOST_CHECK_EQUAL(l1.is_block_def(),    true);
  BOOST_CHECK_EQUAL(l1.is_comment_line(), false);
  BOOST_CHECK_EQUAL(l1.is_data_line(),    false);

  l1 = "BLÖC TEST";
  BOOST_CHECK_EQUAL(l1[0].size(), 5);
  BOOST_CHECK_EQUAL(l1.is_block_def(), false);

  l1 = "BLÖCK TEST";
  BOOST_CHECK_EQUAL(l1[0].size(), 6);
  BOOST_CHECK_EQUAL(l1.is_block_def(), false);

  l1 = "ßLOC TEST";
  BOOST_CHECK_EQUAL(l1[0].size(), 5);
  BOOST_CHECK_EQUAL(l1.is_block_def(), false);

  l1 = "ßLOCK TEST";
  BOOST_CHECK_EQUAL(l1[0].size(), 6);
  BOOST_CHECK_EQUAL(l1.is_block_def(), false);

  l1 = "ßLÖ TEST";
  BOOST_CHECK_EQUAL(l1[0].size(), 5);
  BOOST_CHECK_EQUAL(l1.is_block_def(), false);

  l1 = "DeCaY";
  BOOST_CHECK_EQUAL(l1.size(),      1);
  BOOST_CHECK_EQUAL(l1.data_size(), 1);
  BOOST_CHECK_EQUAL(l1.is_block_def(),     false);
  BOOST_CHECK_EQUAL(l1.is_comment_line(),  false);
  BOOST_CHECK_EQUAL(l1.is_data_line(),     false);

  l1 = "dEcAy TEST";
  BOOST_CHECK_EQUAL(l1.size(),      2);
  BOOST_CHECK_EQUAL(l1.data_size(), 2);
  BOOST_CHECK_EQUAL(l1.is_block_def(),    true);
  BOOST_CHECK_EQUAL(l1.is_comment_line(), false);
  BOOST_CHECK_EQUAL(l1.is_data_line(),    false);

  l1 = "dEcAy #TEST";
  BOOST_CHECK_EQUAL(l1.size(),      2);
  BOOST_CHECK_EQUAL(l1.data_size(), 1);
  BOOST_CHECK_EQUAL(l1.is_block_def(),    false);
  BOOST_CHECK_EQUAL(l1.is_comment_line(), false);
  BOOST_CHECK_EQUAL(l1.is_data_line(),    false);

  l1 = " 1 0.123 # comment ";
  BOOST_CHECK_EQUAL(l1.size(),      3);
  BOOST_CHECK_EQUAL(l1.data_size(), 2);
  BOOST_CHECK_EQUAL(l1.is_block_def(),    false);
  BOOST_CHECK_EQUAL(l1.is_comment_line(), false);
  BOOST_CHECK_EQUAL(l1.is_data_line(),    true);

  l1 = "a b";
  BOOST_CHECK_EQUAL(l1.size(),      2);
  BOOST_CHECK_EQUAL(l1.data_size(), 2);
  BOOST_CHECK_EQUAL(l1.is_block_def(),    false);
  BOOST_CHECK_EQUAL(l1.is_comment_line(), false);
  BOOST_CHECK_EQUAL(l1.is_data_line(),    true);

  l1 = "";
  BOOST_CHECK_EQUAL(l1.size(),      0);
  BOOST_CHECK_EQUAL(l1.data_size(), 0);
  BOOST_CHECK_EQUAL(l1.is_block_def(),    false);
  BOOST_CHECK_EQUAL(l1.is_comment_line(), false);
  BOOST_CHECK_EQUAL(l1.is_data_line(),    false);

  l1 = "1 2 3 4";
  l1[0] = l1[1] = l1[2] = l1[3] = "";
  BOOST_CHECK_EQUAL(l1.size(),      4);
  BOOST_CHECK_EQUAL(l1.data_size(), 4);
  BOOST_CHECK_EQUAL(l1.is_block_def(),    false);
  BOOST_CHECK_EQUAL(l1.is_comment_line(), false);
  BOOST_CHECK_EQUAL(l1.is_data_line(),    true);
}

BOOST_AUTO_TEST_CASE(testSwap)
{
  Line l1(" 1  23 4  5 ");
  Line l2("ab cd ef 567");

  const Line cl1 = l1;
  const Line cl2 = l2;

  BOOST_CHECK_EQUAL(l1, cl1);
  BOOST_CHECK_EQUAL(l2, cl2);

  l1.swap(l2);
  BOOST_CHECK_EQUAL(l2, cl1);
  BOOST_CHECK_EQUAL(l1, cl2);

  l2.swap(l1);
  BOOST_CHECK_EQUAL(l1, cl1);
  BOOST_CHECK_EQUAL(l2, cl2);
}

BOOST_AUTO_TEST_CASE(testReformat)
{
  Line l1;
  l1.reformat();
  BOOST_CHECK_EQUAL(l1.str(), "");

  l1 = "1 2 3 4 5 6";
  l1.reformat();
  BOOST_CHECK_EQUAL(l1.str(), "    1   2   3   4   5   6");

  l1 = "   BLOCK   test1  ";
  l1.reformat();
  BOOST_CHECK_EQUAL(l1.str(), "BLOCK test1");

  l1 = " 1 1.23456 # comment  ";
  l1.reformat();
  BOOST_CHECK_EQUAL(l1.str(), "    1   1.23456     # comment");

  l1 = "1 2 3";
  l1[0] = "";
  l1[2] = "";
  l1.reformat();
  BOOST_CHECK_EQUAL(l1.str(), "        2   ");
}

BOOST_AUTO_TEST_CASE(testUnComment)
{
  Line l1;

  BOOST_CHECK_EQUAL(l1.str(),   "");
  BOOST_CHECK_EQUAL(l1.empty(), true);

  l1.uncomment();
  BOOST_CHECK_EQUAL(l1.str(),   "");
  BOOST_CHECK_EQUAL(l1.empty(), true);

  l1.comment();
  BOOST_CHECK_EQUAL(l1.str(),   "");
  BOOST_CHECK_EQUAL(l1.empty(), true);

  l1 = "1 2 # 3 4";
  BOOST_CHECK_EQUAL(l1.str(),       "1 2 # 3 4");
  BOOST_CHECK_EQUAL(l1.empty(),     false);
  BOOST_CHECK_EQUAL(l1.size(),      3);
  BOOST_CHECK_EQUAL(l1.data_size(), 2);

  l1.comment();
  BOOST_CHECK_EQUAL(l1.str(),       "#1 2 # 3 4");
  BOOST_CHECK_EQUAL(l1.empty(),     false);
  BOOST_CHECK_EQUAL(l1.size(),      1);
  BOOST_CHECK_EQUAL(l1.data_size(), 0);

  l1.uncomment();
  BOOST_CHECK_EQUAL(l1.str(),       "1 2 # 3 4");
  BOOST_CHECK_EQUAL(l1.empty(),     false);
  BOOST_CHECK_EQUAL(l1.size(),      3);
  BOOST_CHECK_EQUAL(l1.data_size(), 2);

  l1 = "#";
  l1.comment();
  BOOST_CHECK_EQUAL(l1.str(),       "##");
  BOOST_CHECK_EQUAL(l1.size(),      1);
  BOOST_CHECK_EQUAL(l1.data_size(), 0);

  l1 = "";
  l1.uncomment();
  BOOST_CHECK_EQUAL(l1.str(),       "");
  BOOST_CHECK_EQUAL(l1.size(),      0);
  BOOST_CHECK_EQUAL(l1.data_size(), 0);
}

BOOST_AUTO_TEST_CASE(testInEquality)
{
  Line l1, l2;

  l1 = "1 2 3";
  l2 = l1;
  BOOST_CHECK_EQUAL(l1, l1);
  BOOST_CHECK_EQUAL(l2, l2);
  BOOST_CHECK_EQUAL(l1, l2);
  BOOST_CHECK_EQUAL(l2, l1);

  l2 += " 4";
  BOOST_CHECK_EQUAL(l1, l1);
  BOOST_CHECK_EQUAL(l2, l2);
  BOOST_CHECK_NE(l1, l2);
  BOOST_CHECK_NE(l2, l1);

  l1.clear();
  l2.clear();
  BOOST_CHECK_EQUAL(l1, l1);
  BOOST_CHECK_EQUAL(l2, l2);
  BOOST_CHECK_EQUAL(l1, l2);
  BOOST_CHECK_EQUAL(l2, l1);

  l1 = "1 2 3 4 5";
  l2 = "1 2 3 4 5";
  BOOST_CHECK_EQUAL(l1, l2);
  BOOST_CHECK_EQUAL(l2, l1);

  l1.clear();
  l2 = "";
  BOOST_CHECK_EQUAL(l1, l2);
  BOOST_CHECK_EQUAL(l2, l1);

  l1 = "1 2 3 4";
  l2 = "1 2 3  4";
  BOOST_CHECK_NE(l1, l2);
  BOOST_CHECK_NE(l2, l1);
  BOOST_CHECK(l1 != l2);
  BOOST_CHECK(l2 != l1);

  l1 = "1 2 3 4";
  l2 = "1 2 3 5";
  BOOST_CHECK_NE(l1, l2);
  BOOST_CHECK_NE(l2, l1);

  l1 = "1 2 3";
  l2 = "1 2 3";
  BOOST_CHECK_GE(l1, l2);
  BOOST_CHECK_GE(l2, l1);
  BOOST_CHECK_LE(l1, l2);
  BOOST_CHECK_LE(l2, l1);

  l1 = "1 2 3";
  l2 = "1 2 4";
  BOOST_CHECK_LT(l1, l2);
  BOOST_CHECK_LE(l1, l2);
  BOOST_CHECK_GT(l2, l1);
  BOOST_CHECK_GE(l2, l1);

  l1 = "#1 2 3";
  l2 = "1 2 3";
  l2[0] = "#1";
  BOOST_CHECK_NE(l1, l2);
  BOOST_CHECK_NE(l2, l1);

  l1 = "1 2";
  l2 = "1 2";
  l1[0] = "  1 2";
  l1[1] = "";
  l2[0] = "";
  l2[1] = "1 2 ";
  BOOST_CHECK_EQUAL(l1.str(),  l2.str());
  BOOST_CHECK_EQUAL(l1.size(), l2.size());
  BOOST_CHECK_NE(l1, l2);
  BOOST_CHECK_NE(l2, l1);
}

BOOST_AUTO_TEST_CASE(testOrder)
{
  Line l1;

  l1 = "2 4 3 1";
  sort(l1.begin(), l1.end());
  BOOST_CHECK_EQUAL(l1.str(), "1 2 3 4");

  l1 = "a d b c";
  sort(l1.begin(), l1.end());
  BOOST_CHECK_EQUAL(l1.str(), "a b c d");

  vector<Line> vl1;
  vl1.push_back(Line(" 3  0.1"));
  vl1.push_back(Line(" 1  0.2"));
  vl1.push_back(Line(" 2  0.3"));
  vl1.push_back(Line("BLOCK TEST"));

  BOOST_CHECK_EQUAL(vl1[0].str(), " 3  0.1");
  BOOST_CHECK_EQUAL(vl1[1].str(), " 1  0.2");
  BOOST_CHECK_EQUAL(vl1[2].str(), " 2  0.3");
  BOOST_CHECK_EQUAL(vl1[3].str(), "BLOCK TEST");

  sort(vl1.begin(), vl1.end());

  BOOST_CHECK_EQUAL(vl1[0].str(), "BLOCK TEST");
  BOOST_CHECK_EQUAL(vl1[1].str(), " 1  0.2");
  BOOST_CHECK_EQUAL(vl1[2].str(), " 2  0.3");
  BOOST_CHECK_EQUAL(vl1[3].str(), " 3  0.1");

  vector<Line> vl2;
  vl2.push_back(Line("1 3 2"));
  vl2.push_back(Line("2 4"));
  vl2.push_back(Line("BLOCK TEST2"));
  vl2.push_back(Line("2 3"));
  vl2.push_back(Line("# comment"));
  vl2.push_back(Line("DECAY 1"));
  vl2.push_back(Line("1 2 3"));
  vl2.push_back(Line("BLOCK TEST1"));

  sort(vl2.begin(), vl2.end());

  BOOST_CHECK_EQUAL(vl2[0].str(), "BLOCK TEST1");
  BOOST_CHECK_EQUAL(vl2[1].str(), "BLOCK TEST2");
  BOOST_CHECK_EQUAL(vl2[2].str(), "DECAY 1");
  BOOST_CHECK_EQUAL(vl2[3].str(), "# comment");
  BOOST_CHECK_EQUAL(vl2[4].str(), "1 2 3");
  BOOST_CHECK_EQUAL(vl2[5].str(), "1 3 2");
  BOOST_CHECK_EQUAL(vl2[6].str(), "2 3");
  BOOST_CHECK_EQUAL(vl2[7].str(), "2 4");
}

BOOST_AUTO_TEST_SUITE_END()
