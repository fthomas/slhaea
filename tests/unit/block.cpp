// SLHAea - containers for SUSY Les Houches Accord input/output
// Copyright © 2009-2010 Frank S. Thomas <frank@timepit.eu>
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file ../../LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <boost/concept/assert.hpp>
#include <boost/test/unit_test.hpp>
#include "slhaea.h"

using namespace std;
using namespace boost;
using namespace SLHAea;

BOOST_AUTO_TEST_SUITE(TestBlock)

BOOST_AUTO_TEST_CASE(assertConcepts)
{
  BOOST_CONCEPT_ASSERT((Mutable_ReversibleContainer<Block>));

  BOOST_CONCEPT_ASSERT((Mutable_RandomAccessIterator<Block::iterator>));
  BOOST_CONCEPT_ASSERT((Mutable_RandomAccessIterator<Block::reverse_iterator>));
  BOOST_CONCEPT_ASSERT((RandomAccessIterator<Block::const_iterator>));
  BOOST_CONCEPT_ASSERT((RandomAccessIterator<Block::const_reverse_iterator>));

  BOOST_CONCEPT_ASSERT((UnaryPredicate<Block::key_matches, Block::value_type>));
}

BOOST_AUTO_TEST_CASE(testName)
{
  Block b1, b2("test");

  BOOST_CHECK_EQUAL(b1.name(), "");
  BOOST_CHECK_EQUAL(b2.name(), "test");

  b1.name("test");
  b2.name("");

  BOOST_CHECK_EQUAL(b1.name(), "test");
  BOOST_CHECK_EQUAL(b2.name(), "");

  b1.swap(b2);

  BOOST_CHECK_EQUAL(b1.name(), "");
  BOOST_CHECK_EQUAL(b2.name(), "test");
}

BOOST_AUTO_TEST_CASE(testRename)
{
  string s1 = "BLOCK test\n";
  Block b1;
  b1.str(s1);

  BOOST_CHECK_EQUAL(b1.name(),     "test");
  BOOST_CHECK_EQUAL(b1.front()[1], "test");

  b1.rename("");
  BOOST_CHECK_EQUAL(b1.name(),     "");
  BOOST_CHECK_EQUAL(b1.front()[1], "");

  b1.rename("foo");
  BOOST_CHECK_EQUAL(b1.name(),     "foo");
  BOOST_CHECK_EQUAL(b1.front()[1], "foo");

  b1.rename("foo bar");
  BOOST_CHECK_EQUAL(b1.name(),     "foo bar");
  BOOST_CHECK_EQUAL(b1.front()[1], "foo bar");

  b1.clear();
  b1.rename("foo");
  BOOST_CHECK_EQUAL(b1.name(),  "foo");
  BOOST_CHECK_EQUAL(b1.empty(), true);
}

BOOST_AUTO_TEST_CASE(testReadWrite)
{
  string s1 =
    "BLOCK test\n"
    " 1  0.1\n"
    " 2  0.2\n";
  stringstream ss1(s1), ss2(s1);

  Block b1, b2, b3;
  b1.read(ss1);
  b2.str(s1);
  ss2 >> b3;

  BOOST_CHECK_EQUAL(b1, b2);
  BOOST_CHECK_EQUAL(b1, b3);
  BOOST_CHECK_EQUAL(b2, b3);

  BOOST_CHECK_EQUAL(b1.str(), s1);
  BOOST_CHECK_EQUAL(b2.str(), s1);
  BOOST_CHECK_EQUAL(b3.str(), s1);

  string s2 =
    "BLOCK test\n"
    " 1  0.1\n"
    "       \n"
    " 2  0.2";

  Block b4;
  b4.str(s2);

  BOOST_CHECK_EQUAL(b1, b4);
  BOOST_CHECK_NE(s1, s2);

  string s3 =
    "BLOCK test1\n" " 1  123\n"
    "BLOCK test2\n" " 2  234\n"
    "BLOCK test3\n" " 3  345\n";
  stringstream ss3(s3);
  Block b5(ss3), b6(ss3), b7("testNOT3");
  b7.read(ss3);

  BOOST_CHECK_EQUAL(b5.name(), "test1");
  BOOST_CHECK_EQUAL(b6.name(), "test2");
  BOOST_CHECK_EQUAL(b7.name(), "testNOT3");
  BOOST_CHECK_EQUAL(b5.size(), 2);
  BOOST_CHECK_EQUAL(b6.size(), 2);
  BOOST_CHECK_EQUAL(b7.size(), 2);
  BOOST_CHECK_EQUAL(b5.str(),  "BLOCK test1\n" " 1  123\n");
  BOOST_CHECK_EQUAL(b6.str(),  "BLOCK test2\n" " 2  234\n");
  BOOST_CHECK_EQUAL(b7.str(),  "BLOCK test3\n" " 3  345\n");
}

BOOST_AUTO_TEST_CASE(testSubscriptAtAccessors)
{
  Block b1;
  b1[""] = " 1 1 # 11";
  b1[""] = " 1 2 # 12";
  b1[""] = " 2 1 # 21";
  b1[""] = " 10 1 2";
  b1[""] = " 11 1 2 3";
  b1[""] = " 12 1 2 3 4";
  const Block cb1 = b1;

  string s1 =
      " 1 1 # 11\n"
      " 1 2 # 12\n"
      " 2 1 # 21\n"
      " 10 1 2\n"
      " 11 1 2 3\n"
      " 12 1 2 3 4\n";

  BOOST_CHECK_EQUAL(b1["1"].str(), " 1 1 # 11");
  BOOST_CHECK_EQUAL(b1["2"].str(), " 2 1 # 21");
  BOOST_CHECK_EQUAL(cb1.at("1").str(), " 1 1 # 11");
  BOOST_CHECK_EQUAL(cb1.at("2").str(), " 2 1 # 21");

  BOOST_CHECK_EQUAL(b1[1].str(), " 1 1 # 11");
  BOOST_CHECK_EQUAL(b1[2].str(), " 2 1 # 21");
  BOOST_CHECK_EQUAL(cb1.at(1).str(), " 1 1 # 11");
  BOOST_CHECK_EQUAL(cb1.at(2).str(), " 2 1 # 21");

  BOOST_CHECK_EQUAL(b1.at({"1", "1"}).str(), " 1 1 # 11");
  BOOST_CHECK_EQUAL(b1.at({"1", "2"}).str(), " 1 2 # 12");
  BOOST_CHECK_EQUAL(b1.at({"2", "1"}).str(), " 2 1 # 21");
  BOOST_CHECK_EQUAL(b1.at({"10", "1", "2"}).str(), " 10 1 2");
  BOOST_CHECK_EQUAL(b1.at({"11", "1", "2", "3"}).str(), " 11 1 2 3");
  BOOST_CHECK_EQUAL(b1.at({"12", "1", "2", "3", "4"}).str(), " 12 1 2 3 4");

  BOOST_CHECK_EQUAL(b1.at({1, 1}).str(), " 1 1 # 11");
  BOOST_CHECK_EQUAL(b1.at({1, 2}).str(), " 1 2 # 12");
  BOOST_CHECK_EQUAL(b1.at({2, 1}).str(), " 2 1 # 21");
  BOOST_CHECK_EQUAL(b1.at({10, 1, 2}).str(), " 10 1 2");
  BOOST_CHECK_EQUAL(b1.at({11, 1, 2, 3}).str(), " 11 1 2 3");
  BOOST_CHECK_EQUAL(b1.at({12, 1, 2, 3, 4}).str(), " 12 1 2 3 4");

  BOOST_CHECK_EQUAL(cb1.at({"1", "1"}).str(), " 1 1 # 11");
  BOOST_CHECK_EQUAL(cb1.at({"1", "2"}).str(), " 1 2 # 12");
  BOOST_CHECK_EQUAL(cb1.at({"2", "1"}).str(), " 2 1 # 21");

  BOOST_CHECK_EQUAL(cb1.at({1, 1}).str(), " 1 1 # 11");
  BOOST_CHECK_EQUAL(cb1.at({1, 2}).str(), " 1 2 # 12");
  BOOST_CHECK_EQUAL(cb1.at({2, 1}).str(), " 2 1 # 21");

  vector<string> vs1(2);
  vs1[0] = "1";
  vs1[1] = "2";

  vector<int> vi1(2);
  vi1[0] = 1;
  vi1[1] = 2;

  BOOST_CHECK_EQUAL(b1[vs1].str(), " 1 2 # 12");
  BOOST_CHECK_EQUAL(b1[vi1].str(), " 1 2 # 12");
  BOOST_CHECK_EQUAL(b1.at(vs1).str(),  " 1 2 # 12");
  BOOST_CHECK_EQUAL(b1.at(vi1).str(),  " 1 2 # 12");
  BOOST_CHECK_EQUAL(cb1.at(vs1).str(), " 1 2 # 12");
  BOOST_CHECK_EQUAL(cb1.at(vi1).str(), " 1 2 # 12");

  vs1[0] = "(any)";
  BOOST_CHECK_EQUAL(b1[vs1].str(),     " 1 2 # 12");
  BOOST_CHECK_EQUAL(cb1.at(vs1).str(), " 1 2 # 12");

  vs1[1] = "(any)";
  BOOST_CHECK_EQUAL(b1[vs1].str(),     " 1 1 # 11");
  BOOST_CHECK_EQUAL(cb1.at(vs1).str(), " 1 1 # 11");

  BOOST_CHECK_THROW(b1.at({9, 8 ,7}),  out_of_range);
  BOOST_CHECK_THROW(cb1.at({9, 8 ,7}), out_of_range);

  BOOST_CHECK_THROW(b1.at(""),  out_of_range);
  BOOST_CHECK_THROW(cb1.at(""), out_of_range);

  int no_ind = -32768;

  BOOST_CHECK_THROW(b1.at(no_ind),  out_of_range);
  BOOST_CHECK_THROW(cb1.at(no_ind), out_of_range);
}

BOOST_AUTO_TEST_CASE(testGeneralAccessors)
{
  Block b1;
  b1[""] = " 1 1 # 11";
  b1[""] = " 1 2 # 12";
  b1[""] = " 1 2 3 4 # 1234";
  b1[""] = " 2 1 # 21";
  const Block cb1 = b1;

  string s1 =
    " 1 1 # 11\n"
    " 1 2 # 12\n"
    " 1 2 3 4 # 1234\n"
    " 2 1 # 21\n";

  BOOST_CHECK_EQUAL(b1.str(),  s1);
  BOOST_CHECK_EQUAL(cb1.str(), s1);

  BOOST_CHECK_EQUAL(b1.front(), cb1.front());
  BOOST_CHECK_EQUAL(b1.back(),  cb1.back());

  BOOST_CHECK_EQUAL(b1.front().str(), " 1 1 # 11");
  BOOST_CHECK_EQUAL(b1.back().str(),  " 2 1 # 21");

  BOOST_CHECK_EQUAL(b1.front(), b1.at({1, 1}));
  BOOST_CHECK_EQUAL(b1.back(),  b1.at({2, 1}));
}

BOOST_AUTO_TEST_CASE(testIterators)
{
  Block b1;
  b1[""] = " 1 1 # 1 1";
  b1[""] = " 1 2 # 1 2";
  b1[""] = " 2 1 # 2 1";
  b1[""] = " 2 2 # 2 2";
  const Block cb1 = b1;

  BOOST_CHECK(b1.find(vector<string>(0))       == b1.end());
  BOOST_CHECK(cb1.find(vector<string>(0))      == cb1.end());
  BOOST_CHECK(cb1.find(vector<string>(4, "A")) == cb1.end());

  BOOST_CHECK_EQUAL((*(b1.begin())).str(),  (*(cb1.begin())).str());
  BOOST_CHECK_EQUAL((*(b1.end()-1)).str(),  (*(cb1.end()-1)).str());
  BOOST_CHECK_EQUAL((*(b1.rbegin())).str(), (*(cb1.rbegin())).str());
  BOOST_CHECK_EQUAL((*(b1.rend()-1)).str(), (*(cb1.rend()-1)).str());

  BOOST_CHECK(b1.begin()+0 == b1.end()-4);
  BOOST_CHECK(b1.begin()+1 == b1.end()-3);
  BOOST_CHECK(b1.begin()+2 == b1.end()-2);
  BOOST_CHECK(b1.begin()+3 == b1.end()-1);

  BOOST_CHECK(b1.rbegin()+0 == b1.rend()-4);
  BOOST_CHECK(b1.rbegin()+1 == b1.rend()-3);
  BOOST_CHECK(b1.rbegin()+2 == b1.rend()-2);
  BOOST_CHECK(b1.rbegin()+3 == b1.rend()-1);

  BOOST_CHECK(b1.cbegin()+0 == b1.cend()-4);
  BOOST_CHECK(b1.cbegin()+1 == b1.cend()-3);
  BOOST_CHECK(b1.cbegin()+2 == b1.cend()-2);
  BOOST_CHECK(b1.cbegin()+3 == b1.cend()-1);

  BOOST_CHECK(b1.crbegin()+0 == b1.crend()-4);
  BOOST_CHECK(b1.crbegin()+1 == b1.crend()-3);
  BOOST_CHECK(b1.crbegin()+2 == b1.crend()-2);
  BOOST_CHECK(b1.crbegin()+3 == b1.crend()-1);

  BOOST_CHECK(cb1.begin()+0 == cb1.end()-4);
  BOOST_CHECK(cb1.begin()+1 == cb1.end()-3);
  BOOST_CHECK(cb1.begin()+2 == cb1.end()-2);
  BOOST_CHECK(cb1.begin()+3 == cb1.end()-1);

  BOOST_CHECK(cb1.rbegin()+0 == cb1.rend()-4);
  BOOST_CHECK(cb1.rbegin()+1 == cb1.rend()-3);
  BOOST_CHECK(cb1.rbegin()+2 == cb1.rend()-2);
  BOOST_CHECK(cb1.rbegin()+3 == cb1.rend()-1);

  BOOST_CHECK(cb1.cbegin()+0 == cb1.cend()-4);
  BOOST_CHECK(cb1.cbegin()+1 == cb1.cend()-3);
  BOOST_CHECK(cb1.cbegin()+2 == cb1.cend()-2);
  BOOST_CHECK(cb1.cbegin()+3 == cb1.cend()-1);

  BOOST_CHECK(cb1.crbegin()+0 == cb1.crend()-4);
  BOOST_CHECK(cb1.crbegin()+1 == cb1.crend()-3);
  BOOST_CHECK(cb1.crbegin()+2 == cb1.crend()-2);
  BOOST_CHECK(cb1.crbegin()+3 == cb1.crend()-1);

  vector<string> vs1(2);
  vs1[0] = "1";
  vs1[1] = "1";

  vector<string> vs2(2);
  vs2[0] = "2";
  vs2[1] = "2";

  BOOST_CHECK_EQUAL((*(b1.begin())).str(),  (*b1.find(vs1)).str());
  BOOST_CHECK_EQUAL((*(b1.end()-1)).str(),  (*b1.find(vs2)).str());
  BOOST_CHECK_EQUAL((*(b1.rbegin())).str(), (*b1.find(vs2)).str());
  BOOST_CHECK_EQUAL((*(b1.rend()-1)).str(), (*b1.find(vs1)).str());

  vector<string> vs3(2);
  vs3[0] = "2";
  vs3[1] = "1";

  BOOST_CHECK_EQUAL((*(b1.begin()+2)).str(),   (*b1.find(vs3)).str());
  BOOST_CHECK_EQUAL((*(b1.end()-2)).str(),     (*b1.find(vs3)).str());
  BOOST_CHECK_EQUAL((*(cb1.begin()+2)).str(),  (*cb1.find(vs3)).str());
  BOOST_CHECK_EQUAL((*(cb1.end()-2)).str(),    (*cb1.find(vs3)).str());
  BOOST_CHECK_EQUAL((*(b1.rbegin()+1)).str(),  (*b1.find(vs3)).str());
  BOOST_CHECK_EQUAL((*(b1.rend()-3)).str(),    (*b1.find(vs3)).str());
  BOOST_CHECK_EQUAL((*(cb1.rbegin()+1)).str(), (*cb1.find(vs3)).str());
  BOOST_CHECK_EQUAL((*(cb1.rend()-3)).str(),   (*cb1.find(vs3)).str());
}

BOOST_AUTO_TEST_CASE(testStaticFind)
{
  Block b1;
  b1[""] = "test 10";
  b1[""] = "test 20";
  b1[""] = "test 30";
  vector<string> v1(1, "test"), v2(1, "foo");

  Line l1 = *Block::find(b1.begin(),   b1.end(),  v1);
  Line l2 = *Block::find(b1.begin()+1, b1.end(),  v1);
  Line l3 = *Block::find(b1.rbegin(),  b1.rend(), v1);

  BOOST_CHECK_EQUAL(l1.at(1), "10");
  BOOST_CHECK_EQUAL(l2.at(1), "20");
  BOOST_CHECK_EQUAL(l3.at(1), "30");

  Block::iterator it = Block::find(b1.begin(), b1.end(), v2);
  Block::const_iterator cit = Block::find(b1.cbegin(), b1.cend(), v2);
  BOOST_CHECK(it  == b1.end());
  BOOST_CHECK(cit == b1.cend());
}

BOOST_AUTO_TEST_CASE(testFindBlockDef)
{
  Block b1;
  b1[""] = "# a comment";
  b1[""] = "BLOCK test Q= 10 # block def";
  b1[""] = " 1 123  # comment";
  b1[""] = " 2 234  # comment";
  b1[""] = "BLOCK foo # another block def";
  const Block cb1 = b1;

  BOOST_CHECK_EQUAL(b1.find_block_def()->at(1),  "test");
  BOOST_CHECK_EQUAL(cb1.find_block_def()->at(1), "test");
  BOOST_CHECK_EQUAL(b1.find_block_def()->at(3),  "10");
  BOOST_CHECK_EQUAL(cb1.find_block_def()->at(3), "10");

  Block b2;
  BOOST_CHECK(b2.find_block_def() == b2.end());

  b2[""] = " 1 2 3";
  b2[""] = " 4 5 6";
  b2[""] = "# comment";
  BOOST_CHECK(b2.find_block_def() == b2.end());
}

BOOST_AUTO_TEST_CASE(testIntrospection)
{
  Block b1;

  BOOST_CHECK_GT(b1.max_size(), 0);
  BOOST_CHECK_EQUAL(b1.size(),  0);
  BOOST_CHECK_EQUAL(b1.empty(), true);

  b1[""] = " a A # aA";
  b1[""] = " a a # aa";

  vector<string> vs1(2);
  vs1[0] = "a";
  vs1[1] = "A";

  vector<string> vs2(2);
  vs2[0] = "A";
  vs2[1] = "b";

  BOOST_CHECK_EQUAL(b1.count(vs1), 2);
  BOOST_CHECK_EQUAL(b1.count(vs2), 0);
  BOOST_CHECK_EQUAL(b1.size(),     2);

  b1[""] = " A a # Aa";
  b1[""] = " a B # aB";

  BOOST_CHECK_EQUAL(b1.count(vs1), 3);
  BOOST_CHECK_EQUAL(b1.count(vs2), 1);
  BOOST_CHECK_EQUAL(b1.size(),     4);

  b1[""] = " a A a # aAa";
  b1[""] = " A b A # AbA";

  BOOST_CHECK_EQUAL(b1.count(vs1), 4);
  BOOST_CHECK_EQUAL(b1.count(vs2), 2);
  BOOST_CHECK_EQUAL(b1.size(),     6);

  vs1.push_back("A");
  vs2.push_back("A");

  BOOST_CHECK_EQUAL(b1.count(vs1), 1);
  BOOST_CHECK_EQUAL(b1.count(vs2), 1);
  BOOST_CHECK_EQUAL(b1.size(),     6);

  vs1.push_back("B");
  vs2.push_back("B");
  vs1.push_back("C");
  vs2.push_back("C");

  BOOST_CHECK_EQUAL(b1.count(vs1), 0);
  BOOST_CHECK_EQUAL(b1.count(vs2), 0);
  BOOST_CHECK_EQUAL(b1.size(),     6);

  b1.clear();

  BOOST_CHECK_EQUAL(b1.size(),  0);
  BOOST_CHECK_EQUAL(b1.empty(), true);

  BOOST_CHECK_EQUAL(b1.count(vs1), 0);
  BOOST_CHECK_EQUAL(b1.count(vector<string>(0)), 0);
}

BOOST_AUTO_TEST_CASE(tesDataSize)
{
  Block b1;
  BOOST_CHECK_EQUAL(b1.size(),      0);
  BOOST_CHECK_EQUAL(b1.data_size(), 0);

  b1[""] = "# comment";
  b1[""] = "BLOCK test1";
  b1[""] = "    1   1.2";
  b1[""] = "    2   3.4";
  BOOST_CHECK_EQUAL(b1.size(),      4);
  BOOST_CHECK_EQUAL(b1.data_size(), 2);

  b1[""] = "    3   5.6";
  BOOST_CHECK_EQUAL(b1.size(),      5);
  BOOST_CHECK_EQUAL(b1.data_size(), 3);

  b1[""] = "#   4   7.8";
  BOOST_CHECK_EQUAL(b1.size(),      6);
  BOOST_CHECK_EQUAL(b1.data_size(), 3);

  b1.clear();
  BOOST_CHECK_EQUAL(b1.size(),      0);
  BOOST_CHECK_EQUAL(b1.data_size(), 0);
}

BOOST_AUTO_TEST_CASE(testPushPop)
{
  Block b1;

  BOOST_CHECK_EQUAL(b1.size(),  0);
  BOOST_CHECK_EQUAL(b1.empty(), true);

  b1.push_back(" 1 1 # 11");
  b1.push_back(Line(" 1 1 # 11"));

  BOOST_CHECK_EQUAL(b1.size(),  2);
  BOOST_CHECK_EQUAL(b1.empty(), false);

  string s1 =
    " 1 1 # 11\n"
    " 1 1 # 11\n";

  BOOST_CHECK_EQUAL(b1.str(), s1);

  b1.pop_back();

  BOOST_CHECK_EQUAL(b1.size(),  1);
  BOOST_CHECK_EQUAL(b1.empty(), false);

  b1.pop_back();

  BOOST_CHECK_EQUAL(b1.size(),  0);
  BOOST_CHECK_EQUAL(b1.empty(), true);
}

BOOST_AUTO_TEST_CASE(testInsertErase)
{
  Block b1("t1");
  b1[""] = " 1 1 # 11";
  b1[""] = " 1 5 # 15";
  const Block cb1 = b1;

  b1.insert(b1.begin()+1, Line(" 1 2 # 12"));

  string s1 =
    " 1 1 # 11\n"
    " 1 2 # 12\n"
    " 1 5 # 15\n";

  BOOST_CHECK_EQUAL(b1.str(),  s1);
  BOOST_CHECK_EQUAL(b1.size(), 3);

  vector<Line> vl1(2);
  vl1[0] = Line(" 1 3 # 13");
  vl1[1] = Line(" 1 4 # 14");

  b1.insert(b1.begin()+2, vl1.begin(), vl1.end());

  string s2 =
    " 1 1 # 11\n"
    " 1 2 # 12\n"
    " 1 3 # 13\n"
    " 1 4 # 14\n"
    " 1 5 # 15\n";

  BOOST_CHECK_EQUAL(b1.str(),  s2);
  BOOST_CHECK_EQUAL(b1.size(), 5);

  b1.erase(b1.end()-3, b1.end()-1);

  BOOST_CHECK_EQUAL(b1.str(),  s1);
  BOOST_CHECK_EQUAL(b1.size(), 3);

  b1.erase(b1.end()-2);

  BOOST_CHECK_EQUAL(b1, cb1);
  BOOST_CHECK_EQUAL(b1.size(), 2);
}

BOOST_AUTO_TEST_CASE(testEraseFirst)
{
  Block b1;
  b1[""] = " 1 1";
  b1[""] = " 2 1";
  b1[""] = " 2 2";
  b1[""] = " 1 1";
  Block::iterator it;

  vector<string> v0(1, "0"), v1(1, "1"), v2(1, "2");

  it = b1.erase_first(v0);
  BOOST_CHECK(it == b1.end());
  BOOST_CHECK_EQUAL(b1.count(v1), 2);
  BOOST_CHECK_EQUAL(b1.count(v2), 2);
  BOOST_CHECK_EQUAL(b1.size(),    4);

  it = b1.erase_first(v2);
  BOOST_CHECK(it == b1.end()-2);
  BOOST_CHECK_EQUAL(b1.count(v1), 2);
  BOOST_CHECK_EQUAL(b1.count(v2), 1);
  BOOST_CHECK_EQUAL(b1.size(),    3);

  it = b1.erase_first(v1);
  BOOST_CHECK(it == b1.begin());
  BOOST_CHECK_EQUAL(b1.count(v1), 1);
  BOOST_CHECK_EQUAL(b1.count(v2), 1);
  BOOST_CHECK_EQUAL(b1.size(),    2);

  it = b1.erase_first(v2);
  BOOST_CHECK(it == b1.begin());
  BOOST_CHECK_EQUAL(b1.count(v1), 1);
  BOOST_CHECK_EQUAL(b1.count(v2), 0);
  BOOST_CHECK_EQUAL(b1.size(),    1);

  it = b1.erase_first(v1);
  BOOST_CHECK(it == b1.end());
  BOOST_CHECK_EQUAL(b1.count(v1), 0);
  BOOST_CHECK_EQUAL(b1.count(v2), 0);
  BOOST_CHECK_EQUAL(b1.size(),    0);

  it = b1.erase_first(v1);
  BOOST_CHECK(it == b1.end());
  BOOST_CHECK_EQUAL(b1.count(v1), 0);
  BOOST_CHECK_EQUAL(b1.count(v2), 0);
  BOOST_CHECK_EQUAL(b1.size(),    0);
}

BOOST_AUTO_TEST_CASE(testEraseLast)
{
  Block b1;
  b1[""] = " 1 1";
  b1[""] = " 2 1";
  b1[""] = " 2 2";
  b1[""] = " 1 1";
  Block::iterator it;

  vector<string> v0(1, "0"), v1(1, "1"), v2(1, "2");

  it = b1.erase_last(v0);
  BOOST_CHECK(it == b1.end());
  BOOST_CHECK_EQUAL(b1.count(v1), 2);
  BOOST_CHECK_EQUAL(b1.count(v2), 2);
  BOOST_CHECK_EQUAL(b1.size(),    4);

  it = b1.erase_last(v2);
  BOOST_CHECK(it == b1.end()-1);
  BOOST_CHECK_EQUAL(b1.count(v1), 2);
  BOOST_CHECK_EQUAL(b1.count(v2), 1);
  BOOST_CHECK_EQUAL(b1.size(),    3);

  it = b1.erase_last(v1);
  BOOST_CHECK(it == b1.end());
  BOOST_CHECK_EQUAL(b1.count(v1), 1);
  BOOST_CHECK_EQUAL(b1.count(v2), 1);
  BOOST_CHECK_EQUAL(b1.size(),    2);

  it = b1.erase_last(v2);
  BOOST_CHECK(it == b1.end());
  BOOST_CHECK_EQUAL(b1.count(v1), 1);
  BOOST_CHECK_EQUAL(b1.count(v2), 0);
  BOOST_CHECK_EQUAL(b1.size(),    1);

  it = b1.erase_last(v1);
  BOOST_CHECK(it == b1.end());
  BOOST_CHECK_EQUAL(b1.count(v1), 0);
  BOOST_CHECK_EQUAL(b1.count(v2), 0);
  BOOST_CHECK_EQUAL(b1.size(),    0);

  it = b1.erase_last(v1);
  BOOST_CHECK(it == b1.end());
  BOOST_CHECK_EQUAL(b1.count(v1), 0);
  BOOST_CHECK_EQUAL(b1.count(v2), 0);
  BOOST_CHECK_EQUAL(b1.size(),    0);
}

BOOST_AUTO_TEST_CASE(testErase)
{
  Block b1;
  b1[""] = " 1 1";
  b1[""] = " 2 1";
  b1[""] = " 2 2";
  b1[""] = " 1 1";

  vector<string> v0(1, "0"), v1(1, "1"), v2(1, "2");

  BOOST_CHECK_EQUAL(b1.erase(v0), 0);
  BOOST_CHECK_EQUAL(b1.count(v1), 2);
  BOOST_CHECK_EQUAL(b1.count(v2), 2);
  BOOST_CHECK_EQUAL(b1.size(),    4);

  BOOST_CHECK_EQUAL(b1.erase(v2), 2);
  BOOST_CHECK_EQUAL(b1.count(v1), 2);
  BOOST_CHECK_EQUAL(b1.count(v2), 0);
  BOOST_CHECK_EQUAL(b1.size(),    2);

  BOOST_CHECK_EQUAL(b1.erase(v1), 2);
  BOOST_CHECK_EQUAL(b1.count(v1), 0);
  BOOST_CHECK_EQUAL(b1.count(v2), 0);
  BOOST_CHECK_EQUAL(b1.size(),    0);

  BOOST_CHECK_EQUAL(b1.erase(v1), 0);
  BOOST_CHECK_EQUAL(b1.count(v1), 0);
  BOOST_CHECK_EQUAL(b1.count(v2), 0);
  BOOST_CHECK_EQUAL(b1.size(),    0);
}

BOOST_AUTO_TEST_CASE(testSwap)
{
  Block b1("t1");
  b1[""] = " 1 2 # 12";
  b1[""] = " 1 3 # 13";
  const Block cb1 = b1;

  Block b2("t2");
  b2[""] = " 2 1 # 21";
  b2[""] = " 2 3 # 23";
  const Block cb2 = b2;

  BOOST_CHECK_EQUAL(b1, cb1);
  BOOST_CHECK_EQUAL(b2, cb2);

  b1.swap(b2);

  BOOST_CHECK_EQUAL(b1, cb2);
  BOOST_CHECK_EQUAL(b2, cb1);

  b2.swap(b1);

  BOOST_CHECK_EQUAL(b1, cb1);
  BOOST_CHECK_EQUAL(b2, cb2);

  b1.clear();
  b2.clear();

  BOOST_CHECK_EQUAL(b1, b2);
  BOOST_CHECK_EQUAL(b1.str(), "");
  BOOST_CHECK_EQUAL(b2.str(), "");
}

BOOST_AUTO_TEST_CASE(testReformat)
{
  Block b1("t1");
  b1[""] = "  BLOCK  t1";
  b1[""] = "   DECAY  t2";
  b1[""] = "1 1 1 1";
  b1[""] = " 2 2 2 2";
  b1[""] = "  # 3 3 3";

  b1.reformat();
  BOOST_CHECK_EQUAL(b1.str(),
    "BLOCK t1\n"
    "DECAY t2\n"
    "    1   1   1   1\n"
    "    2   2   2   2\n"
    "# 3 3 3\n");
}

BOOST_AUTO_TEST_CASE(testUnComment)
{
  Block b1("t1");
  b1[""] = " 1 2 # 12";
  b1[""] = " 2 3 # 23";

  vector<string> vs1(2);
  vs1[0] = "1";
  vs1[1] = "2";

  BOOST_CHECK(b1.find(vs1) == b1.begin());

  b1.comment();

  BOOST_CHECK(b1.find(vs1) == b1.end());
  BOOST_CHECK_EQUAL(b1.front().str(), "# 1 2 # 12");
  BOOST_CHECK_EQUAL(b1.back().str(),  "# 2 3 # 23");

  b1.comment();

  BOOST_CHECK(b1.find(vs1) == b1.end());
  BOOST_CHECK_EQUAL(b1.front().str(), "## 1 2 # 12");
  BOOST_CHECK_EQUAL(b1.back().str(),  "## 2 3 # 23");

  b1.uncomment();
  b1.uncomment();

  BOOST_CHECK(b1.find(vs1) == b1.begin());
  BOOST_CHECK_EQUAL(b1.front().str(), " 1 2 # 12");
  BOOST_CHECK_EQUAL(b1.back().str(),  " 2 3 # 23");

  b1.uncomment();

  BOOST_CHECK(b1.find(vs1) == b1.begin());
  BOOST_CHECK_EQUAL(b1.front().str(), " 1 2 # 12");
  BOOST_CHECK_EQUAL(b1.back().str(),  " 2 3 # 23");
}

BOOST_AUTO_TEST_CASE(testKeyMatches)
{
  Line l1("one TWO 3");
  vector<string> key;
  key.push_back("oNe");
  Block::key_matches pred(key);

  BOOST_CHECK_EQUAL(pred(l1), true);

  l1[0] = "1";
  BOOST_CHECK_EQUAL(pred(l1), false);

  key[0] = "1";
  pred.set_key(key);
  BOOST_CHECK_EQUAL(pred(l1), true);

  key[0] = "(any)";
  key.push_back("two");
  pred.set_key(key);
  BOOST_CHECK_EQUAL(pred(l1), true);

  key[0] = "(any)";
  key[1] = "(any)";
  key.push_back("(any)");
  pred.set_key(key);
  BOOST_CHECK_EQUAL(pred(l1), true);

  key.push_back("(any)");
  pred.set_key(key);
  BOOST_CHECK_EQUAL(pred(l1), false);

  l1 = "1 2 3 4";
  key[0] = "1";
  key[1] = "2";
  key[2] = "3";
  key[3] = "4";
  pred.set_key(key);
  BOOST_CHECK_EQUAL(pred(l1), true);

  key.clear();
  pred.set_key(key);
  BOOST_CHECK_EQUAL(pred(l1), false);
}

BOOST_AUTO_TEST_CASE(testInEquality)
{
  Block b1("t1");
  b1[""] = " 1 2 # 12";
  b1[""] = " 2 3 # 23";

  Block b2("t1");
  b2[""] = " 1 2 # 12";
  b2[""] = " 2 3 # 23";

  BOOST_CHECK_EQUAL(b1, b1);
  BOOST_CHECK_EQUAL(b2, b2);
  BOOST_CHECK_EQUAL(b1, b2);
  BOOST_CHECK_EQUAL(b2, b1);
  BOOST_CHECK_GE(b1, b2);
  BOOST_CHECK_LE(b1, b2);

  b1["2"] = " 2 4 # 24";

  BOOST_CHECK_GE(b1, b2);
  BOOST_CHECK_GT(b1, b2);
  BOOST_CHECK_LE(b2, b1);
  BOOST_CHECK_LT(b2, b1);

  BOOST_CHECK_EQUAL(b1, b1);
  BOOST_CHECK_EQUAL(b2, b2);
  BOOST_CHECK_NE(b1, b2);
  BOOST_CHECK_NE(b2, b1);

  b1.clear();
  b2.clear();

  BOOST_CHECK_EQUAL(b1, b1);
  BOOST_CHECK_EQUAL(b2, b2);
  BOOST_CHECK_EQUAL(b1, b2);
  BOOST_CHECK_EQUAL(b2, b1);

  b1.name("test1");
  b2.name("test2");

  BOOST_CHECK_NE(b1, b2);
  BOOST_CHECK_NE(b2, b1);
  BOOST_CHECK(b1 != b2);
  BOOST_CHECK(b2 != b1);

  b2.name("test1");

  BOOST_CHECK_EQUAL(b1, b2);
  BOOST_CHECK_EQUAL(b2, b1);

  b2.push_back(" 1 2 # 12");

  BOOST_CHECK_NE(b1, b2);
  BOOST_CHECK_NE(b2, b1);
}

BOOST_AUTO_TEST_CASE(testOrder)
{
  Block b1;
  b1[""] = "BLOCK test1";
  b1[""] = " 1  0.1";

  Block b2;
  b2[""] = "BLOCK test2";
  b2[""] = " 2  0.2";

  vector<Block> vb1;
  vb1.push_back(b2);
  vb1.push_back(b1);

  BOOST_CHECK_EQUAL(vb1[0], b2);
  BOOST_CHECK_EQUAL(vb1[1], b1);

  sort(vb1.begin(), vb1.end());

  BOOST_CHECK_EQUAL(vb1[0], b1);
  BOOST_CHECK_EQUAL(vb1[1], b2);

  Block b3;
  b3[""] = " 3  0.31";
  b3[""] = " 3  0.32";

  vb1.insert(vb1.begin(), b3);

  BOOST_CHECK_EQUAL(vb1[0], b3);
  BOOST_CHECK_EQUAL(vb1[1], b1);
  BOOST_CHECK_EQUAL(vb1[2], b2);

  sort(vb1.begin(), vb1.end());

  BOOST_CHECK_EQUAL(vb1[0], b1);
  BOOST_CHECK_EQUAL(vb1[1], b2);
  BOOST_CHECK_EQUAL(vb1[2], b3);

  b1.name("Z");
  b2.name("Y");
  b3.name("X");
  vb1[0] = b1;
  vb1[1] = b2;
  vb1[2] = b3;

  sort(vb1.begin(), vb1.end());

  BOOST_CHECK_EQUAL(vb1[0], b3);
  BOOST_CHECK_EQUAL(vb1[1], b2);
  BOOST_CHECK_EQUAL(vb1[2], b1);
}

BOOST_AUTO_TEST_SUITE_END()
